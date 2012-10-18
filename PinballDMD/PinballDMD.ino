#include <Arduino.h>
#include <SPI.h>
#include <TimerOne.h>

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
  
  Timer1.initialize(100);
  Timer1.attachInterrupt(update);
}

void loop() {
}

int displayRow = 0;

void update() {
  for(int x = 0; x < 128 / 8; ++x) {
    SPI.transfer(McZonk_bits[displayRow * (128/8) + x] ^ 0xff);
  }
    
  digitalWrite(ColLatchPin, HIGH);
  digitalWrite(ColLatchPin, LOW);

  digitalWrite(DisplayEnablePin, LOW);
    
  if(displayRow == 0) {
    digitalWrite(RowDataPin, HIGH);
  } else {
    digitalWrite(RowDataPin, LOW);
  }
    
  digitalWrite(RowClockPin, LOW);
  delayMicroseconds(1);
  digitalWrite(RowClockPin, HIGH);
    
  digitalWrite(DisplayEnablePin, HIGH);
  
  displayRow += 1;
  displayRow %= 32;
}
