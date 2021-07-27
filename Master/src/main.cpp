#include <Arduino.h>
#include <main.h>

void setup() {
    pinMode(SIG_PIN, OUTPUT);
    pinMode(REF_PWM_PIN, OUTPUT);
    pinMode(CON_PWM_PIN, OUTPUT);
    pinMode(REF_SIG_PIN, INPUT);
    pinMode(CON_SIG_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(REF_SIG_PIN), &refInt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(CON_SIG_PIN), &conInt, CHANGE);

    digitalWrite(SIG_PIN, true);
}

void loop() {
    if (refLastChange + CON_ERROR_DELAY < millis() && !error) {
        error = true;
        status = 0;
        digitalWrite(SIG_PIN, true);
        pinMode(SIG_PIN, OUTPUT);
    }
}

void resetError() {
    pinMode(SIG_PIN, INPUT);
    digitalWrite(SIG_PIN, false);
    error = false;
}

void calibrate() {
    if (delayEnd < millis()) {
        switch (status) {
        case 0:
            if (!error && digitalRead(SIG_PIN)) break;
            CAL_DELAY;
            status++;
            break;
        case 1:
            if (digitalRead(REF_SIG_PIN)) {
                refreceJump = refrece;
                status++;
            } else {
                refrece++;
                CAL_DELAY;
            }
            break;
        case 2:
            if (refLastChange + REF_FOUND_DELAY < millis() && digitalRead(REF_SIG_PIN)) {
                status++;
                upperRefrece = refrece;
                refrece = refreceJump;
            } else {
                refrece++;
            }
            CAL_DELAY;
            break;
        case 3:
            if (refLastChange + REF_FOUND_DELAY < millis() && !digitalRead(REF_SIG_PIN)) {
                status++;
                lowerRefrece = refrece;
                // Step 4
                midRefrece = ((upperRefrece-lowerRefrece) / 2) + lowerRefrece;
                analogWrite(CON_PWM_PIN, midRefrece);
                refrece = 0;
                status = 0;
                resetError();
            } else {
                refrece--;
            }
            CAL_DELAY;
            break;
        }
        analogWrite(REF_PWM_PIN, refrece);
    }
}

void refInt() {
    refLastChange = millis();
}

void conInt() {
    conLastChange = millis();
}