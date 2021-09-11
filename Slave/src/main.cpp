#include <Arduino.h>
#include <main.h>
bool lState = false;
void setup() {
    Serial.begin(9600);
    pinMode(SIG_PIN, INPUT);
    pinMode(CON_SIG_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(CON_SIG_PIN), conInt, FALLING);

    Serial.println("Startup Complete...");
}

void loop() {
    if (pause) {
        lastTest = 0;
        pinMode(SIG_PIN, OUTPUT);
        digitalWrite(SIG_PIN, true);
        return;
    }
    if (!digitalRead(SIG_PIN)) {
        if (lastTest == 0) lastTest = (TEST_TIME * 1000) + millis();
        if (lastTest <= millis()) {
            noInterrupts();
            float rpm = (float) counts * 60 / (float)TEST_TIME / 4;
            counts = 0;
            interrupts();
            lastTest = (TEST_TIME * 1000) + millis();
            Serial.print("Current RPM: ");
            Serial.println(rpm);
        }
    }
}

void conInt() {
    counts++;
}

void commandEvent() {
    if (command.startsWith("pause")) {
        Serial.println("Process Paused!");
        pause = true;
    }
    if (command.startsWith("resume")) {
        Serial.println("Process resumed!");
        pause = false;
        pinMode(SIG_PIN, INPUT);
        digitalWrite(SIG_PIN, false);
    }
}

void serialEvent() {
  while (Serial.available()) {
      char symbol = Serial.read();
        Serial.write(symbol);
        if (symbol == '\n') {
            command.toLowerCase();
            commandEvent();
            command = "";
            return;
        }
        if (symbol == 0x08) {
            Serial.write(' ');
            Serial.write(0x08);
            command.remove(command.length()-1);
        } else command += symbol;
  }
}