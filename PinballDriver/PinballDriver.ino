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



byte solenoidData[4]; // 32 solenoid values

const int masterResetPin = 0;

const int solenoidStrobePins[4] = {
  6,
  -1,
  -1,
  -1,
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
}

long oldMillis = 0;

bool on = false;
long onDelay = 4000;

void loop() {
  long delta = millis() - oldMillis;
  oldMillis = millis();
  
  onDelay -= delta;
  if(onDelay < 0) {
    onDelay += 4000;
    
    on = !on;
    
    solenoidData[0] = on ? (1 << 7) : (0 << 7);
  }

  
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
  
}

