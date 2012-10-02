#include <Arduino.h>

/*
 A1P2 / A3P2
 
 01 LSTB Lamp Strobe
 02 LDATA Lamp Data
 03 LCLR Lamp Clear Return
 04 BD7 Solenoid Data 7
 05 BD6 Solenoid Data 6
 06 BD5 Solenoid Data 5
 07 BD4 Solenoid Data 4
 08 BD3 Solenoid Data 3
 09 BD2 Solenoid Data 2
 10 BD1 Solenoid Data 1
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

#define GTS3SolenoidCount 32

#define GTS3LampCount 96
#define GTS3_SWITCH_COUNT 96

namespace GTS3 {

	class Pinball {
	public:
		void setup();
		
		void update(const long dt);
		
		// this message should be called from an interrupt later
		void handleIO();
		
	public:
		bool solenoidTargetValues[GTS3SolenoidCount];
		bool solenoidCurrentValues[GTS3SolenoidCount];
		uint16_t solenoidTimers[GTS3SolenoidCount];

		bool lampValues[GTS3LampCount];
	};

} // namespace GTS3
