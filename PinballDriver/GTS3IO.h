#pragma once

#include <Arduino.h>

namespace GTS3 {
	namespace IO {

		uint8_t getMasterResetPin();
	
		uint8_t getSolenoidStrobePin(const int index);
	
		uint8_t getLampStrobePin();
		uint8_t getLampClearReturnPin();
		uint8_t getLampDataPin();
		uint8_t getLampDataStrobePin();
	
		uint8_t getDataPin(const int index);
	
		void setup();
	
	} // namespace IO
} // namespace GTS3
