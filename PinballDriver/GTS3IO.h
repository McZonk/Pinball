#pragma once

#include <Arduino.h>

namespace GTS3 {
	namespace IO {

		uint8_t getMasterResetPin();
	
		uint8_t getSolenoidStrobePin(const int index);
	
		uint8_t getLampStrobePin();
		uint8_t getLampClearPin();
		uint8_t getLampDataPin();
		uint8_t getLampDataStrobePin();
	
		uint8_t getDataPin(const int index);

		uint8_t getSlamPin();
		uint8_t getTiltPin();
		uint8_t getTestPin();

		uint8_t getReturnPin(const int index);

		void setup();
	
	} // namespace IO
} // namespace GTS3
