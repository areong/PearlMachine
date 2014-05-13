#ifndef DCmotor_H
#define DCmotor_H

#define FORWARD 0x00
#define BACKWARD 0x01

#include "Arduino.h"
#include <inttypes.h>

class DCmotor { 
public:
    // Constructors
    DCmotor(uint8_t IN1, uint8_t IN2);
    DCmotor(uint8_t IN1, uint8_t IN2, uint16_t PWMres);
    // Public Functions
    uint8_t readDirection();
    uint16_t readDuty();
    void run(uint8_t direction, uint16_t dutyCycle);
    void stop();
    void resume();
private:
    uint8_t pin1;
    uint8_t pin2;
    uint8_t dir;
    uint16_t duty;
    uint16_t PWMres;
    void setDirection(uint8_t direction);
    void setDuty(uint16_t dutyCycle);
};

#endif
