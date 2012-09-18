#include "GTS3IO.h"

#include <Arduino.h>

namespace GTS3 {
	namespace IO {

		uint8_t getMasterResetPin() {
			return 22;
		}
	
		uint8_t getSolenoidStrobePin(const int index) {
			return 30 + index;
		}
	
		uint8_t getLampStrobePin() {
			return 24;
		}

		uint8_t getLampClearPin() {
			return 25;
		}
	
		uint8_t getLampDataPin() {
			return 26;
		}

		uint8_t getLampDataStrobePin() {
			return 27;
		}
	
		uint8_t getDataPin(const int index) {
			return 34 + index;
		}
	
		void setup() {
			pinMode(getMasterResetPin(), OUTPUT);

			for(int i = 0; i < 4; ++i) {
				pinMode(getSolenoidStrobePin(i), OUTPUT);
			}
		
			pinMode(getLampStrobePin(), OUTPUT);
			pinMode(getLampClearPin(), OUTPUT);
			pinMode(getLampDataPin(), OUTPUT);
			pinMode(getLampDataStrobePin(), OUTPUT);
			
			for(int i = 0; i < 8; ++i) {
				pinMode(getDataPin(i), OUTPUT);
			}
		}
	} // namespace IO
} // namespaee GTS3
