#include <Arduino.h>
#include <SPI.h>

#define DisplayEnablePin 43
#define RowDataPin 45
#define RowClockPin 47
#define ColLatchPin 49

#define DotClockPin 51 // MISO
#define DotDataPin 52 // SCLK
#define SlaveSelectPin 53

#include "McZonk.h"

void setup() {
  SPI.begin();
  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE3);

  pinMode(DisplayEnablePin, OUTPUT);
  pinMode(RowDataPin, OUTPUT);
  pinMode(RowClockPin, OUTPUT);
  pinMode(ColLatchPin, OUTPUT);
  pinMode(DotClockPin, OUTPUT);
  pinMode(DotDataPin, OUTPUT);
  pinMode(SlaveSelectPin, OUTPUT);

  digitalWrite(DisplayEnablePin, LOW);
  digitalWrite(RowDataPin, LOW);
  digitalWrite(RowClockPin, HIGH); // high
  digitalWrite(ColLatchPin, LOW);
  digitalWrite(DotClockPin, LOW);
  digitalWrite(DotDataPin, LOW);
  digitalWrite(SlaveSelectPin, LOW);
  
  delay(1);
}

void loop() {
  int index = 0;

  for(int y = 0; y < 32; ++y) {
    delayMicroseconds(100);

    for(int x = 0; x < 128 / 8; ++x) {
      SPI.transfer(McZonk_bits[index] ^ 0xff);
      ++index;
    }
    
    digitalWrite(ColLatchPin, HIGH);
    digitalWrite(ColLatchPin, LOW);

    digitalWrite(DisplayEnablePin, LOW);
    
    if(y == 0) {
      digitalWrite(RowDataPin, HIGH);
    } else {
      digitalWrite(RowDataPin, LOW);
    }
    
    digitalWrite(RowClockPin, LOW);
    delayMicroseconds(1);
    digitalWrite(RowClockPin, HIGH);
    
    digitalWrite(DisplayEnablePin, HIGH);
  }
}

