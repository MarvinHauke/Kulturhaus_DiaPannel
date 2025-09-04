# Kulturhaus DiaPannel

ESP32-based OSC button controller interface with 19 buttons for controlling Ableton Live or other OSC-compatible applications.

## Table of Contents

- [Features](#features)
- [Hardware](#hardware)
- [Installation](#installation)
  - [Prerequisites](#prerequisites)
  - [Recommended Tools](#recommended-tools)
  - [Setup](#setup)
- [OSC Protocol](#osc-protocol)
- [Python OSC-to-MIDI Bridge (Optional)](#python-osc-to-midi-bridge-optional)
- [Project Structure](#project-structure)
- [Configuration](#configuration)
  - [WiFi and OSC Settings](#wifi-and-osc-settings)
  - [Change Button Pins](#change-button-pins)
  - [Adjust Debounce Time](#adjust-debounce-time)
- [Troubleshooting](#troubleshooting)
- [Libraries](#libraries)
- [License](#license)
- [Author](#author)

## Features

- 19 hardware buttons with debouncing
- OSC messages over WiFi
- Non-blocking code for optimal performance
- Configurable network settings via `.env` file
- Button press/release events
- OSC address format: `/button/[number]` with value 0 (release) or 1 (press)

## Hardware

- **Board**: ESP32 DevKit
- **Buttons**: 19 push buttons on GPIO pins:
  - Pins: 0, 2, 3, 4, 13, 15, 16, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39
- **Debounce time**: 50ms

## Installation

### Prerequisites

- [PlatformIO](https://platformio.org/) installed
- ESP32 board support
- USB drivers for ESP32

### Recommended Tools

- **[direnv](https://direnv.net/)** - Automatic loading of environment variables from `.env`
  ```bash
  # Installation macOS
  brew install direnv
  # Installation Linux
  apt-get install direnv
  ```

- **[uv](https://github.com/astral-sh/uv)** - Fast Python package manager for the OSC-to-MIDI bridge
  ```bash
  # Installation
  curl -LsSf https://astral.sh/uv/install.sh | sh
  # Or with brew
  brew install uv
  ```

### Setup

1. Clone repository:
```bash
git clone <repository-url>
cd Kulturhaus_DiaPannel
```

2. Create `.env` file:
```env
WIFI_SSID="YourWiFiName"
WIFI_PASSWORD="YourWiFiPassword"
OSC_HOST="192.168.1.100"
OSC_PORT=8000
```

3. Activate environment variables:

**With direnv (recommended):**
```bash
# One-time setup: activate direnv for your shell
echo 'eval "$(direnv hook bash)"' >> ~/.bashrc  # for bash
echo 'eval "$(direnv hook zsh)"' >> ~/.zshrc    # for zsh

# In project directory
direnv allow
```

**Or manually:**
```bash
export WIFI_SSID="YourWiFiName"
export WIFI_PASSWORD="YourWiFiPassword"
```

4. Compile and upload project:
```bash
pio run -t upload
```

5. Serial monitor for debugging:
```bash
pio device monitor
```

## OSC Protocol

The system sends OSC messages in the format:

- **Address**: `/button/[1-19]`
- **Value**: 
  - `1` = Button pressed
  - `0` = Button released

### Examples:
- Button 1 pressed: `/button/1` with value `1`
- Button 1 released: `/button/1` with value `0`
- Button 6 pressed: `/button/6` with value `1`

## Python OSC-to-MIDI Bridge (Optional)

A Python script for converting OSC messages to MIDI CC is available:

### Installation:
```bash
# With uv (recommended)
uv pip install python-osc mido python-rtmidi

# Or with pip
pip install python-osc mido python-rtmidi
```

### Usage:
```bash
python osc_to_midi.py --ip 0.0.0.0 --port 8000
```

The script maps each button to a MIDI CC number and sends:
- CC value 127 on button press
- CC value 0 on button release

## Project Structure

```
Kulturhaus_DiaPannel/
├── src/
│   └── main.cpp          # Main program
├── .env                  # WiFi/OSC configuration (not in Git)
├── .envrc                # direnv configuration
├── platformio.ini        # PlatformIO configuration
├── osc_to_midi.py        # Optional: OSC to MIDI bridge
└── README.md            # This file
```

## Configuration

### WiFi and OSC Settings

Edit `.env`:
```env
WIFI_SSID="NewNetwork"
WIFI_PASSWORD="NewPassword"  
OSC_HOST="10.0.0.100"
OSC_PORT=8000
```

### Change Button Pins

In `src/main.cpp` adjust the `BUTTON_PINS[]` array.

### Adjust Debounce Time

In `src/main.cpp`:
```cpp
buttons[i]->setDebounceTime(50); // Time in milliseconds
```

## Troubleshooting

### "SSID too long or missing"
- Check if `.env` file exists
- Ensure environment variables are set (`echo $WIFI_SSID`)
- Run `direnv allow` or export variables manually

### No OSC messages received
- Check receiver's IP address in `.env`
- Ensure ESP32 and receiver are on the same network
- Check firewall settings

### Build errors
```bash
pio run -t clean
pio run
```

## Libraries

- [ezButton](https://github.com/ArduinoGetStarted/ezButton) - Button debouncing
- [OSC](https://github.com/CNMAT/OSC) - OSC protocol
- AsyncUDP - ESP32 UDP communication

## License

[Your license here]

## Author

[Your name]
