#include <Arduino.h>

/*
  A1P2
  
  01 LSTB Lamp Strobe
  02 LDATA Lamp Data
  03 LCLR Lamp Clear Return
  04 BD0 Solenoid Data 7
  05 BD0 Solenoid Data 6
  06 BD0 Solenoid Data 5
  07 BD0 Solenoid Data 4
  08 BD0 Solenoid Data 3
  09 BD0 Solenoid Data 2
  10 BD0 Solenoid Data 1
  11 BD0 Solenoid Data 0
  12 Master Reset
  13 SS0 Solenoid Strobe 0
  14 SS1 Solenoid Strobe 1
  15 SS2 Solenoid Strobe 2
  16 SS3 Solenoid Strobe 3
  17 LDS Lamp Data Strobe
  18 - Ununsed
  19 5V
  20 Ground
*/

/*
  SF2 Solenoids
  00  0 0  0  Pop Bumber
  01  0 1  0  Left Kicking Rubber
  02  0 2  0  Right Kicking Rubber
  03  0 3  0  Bottom Left Kicker
  04  0 4  0  Bottom Right Kicker
  05  0 5  0  Left Plunger Gate
  06  0 6  0  Right Plunger Gate
  07  0 7  1  Left Stargate Ramp
  08  1 0  1  Right Stargate Ramp
  09  1 1  0  Left Hole
  10  1 2  0  Right Hole
  11  1 3  0  - Unused
  12  1 4  0  - Unused
  13  1 5  0  Car Reset
  14  1 6  0  Lamp Left Tap 
  15  1 7  0  Lamp Lightstrip
  16  2 0  0  Lamp Left Bottom
  17  2 1  0  Lamp Ramp
  18  2 2  0  Lamp Left Flipper
  19  2 3  0  Lamp Right Bottom
  20  2 4  0  Lamp Right Center
  21  2 5  0  Lamp Right Top
  22  2 6  0  Lamp Right Flipper
  23  2 7  1  Low Playfield Flipper Relay
  24  3 0  1  Motor Relay
  25  3 1  1  Lightbox Relay 
  26  3 2  0  Coin Meter
  27  3 3  0  Ball Release
  28  3 4  0  Outhole
  29  3 5  0  Knocker
  30  3 6  1  Tilt Relay
  31  3 7  1  Game Over Relay
*/

// 0, 1, 2, 3, 5, 6, 7, 8, 9

// 0 Master Reset

// 1 Solenoid Data 0
// 2 Solenoid Data 1
// 3 Solenoid Data 2
// 5 Solenoid Data 3

// 6 Solenoid Strobe 0
// 7 Solenoid Strobe 1
// 8 Solenoid Strobe 2
// 9 Solenoid Strobe 3



byte solenoidData[4] = {
  0x00, 0x00, 0x00, 0x00
}; // 32 solenoid values

// after each run the solenoidData & solenoidReset
// this will ensure that solenoids are not triggered for a long time
byte solenoidHold[4] = {
  0x80, 0x01, 0x80, 0xC3
};

const int masterResetPin = 0;

const int solenoidStrobePins[4] = {
  -1,
  -1,
  -1,
  6,
};

const int solenoidDataPins[8] = {
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  -1,
  5,
};

// The left stargate ramp is solenoid 07

void registerOutputPins(const int* pins, const int pinCount) {
  for(int i = 0; i < pinCount; ++i) {
    int pin = *pins;
    if(pin >= 0) {
      pinMode(pin, OUTPUT);
    }
    pins++;
  }
}

void setup() {
  registerOutputPins(&masterResetPin, 1);
  registerOutputPins(solenoidStrobePins, 4);
  registerOutputPins(solenoidDataPins, 8);
  
  digitalWrite(masterResetPin, HIGH);
  delay(2000);
  
  solenoidData[3] = 0x80;
}

void loop() {
  for(int solenoidStrobeIndex = 0; solenoidStrobeIndex < 4; ++solenoidStrobeIndex) {    
    const byte solenoidByte = solenoidData[solenoidStrobeIndex];
    
    // write data
    for(int solenoidDataIndex = 0; solenoidDataIndex < 8; ++solenoidDataIndex) {

      const byte solenoidDataValue = ((solenoidByte >> solenoidDataIndex) & 1) ? HIGH : LOW;
      
      const int solenoidDataPin = solenoidDataPins[solenoidDataIndex];
      if(solenoidDataPin >= 0) {
        digitalWrite(solenoidDataPin, solenoidDataValue);
      }
//      Serial.print(solenoidDataIndex, DEC);
//      Serial.print(" ");
//      Serial.print(solenoidDataPin, DEC);
//      Serial.print(" ");
//      Serial.println(solenoidDataValue, DEC);
    }
    
    // strobe pin
    const int solenoidStrobePin = solenoidStrobePins[solenoidStrobeIndex];
    if(solenoidStrobePin >= 0) {
      delay(1);
      digitalWrite(solenoidStrobePin, HIGH);
      delay(1);
      digitalWrite(solenoidStrobePin, LOW);
    }
    
//    Serial.print("STROBE: ");
//    Serial.print(solenoidStrobeIndex, DEC);
//    Serial.print(" ");
//    Serial.println(solenoidStrobePin, DEC);
//    delay(3000);
  }
  
  for(int solenoidIndex = 0; solenoidIndex < 4; ++solenoidIndex) {
    solenoidData[solenoidIndex] = solenoidData[solenoidIndex] & solenoidHold[solenoidIndex];
  }
}

