#ifndef SIMPLE_BUTTON_H
#define SIMPLE_BUTTON_H

#include <Arduino.h>
#include <ezButton.h>
#include <OSCMessage.h>
#include <AsyncUDP.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

enum ButtonMode {
    MOMENTARY,
    TOGGLE
};

struct ButtonConfig {
    uint8_t id;
    uint8_t pin;
    String oscAddress;
    String wledEndpoint;
    int8_t potiPin;
    ButtonMode mode;
    bool toggleState = false;
};

class ButtonSystem {
public:
    static ButtonSystem& getInstance();
    
    bool loadConfig(const char* jsonConfig);
    void init();
    void update();
    
    void setOSCTarget(IPAddress ip, uint16_t port);
    void setWLEDHost(const char* host);
    
private:
    ButtonSystem() = default;
    ~ButtonSystem();
    
    void sendOSC(ButtonConfig& btn, int value);
    void sendWLED(ButtonConfig& btn, int value);
    
    static constexpr uint8_t MAX_BUTTONS = 20;
    ButtonConfig buttons[MAX_BUTTONS];
    ezButton* ezButtons[MAX_BUTTONS];
    uint8_t buttonCount = 0;
    
    AsyncUDP udp;
    IPAddress oscTargetIP;
    uint16_t oscTargetPort;
    String wledHost;
};

#endif