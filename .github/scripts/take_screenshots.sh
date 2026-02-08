#!/bin/bash
# SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
# SPDX-License-Identifier: GPL-2.0-or-later

# Screenshot generation script for QtEmu CI
# Don't use set -e - we want to continue even if some screenshots fail

SCREENSHOT_DIR="${1:-screenshots}"
mkdir -p "$SCREENSHOT_DIR"

# Pre-configure settings to skip first-run wizard
mkdir -p ~/.config/QtEmu
mkdir -p ~/.qtemu/logs

# Write QSettings for QtEmu (INI format on Linux)
cat > ~/.config/QtEmu/QtEmu.conf << 'QSETTINGS'
[Configuration]
firstrunwizard=false
language=en
qemuBinaryPath=/usr/bin

[DataFolder]
QtEmuData=/home/runner/.qtemu/
QtEmuLogs=/home/runner/.qtemu/logs
QSETTINGS

# Replace paths with actual home
sed -i "s|/home/runner|$HOME|g" ~/.config/QtEmu/QtEmu.conf

echo "=== QSettings content ==="
cat ~/.config/QtEmu/QtEmu.conf
echo "========================="

# Helper: wait for a window matching a name pattern
wait_for_window() {
    local pattern="$1"
    local timeout="${2:-15}"
    local count=0
    local wid=""
    while [ -z "$wid" ]; do
        wid=$(xdotool search --name "$pattern" 2>/dev/null | head -1)
        if [ -n "$wid" ]; then
            echo "$wid"
            return 0
        fi
        sleep 0.5
        count=$((count + 1))
        if [ $count -ge $((timeout * 2)) ]; then
            echo "Timeout waiting for window: $pattern" >&2
            return 1
        fi
    done
}

# Helper: take screenshot of a specific window
screenshot_window() {
    local window_id="$1"
    local filename="$2"
    if [ -z "$window_id" ]; then
        echo "ERROR: No window ID for $filename" >&2
        return 1
    fi
    sleep 0.5
    if import -window "$window_id" "$SCREENSHOT_DIR/$filename"; then
        echo "Captured: $filename"
    else
        echo "FAILED to capture: $filename" >&2
    fi
}

# Helper: take full desktop screenshot
screenshot_desktop() {
    local filename="$1"
    sleep 0.5
    if import -window root "$SCREENSHOT_DIR/$filename"; then
        echo "Captured desktop: $filename"
    else
        echo "FAILED to capture desktop: $filename" >&2
    fi
}

echo "=== Starting QtEmu for screenshots ==="
echo "DISPLAY=$DISPLAY"
echo "Working directory: $(pwd)"
echo "QtEmu binary exists: $(test -f ./build/QtEmu && echo YES || echo NO)"

# Take a pre-launch desktop screenshot
screenshot_desktop "00-desktop-before-launch.png"

# 1. Main Window screenshot
echo "--- Launching QtEmu ---"
./build/QtEmu &
APP_PID=$!
echo "Started QtEmu with PID: $APP_PID"
sleep 5

# List all windows
echo "=== Available windows ==="
xdotool search --name "" 2>/dev/null | while read wid; do
    name=$(xdotool getwindowname "$wid" 2>/dev/null || echo "unknown")
    echo "  Window $wid: $name"
done
echo "========================="

MAIN_WID=$(wait_for_window "QtEmu" 20)
echo "Main window ID: $MAIN_WID"

if [ -n "$MAIN_WID" ]; then
    # Resize for consistent screenshots
    xdotool windowsize "$MAIN_WID" 1024 700 2>/dev/null || true
    xdotool windowactivate --sync "$MAIN_WID" 2>/dev/null || true
    sleep 1
    screenshot_window "$MAIN_WID" "01-main-window.png"

    # 2. New Machine Wizard screenshot
    echo "--- Opening New Machine Wizard ---"
    xdotool windowactivate --sync "$MAIN_WID" 2>/dev/null || true
    sleep 0.5
    # Open Machine menu -> New Machine (first item)
    xdotool key alt+m
    sleep 0.8
    xdotool key Return
    sleep 2

    WIZARD_WID=$(xdotool search --name "Create a new Machine" 2>/dev/null | head -1)
    if [ -z "$WIZARD_WID" ]; then
        WIZARD_WID=$(xdotool search --name "New Machine" 2>/dev/null | head -1)
    fi
    echo "Wizard window ID: $WIZARD_WID"

    if [ -n "$WIZARD_WID" ] && [ "$WIZARD_WID" != "$MAIN_WID" ]; then
        xdotool windowactivate --sync "$WIZARD_WID" 2>/dev/null || true
        sleep 0.5
        screenshot_window "$WIZARD_WID" "02-new-machine-wizard-page1.png"

        # Navigate through wizard pages
        for page_num in 2 3 4 5 6; do
            xdotool key alt+n
            sleep 1
            screenshot_window "$WIZARD_WID" "02-wizard-page-${page_num}.png"
        done

        # Close wizard
        xdotool key Escape
        sleep 1
    fi

    # 3. File menu screenshot
    echo "--- Opening File menu ---"
    xdotool windowactivate --sync "$MAIN_WID" 2>/dev/null || true
    sleep 0.5
    xdotool key alt+f
    sleep 0.8
    screenshot_desktop "03-file-menu.png"
    xdotool key Escape
    sleep 0.5

    # 4. Machine menu screenshot
    echo "--- Opening Machine menu ---"
    xdotool windowactivate --sync "$MAIN_WID" 2>/dev/null || true
    sleep 0.5
    xdotool key alt+m
    sleep 0.8
    screenshot_desktop "04-machine-menu.png"
    xdotool key Escape
    sleep 0.5

    # 5. Help -> About screenshot
    echo "--- Opening Help menu ---"
    xdotool windowactivate --sync "$MAIN_WID" 2>/dev/null || true
    sleep 0.5
    xdotool key alt+h
    sleep 0.8
    screenshot_desktop "05-help-menu.png"
    # Select About (usually last item)
    xdotool key Up
    sleep 0.3
    xdotool key Return
    sleep 1

    ABOUT_WID=$(xdotool search --name "About" 2>/dev/null | head -1)
    echo "About window ID: $ABOUT_WID"
    if [ -n "$ABOUT_WID" ]; then
        xdotool windowactivate --sync "$ABOUT_WID" 2>/dev/null || true
        sleep 0.5
        screenshot_window "$ABOUT_WID" "06-about-dialog.png"
        xdotool key Escape
        sleep 0.5
    fi

    # 6. Machine Configuration screenshots (including Cloud-init tab)
    echo "--- Creating a test machine for configuration screenshots ---"
    xdotool windowactivate --sync "$MAIN_WID" 2>/dev/null || true
    sleep 0.5

    # Open Machine menu -> New Machine
    xdotool key alt+m
    sleep 0.8
    xdotool key Return
    sleep 2

    WIZARD_WID=$(xdotool search --name "Create a new Machine" 2>/dev/null | head -1)
    if [ -z "$WIZARD_WID" ]; then
        WIZARD_WID=$(xdotool search --name "New Machine" 2>/dev/null | head -1)
    fi

    if [ -n "$WIZARD_WID" ]; then
        xdotool windowactivate --sync "$WIZARD_WID" 2>/dev/null || true
        sleep 0.5

        # Fill in minimal machine details and go through wizard
        # Page 1: Select machine type
        xdotool key alt+n
        sleep 1

        # Page 2: General page - enter name
        xdotool type --delay 50 "TestVM"
        sleep 0.5
        xdotool key alt+n
        sleep 1

        # Page 3-6: Skip through remaining pages
        for i in {1..4}; do
            xdotool key alt+n
            sleep 1
        done

        # Finish wizard
        xdotool key alt+f
        sleep 2
    fi

    # Now open the machine configuration
    echo "--- Opening Machine Configuration window ---"
    xdotool windowactivate --sync "$MAIN_WID" 2>/dev/null || true
    sleep 1

    # Right-click on the machine (assuming it's in the list)
    # Try to click on the machine list area and open settings
    xdotool key alt+m
    sleep 0.5
    xdotool key Down Down  # Navigate to Settings
    sleep 0.3
    xdotool key Return
    sleep 2

    CONFIG_WID=$(xdotool search --name "Settings" 2>/dev/null | head -1)
    if [ -z "$CONFIG_WID" ]; then
        CONFIG_WID=$(xdotool search --name "Machine Settings" 2>/dev/null | head -1)
    fi
    if [ -z "$CONFIG_WID" ]; then
        CONFIG_WID=$(xdotool search --name "Configuration" 2>/dev/null | head -1)
    fi

    echo "Configuration window ID: $CONFIG_WID"

    if [ -n "$CONFIG_WID" ] && [ "$CONFIG_WID" != "$MAIN_WID" ]; then
        xdotool windowactivate --sync "$CONFIG_WID" 2>/dev/null || true
        xdotool windowsize "$CONFIG_WID" 900 650 2>/dev/null || true
        sleep 0.5

        # Take screenshots of each configuration tab
        # The tabs are: General, Hardware, Boot, Media, Network, Audio, Accelerator, BIOS, Cloud-init
        screenshot_window "$CONFIG_WID" "08-config-general.png"

        # Navigate through tabs (Down arrow in the list widget)
        local tab_names=("hardware" "boot" "media" "network" "audio" "accelerator" "bios" "cloudinit")
        for i in "${!tab_names[@]}"; do
            xdotool key Down
            sleep 0.5
            screenshot_window "$CONFIG_WID" "08-config-${tab_names[$i]}.png"
        done

        # Special focus on Cloud-init tab - fill in some example data
        echo "--- Demonstrating Cloud-init configuration ---"
        # Cloud-init should be the last tab (8th position from General)
        # Make sure we're on Cloud-init tab
        xdotool key Home  # Go to first tab
        sleep 0.3
        for i in {1..8}; do
            xdotool key Down
            sleep 0.3
        done
        sleep 0.5

        # Enable cloud-init checkbox
        xdotool key Tab Tab  # Navigate to Enable checkbox
        xdotool key space    # Check it
        sleep 0.5

        # Fill in hostname
        xdotool key Tab
        xdotool type --delay 50 "test-vm"
        sleep 0.3

        # Fill in username
        xdotool key Tab
        xdotool type --delay 50 "ubuntu"
        sleep 0.3

        # Fill in password (will be hidden)
        xdotool key Tab
        xdotool type --delay 50 "testpass123"
        sleep 0.3

        screenshot_window "$CONFIG_WID" "09-config-cloudinit-filled.png"

        # Close configuration window
        xdotool key Escape
        sleep 1
    else
        echo "Could not open configuration window, skipping config screenshots"
    fi
else
    echo "ERROR: Could not find main window!" >&2
    # Take desktop screenshot to see what happened
    screenshot_desktop "ERROR-no-main-window.png"
fi

# 10. Final desktop overview
screenshot_desktop "10-desktop-overview.png"

# Clean up
echo "--- Cleaning up ---"
kill $APP_PID 2>/dev/null || true
wait $APP_PID 2>/dev/null || true

echo "=== Screenshot generation complete ==="
echo "Generated files:"
ls -la "$SCREENSHOT_DIR/" || echo "No screenshots directory!"
