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
		
		uint8_t getSlamPin() {
			return 2;
		}
		
		uint8_t getTiltPin() {
			return 3;
		}
		
		uint8_t getTestPin() {
			return 4;
		}
		
		uint8_t getReturnPin(const int index) {
			return 42 + index;
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
			
			pinMode(getSlamPin(), INPUT);
			pinMode(getTiltPin(), INPUT);
			pinMode(getTestPin(), INPUT);

			for(int i = 0; i < 8; ++i) {
				pinMode(getReturnPin(i), INPUT);
			}
		}
	} // namespace IO
} // namespaee GTS3
