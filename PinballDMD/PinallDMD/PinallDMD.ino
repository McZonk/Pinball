#include <Arduino.h>

#define DisplayEnablePin 3
#define RowDataPin 4
#define RowClockPin 5
#define ColLatchPin 6

#define DotClockPin 51
#define DotDataPin 53

void setup() {
  pinMode(DisplayEnablePin, OUTPUT);
  pinMode(RowDataPin, OUTPUT);
  pinMode(RowClockPin, OUTPUT);
  pinMode(ColLatchPin, OUTPUT);
  pinMode(DotClockPin, OUTPUT);
  pinMode(DotDataPin, OUTPUT);
  
  digitalWrite(DisplayEnablePin, LOW);
  digitalWrite(RowDataPin, LOW);
  digitalWrite(RowClockPin, HIGH); // high
  digitalWrite(ColLatchPin, LOW);
  digitalWrite(DotClockPin, LOW);
  digitalWrite(DotDataPin, LOW);
  
  delay(1);
}

void loop() {
  int on = 0;
  for(int y = 0; y < 32; ++y) {
    delayMicroseconds(10);
    
    for(int x = 0; x < 128; ++x) {
      digitalWrite(DotDataPin, on & 1 ? HIGH : LOW);

      digitalWrite(DotClockPin, HIGH);
      digitalWrite(DotClockPin, LOW);
      
      on++;
    }
    on++;
    
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

