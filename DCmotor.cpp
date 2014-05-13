/*

***** DC motor Library for Arduino IDE 1.0 *****

Author: Ming-Yuan Yu, National Taiwan University
Last Modified: 01/28/2014

Note:

1. The two pins should be connected to the inputs of the H-bridge.
    In order to make the analogWrite() function work properly, be sure
    that the first pin you specified (IN1) has PWM capability on your
    Arduino. The second pin (IN2) can be any ordinary digital pin and
    not necessarily to have PWM capability.

2. The PWM resolution is set to the default value 255. If you have
    changed the resolution to a different value, please specify the
    value while creating the object.

*/

#include "DCmotor.h"

// Constructors
DCmotor::DCmotor(uint8_t IN1, uint8_t IN2) {
    pin1 = IN1;
    pin2 = IN2;
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    PWMres = 0xFF;
    setDirection(FORWARD);
    setDuty(0);
}

DCmotor::DCmotor(uint8_t IN1, uint8_t IN2, uint16_t PWMresolution) {
    pin1 = IN1;
    pin2 = IN2;
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    PWMres = PWMresolution;
    setDirection(FORWARD);
    setDuty(0);
}

// Public Functions
uint8_t DCmotor::readDirection() {
    return dir;
}

uint16_t DCmotor::readDuty() {
    return duty;
}

void DCmotor::run(uint8_t direction, uint16_t dutyCycle) {
    setDirection(direction);
    setDuty(dutyCycle);
    switch (dir) {
    case FORWARD:
        digitalWrite(pin2, LOW);
        analogWrite(pin1, duty);
        break;
    case BACKWARD:
        digitalWrite(pin2, HIGH);
        analogWrite(pin1, PWMres - duty);
        break;
    }
}

void DCmotor::stop() {
    analogWrite(pin1, 0);
    digitalWrite(pin2, LOW);
}

void DCmotor::resume() {
    run(dir, duty);
}

// Private Functions
void DCmotor::setDirection(uint8_t direction) {
    if(direction == FORWARD || direction == BACKWARD)
        dir = direction;
    else
        dir = FORWARD;
}

void DCmotor::setDuty(uint16_t dutyCycle) {
    duty = constrain(dutyCycle, 0, PWMres);
}