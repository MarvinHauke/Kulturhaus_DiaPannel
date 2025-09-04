#ifndef SIMPLE_BUTTON_H
#define SIMPLE_BUTTON_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncUDP.h>
#include <HTTPClient.h>
#include <OSCMessage.h>
#include <ezButton.h>

enum ButtonMode { MOMENTARY, TOGGLE };

// Define the following for every Button in /data/buttons_config.json
struct ButtonConfig {
  uint8_t id;
  uint8_t pin;
  uint8_t presetNumber;
  String wledAttribute;
  String oscAddress;
  String wledEndpoint;
  int8_t potiPin;
  ButtonMode mode;
  bool toggleState = false;
};

class ButtonSystem {
public:
  static ButtonSystem &getInstance();

  bool loadConfig(const char *jsonConfig);
  void init();
  void update();

  void setOSCTarget(IPAddress ip, uint16_t port);
  void setWLEDHost(const char *host);

private:
  ButtonSystem() = default;
  ~ButtonSystem();

  void sendOSC(ButtonConfig &btn, int value);
  void sendWLED(ButtonConfig &btn, int value);

  static constexpr uint8_t MAX_BUTTONS = 20;
  ButtonConfig buttons[MAX_BUTTONS];
  ezButton *ezButtons[MAX_BUTTONS];
  uint8_t buttonCount = 0;

  AsyncUDP udp;
  IPAddress oscTargetIP;
  uint16_t oscTargetPort;
  String wledHost;
};

#endif
