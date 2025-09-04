#include "SimpleButton.h"
#include <SPIFFS.h>
#include <WiFi.h>

// WiFi credentials
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// OSC settings
const char *oscHost = OSC_HOST;
const int oscPort = OSC_PORT;

// WLED settings (optional)
const char *wledHost = WLED_HOST;

void setup() {
  Serial.begin(9600);

  // Initialize SPIFFS to read config file
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mount failed!");
    return;
  }

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  // Load button configuration from file
  File configFile = SPIFFS.open("/buttons_config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file!");
    return;
  }

  String jsonConfig = configFile.readString();
  configFile.close();

  // Setup button system
  ButtonSystem &buttons = ButtonSystem::getInstance();

  if (!buttons.loadConfig(jsonConfig.c_str())) {
    Serial.println("Failed to load button config!");
    return;
  }

  // Configure network targets
  IPAddress targetIP;
  targetIP.fromString(oscHost);
  buttons.setOSCTarget(targetIP, oscPort);
  buttons.setWLEDHost(wledHost);

  // Initialize buttons
  buttons.init();

  Serial.println("System ready!");
}

void loop() { ButtonSystem::getInstance().update(); }
