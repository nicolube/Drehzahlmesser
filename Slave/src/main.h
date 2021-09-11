#include <Arduino.h>

#define SIG_PIN 6 

#define CON_SIG_PIN 2

#define TEST_TIME 10 // In seconds

bool pause = false;

void commandEvent();

String command = "";
uint16_t counts = 0;
unsigned long lastTest;

/**
 * Interrupt for CON_SIG_PIN
 * stores time of last signal change to @link conLastChange
 */
void conInt();