#include "SimpleButton.h"

ButtonSystem& ButtonSystem::getInstance() {
    static ButtonSystem instance;
    return instance;
}

ButtonSystem::~ButtonSystem() {
    for (uint8_t i = 0; i < buttonCount; i++) {
        delete ezButtons[i];
    }
}

bool ButtonSystem::loadConfig(const char* jsonConfig) {
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, jsonConfig);
    
    if (error) {
        Serial.print("JSON parse error: ");
        Serial.println(error.c_str());
        return false;
    }
    
    JsonArray buttonsArray = doc["buttons"];
    buttonCount = 0;
    
    for (JsonObject btn : buttonsArray) {
        if (buttonCount >= MAX_BUTTONS) break;
        
        ButtonConfig& config = buttons[buttonCount];
        config.id = btn["id"];
        config.pin = btn["pin"];
        config.oscAddress = btn["osc"].as<String>();
        config.wledEndpoint = btn["wled"].as<String>();
        config.potiPin = btn["poti_pin"] | -1;
        
        String modeStr = btn["mode"] | "momentary";
        config.mode = (modeStr == "toggle") ? TOGGLE : MOMENTARY;
        config.toggleState = false;
        
        ezButtons[buttonCount] = new ezButton(config.pin);
        buttonCount++;
    }
    
    Serial.printf("Loaded %d buttons from config\n", buttonCount);
    return true;
}

void ButtonSystem::init() {
    for (uint8_t i = 0; i < buttonCount; i++) {
        ezButtons[i]->setDebounceTime(50);
    }
}

void ButtonSystem::update() {
    for (uint8_t i = 0; i < buttonCount; i++) {
        ezButtons[i]->loop();
        ButtonConfig& btn = buttons[i];
        
        if (ezButtons[i]->isPressed()) {
            Serial.printf("Button %d pressed\n", btn.id);
            
            if (btn.mode == TOGGLE) {
                btn.toggleState = !btn.toggleState;
                int value = btn.toggleState ? 127 : 0;
                sendOSC(btn, value);
                sendWLED(btn, btn.toggleState ? 1 : 0);
            } else {
                int value = 1;
                if (btn.potiPin >= 0) {
                    value = map(analogRead(btn.potiPin), 0, 4095, 0, 127);
                }
                sendOSC(btn, value);
                sendWLED(btn, value);
            }
        }
        
        if (ezButtons[i]->isReleased() && btn.mode == MOMENTARY) {
            Serial.printf("Button %d released\n", btn.id);
            sendOSC(btn, 0);
            sendWLED(btn, 0);
        }
    }
}

void ButtonSystem::sendOSC(ButtonConfig& btn, int value) {
    if (btn.oscAddress.length() == 0) return;
    
    OSCMessage msg(btn.oscAddress.c_str());
    msg.add(value);
    
    AsyncUDPMessage packet;
    msg.send(packet);
    udp.sendTo(packet, oscTargetIP, oscTargetPort);
    
    Serial.printf("OSC: %s = %d\n", btn.oscAddress.c_str(), value);
}

void ButtonSystem::sendWLED(ButtonConfig& btn, int value) {
    if (wledHost.length() == 0 || btn.wledEndpoint.length() == 0) return;
    
    // Don't send WLED command on button release (value = 0)
    if (value == 0) return;
    
    HTTPClient http;
    String url = wledHost + btn.wledEndpoint;
    
    Serial.printf("WLED Request: %s\n", url.c_str());
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode == 200) {
        Serial.printf("✓ WLED Success: %s - Code: %d\n", url.c_str(), httpCode);
        String response = http.getString();
        Serial.printf("Response: %s\n", response.c_str());
    } else if (httpCode > 0) {
        Serial.printf("⚠ WLED Response: %s - Code: %d\n", url.c_str(), httpCode);
    } else {
        Serial.printf("✗ WLED Error: %s - %s\n", url.c_str(), http.errorToString(httpCode).c_str());
    }
    
    http.end();
}

void ButtonSystem::setOSCTarget(IPAddress ip, uint16_t port) {
    oscTargetIP = ip;
    oscTargetPort = port;
}

void ButtonSystem::setWLEDHost(const char* host) {
    wledHost = String(host);
}