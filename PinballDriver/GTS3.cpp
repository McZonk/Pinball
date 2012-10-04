#include "GTS3.h"

#include "GTS3IO.h"
#include "GTS3Solenoid.h"

namespace GTS3 {

	void Pinball::setup() {
		IO::setup();

		digitalWrite(IO::getMasterResetPin(), HIGH);
	}

	void Pinball::update(const long dt) {
		noInterrupts();
		
		for(int i = 0; i < GTS3SolenoidCount; ++i) {
			// update timer
			if(solenoidTimers[i] != 0) {
				if(solenoidTimers[i] > dt) {
					solenoidTimers[i] -= dt;
				} else {
					solenoidTimers[i] = 0;
				}
			}
			
			// update solenoid
			if(solenoidTimers[i] == 0) {
				if(solenoidCurrentValues[i]) {
					uint16_t cooldown = Solenoid::getSolenoidCooldownTime(i);
					if(cooldown) {
						solenoidCurrentValues[i] = false;
						solenoidTimers[i] = cooldown;
					} else {
						solenoidCurrentValues[i] = solenoidTargetValues[i];
					}
				} else {
					if(solenoidTargetValues[i]) {
						solenoidCurrentValues[i] = true;
						solenoidTimers[i] = Solenoid::getSolenoidHoldTime(i);
					}
				}
			}
		}
		
		interrupts();
	}
	
	void Pinball::handleSolenoids() {
		for(int solenoidIndex = 0; solenoidIndex < 4; ++solenoidIndex) {
			// fill bus
			for(int dataIndex = 0; dataIndex < 8; ++dataIndex) {
				const bool dataValue = solenoidCurrentValues[solenoidIndex * 8 + dataIndex];
				
				const int dataPin = IO::getDataPin(dataIndex);
				digitalWrite(dataPin, dataValue ? HIGH : LOW);
			}
			
			// strobe pin
			const int solenoidStrobePin = IO::getSolenoidStrobePin(solenoidIndex);
			digitalWrite(solenoidStrobePin, HIGH);
			digitalWrite(solenoidStrobePin, LOW);
		}
	}

	void Pinball::handleLampsAndSwitches() {
		const int LCLR = GTS3::IO::getLampClearPin();
		
		const int LDATA = GTS3::IO::getLampDataPin();
		const int LSTB  = GTS3::IO::getLampStrobePin();
		const int LDS   = GTS3::IO::getLampDataStrobePin();
		
		// clear bus
		for(int k = 0; k < 8; ++k) {
			const int dataPin = GTS3::IO::getDataPin(k);
			digitalWrite(dataPin, LOW);
		}
		digitalWrite(LDS, HIGH);
		digitalWrite(LDS, LOW);

		for(int i = 0; i < 12; ++i) {
			digitalWrite(LDATA, i == 0 ? HIGH : LOW);

			// shifts the matrix row by one
			digitalWrite(LSTB, HIGH);
			digitalWrite(LSTB, LOW);

			// fill bus
			for(int k = 0; k < 8; ++k) {
				const bool dataValue = lampValues[i * 8 + k];
				
				const int dataPin = GTS3::IO::getDataPin(k);
				digitalWrite(dataPin, dataValue ? HIGH : LOW);
			}

			// strobe lamp data
			digitalWrite(LDS, HIGH);
			digitalWrite(LDS, LOW);

			delayMicroseconds(500);
			
			// read switches
			for(int k = 0; k < 8; ++k) {
				const int returnPin = GTS3::IO::getReturnPin(k);
				const int value = digitalRead(returnPin);
				
				if(switchValues[i * 8 + k] != ((value == HIGH) ? false : true)) {
					Serial.print("CHANGED ");
					Serial.print(i, HEX);
					Serial.print(k, DEC);
					Serial.print(" ");
					Serial.print(i * 8 + k, DEC);
					Serial.print(" ");
					Serial.print(value ? false : true, DEC);
					Serial.println("");
				}
				
				switchValues[i * 8 + k] = (value == HIGH) ? false : true;
			}
			
			delayMicroseconds(500);

			// clear bus
			for(int k = 0; k < 8; ++k) {
				const int dataPin = GTS3::IO::getDataPin(k);
				digitalWrite(dataPin, LOW);
			}
			digitalWrite(LDS, HIGH);
			digitalWrite(LDS, LOW);
		}

		
		// shifts the matrix row, all rows are now 0
		digitalWrite(LSTB, HIGH);
		digitalWrite(LSTB, LOW);
		
		{
			const int slamPin = GTS3::IO::getSlamPin();
			const int value = digitalRead(slamPin);
			
			if(slamValue != ((value == HIGH) ? false : true)) {
				Serial.print("SLAM ");
				Serial.print(value ? false : true, DEC);
				Serial.println("");
			}
			
			slamValue = (value == HIGH) ? false : true;
		}
		
		{
			const int tiltPin = GTS3::IO::getTiltPin();
			const int value = digitalRead(tiltPin);
			
			if(tiltValue != ((value == HIGH) ? false : true)) {
				Serial.print("TILT ");
				Serial.print(value ? false : true, DEC);
				Serial.println("");
			}
			
			tiltValue = (value == HIGH) ? false : true;
		}
		
		{
			const int testPin = GTS3::IO::getTestPin();
			const int value = digitalRead(testPin);
			
			if(testValue != ((value == HIGH) ? false : true)) {
				Serial.print("TEST ");
				Serial.print(value ? false : true, DEC);
				Serial.println("");
			}
			
			testValue = (value == HIGH) ? false : true;
		}
	}

} // namespace GTS3

