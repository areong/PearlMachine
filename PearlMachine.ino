/*
The circuit:

GND: LCD2004 (R/W)               Vin: 3.7 to 5.5 volts
  0: Limit switch 1             AGND:
  1: Limit switch 2             3.3V: 100 mA max.
  2: T.C. (CS)                    23:
  3: DC motor 1, stir, (IN1)      22:
  4: DC motor 2, level, (IN1)     21:
  5: DC motor 1, stir, (IN2)      20:
  6: DC motor 2, level, (IN2)     19: LCD2004 (D7)
  7: Button 1, Enter              18: LCD2004 (D6)
  8: Button 2, Pause/Resume       17: LCD2004 (D5)
  9: Button 3, Cancel             16: LCD2004 (D4)
 10: microSD (CS)                 15: LCD2004 (Enable)
 11: microSD & T.C. (MOSI)        14: LCD2004 (RS)
 12: microSD & T.C. (MISO)        13: microSD & T.C. (SCK)

LCD2004:
* 10K resistor:
* ends to +5V and ground
* wiper to LCD VO pin (pin 3)

Thermalcouple & SD card attached to SPI bus
*/

// include the library code:
#include <LiquidCrystal.h>
#include <SD.h>
#include "DCmotor.h"
#include "Adafruit_MAX31855.h"

// LCD2004 display
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);
// microSD card:
const uint8_t microSD_CS = 10, thermal_CS = 2;
File scheduleFile;
const int row = 32; const int column = 8;
int schedule[row][column] = {0};
// Initialize the DC motors:
DCmotor stir(3, 5, 1023);
DCmotor level(4, 6 ,1023);
// Initialize the thermocouple
Adafruit_MAX31855 thermocouple(13, 2, 12);
// Create an IntervalTimer object to use timer interrupt
IntervalTimer refresh;
// Button state
volatile byte buttonState = B0000;

void setup() {
    // set up the LCD's number of columns and rows: 
    lcd.begin(20, 4);
    // Print a message to the LCD.
    lcd.setCursor(0, 0); lcd.print("Booting...");
    // Set PWM resolution to 10-bit, frequency to 23437 Hz (Ultrasonic)
    analogWriteResolution(10); // analogWrite value 0 to 1023
    analogWriteFrequency(4, 23437); // Teensy 3.1 pin 3 also changes to 23437 Hz
    // Initialize the COM port for debugging...
    Serial.begin(115200);
    // External interrupts
    attachInterrupt(0, reverseMotorISR, RISING); // Limit switch 1
    attachInterrupt(1, reverseMotorISR, RISING); // Limit switch 2
    attachInterrupt(7, enterISR, RISING); // Button 1 (Enter)
    attachInterrupt(8, pauseISR, RISING); // Button 2 (Pause/Resume)
    attachInterrupt(9, cancelISR, RISING); // Button 3 (Cancel)

    lcd.clear(); lcd.setCursor(0, 0);
    lcd.print("Initializing SD card...");
    pinMode(microSD_CS, OUTPUT);
    if (!SD.begin(microSD_CS)) {
        lcd.clear(); lcd.setCursor(0, 0);
        lcd.print("Card failed, or not present");
        // don't do anything more:
        return;
    }
    lcd.clear(); lcd.setCursor(0, 0); 
    lcd.print("Card initialized.");
    // 從這裡開始有問題..............................................................
    scheduleFile = SD.open("schedule.txt");
    if(scheduleFile) {
        char scheduleTemp = scheduleFile.read();
        // 讀SD卡到底該怎麼做 Q_Q
        /*
        for(int i = 0; i < row; i++){
          for(int j = 0; j , column; j++){
            schedule[i][j] = atoi(scheduleTemp[i*column+j+1]);
          }
        }
        Serial.print(schedule);
        */
        Serial.print(scheduleTemp);
        Serial.println("=~=");
        Serial.write(scheduleTemp);
    }
    scheduleFile.close();
    // Q_Q...........................................................................

    // Start calling the ISR, every 0.5 second
    refresh.begin(refreshInfoISR, 500000);
}

void loop() {
}

void refreshInfoISR() {
    double c = thermocouple.readCelsius();
    if (isnan(c));
    else {
        lcd.setCursor(0, 0); 
        lcd.print(c);
    }
}
void reverseMotorISR() {
    uint8_t newDirection = stir.readDirection() ^ 0x01;
    stir.run(stir.readDuty(), newDirection);
}

void enterISR() {
    buttonState |= B0001;
}

void pauseISR() {
    buttonState |= B0010;
}

void cancelISR() {
    buttonState |= B0100;
}
