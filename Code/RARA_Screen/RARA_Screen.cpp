/* Implementation of RARA_Screen class

 * Devry Senior Project
 @group Team B
 @project RARA Module
 @author Rodrick Bloomfield 
 @version 1.1 08/26/17
*************************************************************/
#include "RARA_Screen.h"

/*Class constructor
**********************************************************************************/
RARA_Screen::RARA_Screen() {}

/*Establish serial connections and exchanges information with the screen. Allows the use of multiple
 * models of TFT screen driver chips.
 *
 @param: none
 @return: none
 * 
 * Use in the setup fucntion of main file to initialize screen.
**********************************************************************************/
void RARA_Screen::initScreen() {

	Serial.begin(115200);                 //Establish Arduino serial communications for testing purposes.
	//Serial.println(F("TFT LCD test"));  //Comms established test print line.

//Identify whether direct shield connection or breakout pins are being used.
#ifdef USE_ADAFRUIT_SHIELD_PINOUT
	//Serial.println(F("Using Adafruit 2.4\" TFT Arduino Shield Pinout"));
#else
	//Serial.println(F("Using Adafruit 2.4\" TFT Breakout Board Pinout"));
#endif

	// Print lines for screen parameters. Tests for proper commmunication. Helps identify wiring issues.
	//Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());
	tft.reset();
	uint16_t identifier = tft.readID();   //Chip ID information from screen

	if (identifier == 0x9325) {
		//Serial.println(F("Found ILI9325 LCD driver"));
	}
	else if (identifier == 0x9328) {
		//Serial.println(F("Found ILI9328 LCD driver"));
	}
	else if (identifier == 0x4535) {
		//Serial.println(F("Found LGDP4535 LCD driver"));
	}
	else if (identifier == 0x7575) {
		//Serial.println(F("Found HX8347G LCD driver"));
	}
	else if (identifier == 0x9341) {
		//Serial.println(F("Found ILI9341 LCD driver"));
	}
	else if (identifier == 0x8357) {
		//Serial.println(F("Found HX8357D LCD driver"));
	}
	else if (identifier == 0x0101) {
		identifier = 0x9341;
		//Serial.println(F("Found 0x9341 LCD driver"));
	}
	else {
		Serial.print(F("Unknown LCD driver chip: "));
		Serial.println(identifier, HEX);
		Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
		Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
		Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
		Serial.println(F("If using the breakout board, it should NOT be #defined!"));
		Serial.println(F("Also if using the breakout, double-check that all wiring"));
		Serial.println(F("matches the tutorial."));
		identifier = 0x9341;
	}

	tft.begin(identifier);       // Delivers initialization codes to the screen based on control chip.    
	tft.setRotation(1);          // Set screen position to landscape.
}

void RARA_Screen::drawBatteryIndicator(uint16_t letterColor, uint16_t bColor) {
	tft.drawRect(283, 4, 6, 10, letterColor);
	tft.drawRect(288, 2, 30, 15, letterColor);
}

void RARA_Screen::batteryIndicatorBars(uint8_t bar, uint16_t letterColor, uint16_t bColor) {
	switch (bar){
		case 4:
			//Serial.println("4 bars");
			for (int i = 0; i < 4; i++){
				tft.fillRect(290 + ( i * 7) , 4, 4, 11, letterColor);
			}
			break;
		case 3:
			//Serial.println("3 bars");
			for (int i = 0; i < 3; i++){
				tft.fillRect(290 + ( i * 7) , 4, 4, 11, letterColor);
			}
			for (int i = 3; i < 4; i++){
				tft.fillRect(290 + ( i * 7) , 4, 4, 11, bColor);
			}
			break;
		case 2:
			//Serial.println("2 bars");
			for (int i = 0; i < 2; i++){
				tft.fillRect(290 + ( i * 7) , 4, 4, 11, letterColor);
			}
			for (int i = 2; i < 4; i++){
				tft.fillRect(290 + ( i * 7) , 4, 4, 11, bColor);
			}
			break;
		case 1:
			//Serial.println("1 bars");
			for (int i = 0; i < 1; i++){
				tft.fillRect(290 + ( i * 7) , 4, 4, 11, YELLOW);
			}
			for (int i = 1; i < 4; i++){
				tft.fillRect(290 + ( i * 7) , 4, 4, 11, bColor);
			}
			break;
		case 0:
			//Serial.println("no bars");
			tft.drawRect(283, 4, 6, 10, RED);
			tft.drawRect(288, 2, 30, 15, RED);
			break;	
		default:
			//Serial.println("fault");
			break;
	}
}

/* CLass destructor
**********************************************************************************/
RARA_Screen::~RARA_Screen() {}


