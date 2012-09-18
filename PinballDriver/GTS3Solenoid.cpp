#include "GTS3Solenoid.h"

#include <Arduino.h>

namespace GTS3 {
	namespace Solenoid {
		
		static const prog_int16_t PROGMEM SF2SolenoidHoldTimes[32] = {
			250, // 00 Pop Bumper
			100, // 01 Left Kicking Rubber
			100, // 02 Right Kicking Rubber
			250, // 03 Bottom Left Kicker
			250, // 04 Bottom Right Kicker
			250, // 05 Left Plunger Gate
			250, // 06 Right Plunger Gate
			  0, // 07 Left Stargate Ramp
			  0, // 08 Right Stargate Ramp
			250, // 09 Left Hole
			250, // 10 Right Hole
			250, // 11
			250, // 12
			500, // 13 Car Reset
			 50, // 14 Lamp Left Top
			 50, // 15 Lamp Lightstrip
			 50, // 16 Lamp Left Bottom
			 50, // 17 Lamp Left Ramp
			 50, // 18 Lamp Left Flipper
			 50, // 19 Lamp Right Bottom
			 50, // 20 Lamp Right Center
			 50, // 21 Lamp Right Top
			 50, // 22 Lamp Right Flipper
			  0, // 23 Car Playfield Flipper Relay
			  0, // 24 Motor Relay
			  0, // 25 Lightbox Relay
			250, // 26 Coin Meter
			400, // 27 Ball Release
			250, // 28 Outhole
			250, // 29 Knocker
			  0, // 30 Tilt Relay
			  0, // 31 Game Over Relay
		};
		
		static const prog_int16_t PROGMEM SF2SolenoidCooldownTimes[32] = {
			 500, // 00 Pop Bumper
			 250, // 01 Left Kicking Rubber
			 250, // 02 Right Kicking Rubber
			 500, // 03 Bottom Left Kicker
			 500, // 04 Bottom Right Kicker
			 500, // 05 Left Plunger Gate
			 500, // 06 Right Plunger Gate
			   0, // 07 Left Stargate Ramp
			   0, // 08 Right Stargate Ramp
			 500, // 09 Left Hole
			 500, // 10 Right Hole
			 500, // 11
			 500, // 12
			 500, // 13 Car Reset
			 500, // 14 Lamp Left Top
			 500, // 15 Lamp Lightstrip
			 500, // 16 Lamp Left Bottom
			 500, // 17 Lamp Left Ramp
			 500, // 18 Lamp Left Flipper
			 500, // 19 Lamp Right Bottom
			 500, // 20 Lamp Right Center
			 500, // 21 Lamp Right Top
			 500, // 22 Lamp Right Flipper
			   0, // 23 Car Playfield Flipper Relay
			   0, // 24 Motor Relay
			   0, // 25 Lightbox Relay
			 500, // 26 Coin Meter
			2000, // 27 Ball Release
			 500, // 28 Outhole
			 500, // 29 Knocker
			   0, // 30 Tilt Relay
			   0, // 31 Game Over Relay
		};
		
		uint16_t getSolenoidHoldTime(const int index) {
			return pgm_read_word(&SF2SolenoidHoldTimes[index]);
		}
		
		uint16_t getSolenoidCooldownTime(const int index) {
			return pgm_read_word(&SF2SolenoidCooldownTimes[index]);
		}

	} // namespace Solenoid
} // namespace GTS3
