#include "GTS3.h"

#include "GTS3IO.h"
#include "GTS3Solenoid.h"

namespace GTS3 {

	void Pinball::setup() {
		IO::setup();

		digitalWrite(IO::getMasterResetPin(), HIGH);
		
		interruptCycleStep = 0;
	}

	void Pinball::update(const long dt) {
		solenoidTargetValues[31] = true; // playfield
		
		solenoidTargetValues[ 9] = switchValues[11]; // blanka
		solenoidTargetValues[10] = switchValues[12]; // vega
		
		solenoidTargetValues[ 3] = switchValues[27]; // honda
		solenoidTargetValues[ 4] = switchValues[28]; // bison

		solenoidTargetValues[28] = switchValues[17]; // outhole
		solenoidTargetValues[27] = switchValues[ 4]; // release

		solenoidTargetValues[ 1] = switchValues[ 9]; // left rubber
		solenoidTargetValues[ 2] = switchValues[10]; // right rubber
		
		solenoidTargetValues[ 0] = switchValues[ 8]; // pop bumper
		
		if(switchValues[80]) {
			solenoidTargetValues[7] = true;
		}
		if(switchValues[93]) {
			solenoidTargetValues[7] = false;
		}

//		noInterrupts();
		
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
		
//		interrupts();
	}
	
	void Pinball::handleTimerInterrupt() {
		switch(interruptCycleStep) {
			case  1 : performLampCycle(0); break;
			case  2 : performSwitchCycle(0); break;

			case  3 : performLampCycle(1); break;
			case  4 : performSwitchCycle(1); break;

			case  5 : performLampCycle(2); break;
			case  6 : performSwitchCycle(2); break;
		
			case  7 : performLampCycle(3); break;
			case  8 : performSwitchCycle(3); break;
		
			case  9 : performLampCycle(4); break;
			case 10 : performSwitchCycle(4); break;

			case 11 : performLampCycle(5); break;
			case 12 : performSwitchCycle(5); break;
		
			case 13 : performLampCycle(6); break;
			case 14 : performSwitchCycle(6); break;
		
			case 15 : performLampCycle(7); break;
			case 16 : performSwitchCycle(7); break;
		
			case 17 : performLampCycle(8); break;
			case 18 : performSwitchCycle(8); break;

			case 19 : performLampCycle(9); break;
			case 20 : performSwitchCycle(9); break;
		
			case 21 : performLampCycle(10); break;
			case 22 : performSwitchCycle(10); break;
		
			case 23 : performLampCycle(11); break;
			case 24 : performSwitchCycle(11); break;
		
			case 25 : performLampCycleEnd(); break;
				
			default : performSolenoidCycle(); break;
		}
		
		interruptCycleStep += 1;
		interruptCycleStep %= 26;
	}

	void Pinball::handleTiltInterrupt() {
		const int tiltPin = GTS3::IO::getTiltPin();
		const int value = digitalRead(tiltPin);

		tiltValue = (value == HIGH) ? false : true;
	}

	void Pinball::handleSlamInterrupt() {
		const int slamPin = GTS3::IO::getSlamPin();
		const int value = digitalRead(slamPin);

		slamValue = (value == HIGH) ? false : true;
	}
	
	void Pinball::performSolenoidCycle() {
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
	
	void Pinball::performLampCycle(const int row) {
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
		
		// shifts the matrix row
		digitalWrite(LDATA, row == 0 ? HIGH : LOW);
		digitalWrite(LSTB, HIGH);
		digitalWrite(LSTB, LOW);
		
		// fill bus
		for(int k = 0; k < 8; ++k) {
			const bool dataValue = lampValues[row * 8 + k];
			
			const int dataPin = GTS3::IO::getDataPin(k);
			digitalWrite(dataPin, dataValue ? HIGH : LOW);
		}
		digitalWrite(LDS, HIGH);
		digitalWrite(LDS, LOW);
	}
	
	void Pinball::performLampCycleEnd() {
		const int LSTB  = GTS3::IO::getLampStrobePin();
		const int LDS   = GTS3::IO::getLampDataStrobePin();

		// clear bus
		for(int k = 0; k < 8; ++k) {
			const int dataPin = GTS3::IO::getDataPin(k);
			digitalWrite(dataPin, LOW);
		}
		digitalWrite(LDS, HIGH);
		digitalWrite(LDS, LOW);
		
		// shifts the matrix row
		digitalWrite(LSTB, HIGH);
		digitalWrite(LSTB, LOW);
		
		// read remaining switches
		{
			const int testPin = GTS3::IO::getTestPin();
			const int value = digitalRead(testPin);
			
			testValue = (value == HIGH) ? false : true;
		}
	}
	
	void Pinball::performSwitchCycle(const int row) {
		// read switches
		for(int k = 0; k < 8; ++k) {
			const int returnPin = GTS3::IO::getReturnPin(k);
			const int value = digitalRead(returnPin);
			
			switchValues[row * 8 + k] = (value == HIGH) ? false : true;
		}
	}

} // namespace GTS3

