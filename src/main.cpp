#include <AsyncUDP.h>
#include <OSCMessage.h>
#include <WiFi.h>
#include <ezButton.h>

// WiFi credentials
const char *ssid = "FreeAllAntifas";
const char *password = "acab1312";

// OSC settings
const char *oscHost = "192.168.1.209"; // Target IP address
const int oscPort = 8000;              // Target port

// Pin definitions
const uint8_t BUTTON_PINS[] = {0,  2,  3,  4,  13, 15, 16, 21, 22, 23,
                               25, 26, 27, 32, 33, 34, 35, 36, 39};
const uint8_t NUM_BUTTONS = sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]);

// Button array
ezButton *buttons[NUM_BUTTONS];

AsyncUDP udp;
IPAddress targetIP;

void setup() {
  Serial.begin(9600);

  // Initialize buttons
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    buttons[i] = new ezButton(BUTTON_PINS[i]);
    buttons[i]->setDebounceTime(50);
  }

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());

  // Parse target IP
  targetIP.fromString(oscHost);
  Serial.println("OSC ready");
}

void sendOSC(int buttonNum, int value) {
  // Adresse dynamisch zusammensetzen: /button/[i]
  char address[32];
  snprintf(address, sizeof(address), "/button/%d",
           buttonNum); // snprintf used for non dynamic heap allocation

  OSCMessage msg(address);
  msg.add(value); // only appened the value

  AsyncUDPMessage packet;
  msg.send(packet);
  udp.sendTo(packet, targetIP, oscPort);

  Serial.printf("OSC: %s %d\n", address, value);
}

void loop() {
  // Update all buttons
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    buttons[i]->loop();

    if (buttons[i]->isPressed()) {
      Serial.printf("Button %d pressed\n", i + 1);
      sendOSC(i + 1, 1); // Send press event
    }

    if (buttons[i]->isReleased()) {
      Serial.printf("Button %d released\n", i + 1);
      sendOSC(i + 1, 0); // Send release event
    }
  }
}
