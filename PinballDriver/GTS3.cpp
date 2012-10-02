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
			
			if(solenoidTimers[i] != 0) {
				if(solenoidTimers[i] > dt) {
					solenoidTimers[i] -= dt;
				} else {
					solenoidTimers[i] = 0;
				}
			}
			
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
	
	void Pinball::handleIO() {
		for(int solenoidIndex = 0; solenoidIndex < 4; ++solenoidIndex) {
			// write data
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

} // namespace GTS3

