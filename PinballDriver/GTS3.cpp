//
//  GottliebSystem3.cpp
//  PinballDriver
//
//  Created by Maximilian Christ on 2012-09-10.
//
//

#include "GTS3.h"

#include "GTS3IO.h"
#include "GTS3Solenoid.h"

namespace GTS3
{
	void Pinball::setup()
	{
		IO::setup();

		digitalWrite(IO::getMasterResetPin(), HIGH);
	}

	void Pinball::update(const long dt)
	{
		noInterrupts();
		
		for(int i = 0; i < GTS3SolenoidCount; ++i)
		{
			if(solenoidTimers[i] != 0)
			{
				if(solenoidTimers[i] > dt)
				{
					solenoidTimers[i] -= dt;
				}
				else
				{
					solenoidTimers[i] = 0;
				}
			}
			
			if(solenoidTimers[i] == 0)
			{
				if(solenoidCurrentValues[i])
				{
					uint16_t cooldown = Solenoid::getSolenoidCooldownTime(i);
					if(cooldown)
					{
						solenoidCurrentValues[i] = false;
						solenoidTimers[i] = cooldown;
					}
					else
					{
						solenoidCurrentValues[i] = solenoidTargetValues[i];
					}
				}
				else
				{
					if(solenoidTargetValues[i])
					{
						solenoidCurrentValues[i] = true;
						solenoidTimers[i] = Solenoid::getSolenoidHoldTime(i);
					}
				}
			}
		}
		
		interrupts();
	}
	
	void Pinball::handleIO()
	{
		for(int solenoidIndex = 0; solenoidIndex < 4; ++solenoidIndex)
		{
			// write data
			for(int dataIndex = 0; dataIndex < 8; ++dataIndex)
			{
				const bool dataValue = solenoidCurrentValues[solenoidIndex * 8 + dataIndex];
				
				const int dataPin = IO::getDataPin(dataIndex);
				digitalWrite(dataPin, dataValue ? HIGH : LOW);

                                if(dataValue) {
                                  //Serial.print("Solenoid: ");
                                  //Serial.println(solenoidIndex * 8 + dataIndex, DEC);
                                }
#if 0
				Serial.print(solenoidIndex, DEC);
				Serial.print(" ");
				Serial.print(dataIndex, DEC);
				Serial.print(" ");
				Serial.print(dataValue, DEC);
				Serial.print(" ");
				Serial.println(dataPin, DEC);
#endif
			}
			
			// strobe pin
			const int solenoidStrobePin = IO::getSolenoidStrobePin(solenoidIndex);
			//delay(1);
			digitalWrite(solenoidStrobePin, HIGH);
			//9delay(1);
			digitalWrite(solenoidStrobePin, LOW);
			
#if 0
			Serial.print("STROBE: ");
			Serial.print(solenoidIndex, DEC);
			Serial.print(" ");
			Serial.println(solenoidStrobePin, DEC);
			delay(1000);
#endif
		}
	}

	
} // namespace GTS3

