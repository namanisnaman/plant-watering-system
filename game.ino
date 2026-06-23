#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define MOISTURE_PIN 34
#define PUMP_PIN 25

bool autoMode = true;
int threshold = 40;

void setup() {
  Serial.begin(115200);

  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  SerialBT.begin("PlantWateringESP32");
}

int getMoisture() {
  int raw = analogRead(MOISTURE_PIN);

  int wet = 1200;
  int dry = 3200;

  return constrain(map(raw, dry, wet, 0, 100), 0, 100);
}

void loop() {

  if (SerialBT.available()) {

    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();

    if (cmd == "PUMP_ON") {
      digitalWrite(PUMP_PIN, HIGH);
    }

    if (cmd == "PUMP_OFF") {
      digitalWrite(PUMP_PIN, LOW);
    }

    if (cmd == "AUTO_ON") {
      autoMode = true;
    }

    if (cmd == "AUTO_OFF") {
      autoMode = false;
    }

    if (cmd == "MOISTURE") {
      SerialBT.println(getMoisture());
    }
  }

  if (autoMode && getMoisture() < threshold) {

    digitalWrite(PUMP_PIN, HIGH);
    delay(5000);
    digitalWrite(PUMP_PIN, LOW);

    delay(10000);
  }

  delay(200);
}