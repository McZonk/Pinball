#pragma once

#include <Arduino.h>

namespace GTS3 {
	namespace Solenoid {
		
		uint16_t getSolenoidHoldTime(const int index);
		
		uint16_t getSolenoidCooldownTime(const int index);
		
	} // namespace Solenoid
} // namespace GTS3
