# Kulturhaus DiaPannel

ESP32-basiertes OSC Button Controller Interface mit 19 Buttons für die Steuerung von Ableton Live oder anderen OSC-kompatiblen Anwendungen.

## Features

- 19 Hardware-Buttons mit Debouncing
- OSC-Nachrichten über WiFi
- Non-blocking Code für optimale Performance
- Konfigurierbare Netzwerk-Einstellungen über `.env` File
- Button Press/Release Events
- OSC-Adressformat: `/button/[nummer]` mit Wert 0 (release) oder 1 (press)

## Hardware

- **Board**: ESP32 DevKit
- **Buttons**: 19 Taster an GPIO Pins:
  - Pins: 0, 2, 3, 4, 13, 15, 16, 21, 22, 23, 25, 26, 27, 32, 33, 34, 35, 36, 39
- **Debounce-Zeit**: 50ms

## Installation

### Voraussetzungen

- [PlatformIO](https://platformio.org/) installiert
- ESP32 Board Support
- USB-Treiber für ESP32

### Empfohlene Tools

- **[direnv](https://direnv.net/)** - Automatisches Laden von Environment-Variablen aus `.env`
  ```bash
  # Installation macOS
  brew install direnv
  # Installation Linux
  apt-get install direnv
  ```

- **[uv](https://github.com/astral-sh/uv)** - Schneller Python Package Manager für die OSC-zu-MIDI Bridge
  ```bash
  # Installation
  curl -LsSf https://astral.sh/uv/install.sh | sh
  # Oder mit brew
  brew install uv
  ```

### Setup

1. Repository klonen:
```bash
git clone <repository-url>
cd Kulturhaus_DiaPannel
```

2. `.env` File erstellen:
```env
WIFI_SSID="DeinWiFiName"
WIFI_PASSWORD="DeinWiFiPasswort"
OSC_HOST="192.168.1.100"
OSC_PORT=8000
```

3. Environment-Variablen aktivieren:

**Mit direnv (empfohlen):**
```bash
# Einmalig direnv für deine Shell aktivieren
echo 'eval "$(direnv hook bash)"' >> ~/.bashrc  # für bash
echo 'eval "$(direnv hook zsh)"' >> ~/.zshrc    # für zsh

# Im Projektverzeichnis
direnv allow
```

**Oder manuell:**
```bash
export WIFI_SSID="DeinWiFiName"
export WIFI_PASSWORD="DeinWiFiPasswort"
```

4. Projekt kompilieren und hochladen:
```bash
pio run -t upload
```

5. Serial Monitor für Debugging:
```bash
pio device monitor
```

## OSC Protokoll

Das System sendet OSC-Nachrichten im Format:

- **Adresse**: `/button/[1-19]`
- **Wert**: 
  - `1` = Button gedrückt
  - `0` = Button losgelassen

### Beispiele:
- Button 1 gedrückt: `/button/1` mit Wert `1`
- Button 1 losgelassen: `/button/1` mit Wert `0`
- Button 6 gedrückt: `/button/6` mit Wert `1`

## Python OSC-zu-MIDI Bridge (Optional)

Ein Python-Script zur Konvertierung von OSC-Nachrichten zu MIDI CC ist verfügbar:

### Installation:
```bash
# Mit uv (empfohlen)
uv pip install python-osc mido python-rtmidi

# Oder mit pip
pip install python-osc mido python-rtmidi
```

### Verwendung:
```bash
python osc_to_midi.py --ip 0.0.0.0 --port 8000
```

Das Script mappt jeden Button zu einer MIDI CC-Nummer und sendet:
- CC Wert 127 bei Button-Press
- CC Wert 0 bei Button-Release

## Projektstruktur

```
Kulturhaus_DiaPannel/
├── src/
│   └── main.cpp          # Hauptprogramm
├── .env                  # WiFi/OSC Konfiguration (nicht im Git)
├── .envrc                # direnv Konfiguration
├── platformio.ini        # PlatformIO Konfiguration
├── osc_to_midi.py        # Optional: OSC zu MIDI Bridge
└── README.md            # Diese Datei
```

## Konfiguration

### WiFi und OSC Einstellungen

Editiere `.env`:
```env
WIFI_SSID="NeuesNetzwerk"
WIFI_PASSWORD="NeuesPasswort"  
OSC_HOST="10.0.0.100"
OSC_PORT=8000
```

### Button-Pins ändern

In `src/main.cpp` das Array `BUTTON_PINS[]` anpassen.

### Debounce-Zeit anpassen

In `src/main.cpp`:
```cpp
buttons[i]->setDebounceTime(50); // Zeit in Millisekunden
```

## Troubleshooting

### "SSID too long or missing"
- Prüfe ob `.env` File existiert
- Stelle sicher dass Environment-Variablen gesetzt sind (`echo $WIFI_SSID`)
- Führe `direnv allow` aus oder exportiere Variablen manuell

### Keine OSC-Nachrichten empfangen
- Prüfe IP-Adresse des Empfängers in `.env`
- Stelle sicher dass ESP32 und Empfänger im gleichen Netzwerk sind
- Prüfe Firewall-Einstellungen

### Build-Fehler
```bash
pio run -t clean
pio run
```

## Libraries

- [ezButton](https://github.com/ArduinoGetStarted/ezButton) - Button Debouncing
- [OSC](https://github.com/CNMAT/OSC) - OSC Protokoll
- AsyncUDP - ESP32 UDP Kommunikation

## Lizenz

[Deine Lizenz hier]

## Autor

[Dein Name]
