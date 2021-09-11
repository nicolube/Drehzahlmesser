#include <Arduino.h>

#define SIG_PIN 19

#define REF_PWM_PIN 6
#define CON_PWM_PIN 5

#define REF_SIG_PIN 3
#define CON_SIG_PIN 2

#define REF_FOUND_DELAY 200
#define REF_AJUST_DELAY 10

#define CON_ERROR_DELAY 300

// calibrate vars
uint8_t status = 0;
uint8_t refrece = 0;
uint8_t refreceJump = 0;
uint8_t upperRefrece = 0;
uint8_t midRefrece = 0;
uint8_t lowerRefrece = 0;
unsigned long delayEnd = 0;
#define CAL_DELAY delayEnd = REF_AJUST_DELAY + millis()

unsigned long conLastChange = 0;
unsigned long refLastChange = 0;

bool error = true;

/**
 * Interrupt for REF_SIG_PIN
 * stores time of last signal change to @link conLastChange
 */
void refInt();

/**
 * Interrupt for CON_SIG_PIN
 * stores time of last signal change to @link conLastChange
 */
void conInt();

/**
 * Runns calliration cycle.
 * 0) Init delay
 * 1) Search signal
 * 2) Find upper edge of signal
 * 3) Find lower edge of signal
 * 4) Calculate avridge and set that to @link CON_SIG_PIN
 *    and reset values
 */
void calibrate();