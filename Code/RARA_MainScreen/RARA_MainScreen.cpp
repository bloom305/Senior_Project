/* Implemetation of RARA_MainScreen class.

 * Devry Senior Project
 @group Team B
 @project RARA Module
 @author Rodrick Bloomfield 
 @version 1.1 08/26/17
*************************************************************/
#include "RARA_MainScreen.h"

/*Class constructor
***************************************************************************/
RARA_MainScreen::RARA_MainScreen(){}

/*Gathers and handles the user input.
 *
 @param: none
 @return: Screeen selector enumeration value based on user input.
 *
 * - Use in the loop to poll screen for user interaction.
******************************************************************************/
ScreenSelector RARA_MainScreen::touchResponse() {

	digitalWrite(52, HIGH);                       // Pulse to the SPI clock of screen to get touch data.
	TSPoint p = ts.getPoint();                    // Instance of Pont class to hold x,y,z, data from screen query.
	digitalWrite(52, LOW);                        // Return pin to LOW.

												  // If sharing pins, the direction of the touchscreen pins must be fixed.
	pinMode(XM, OUTPUT);
	pinMode(YP, OUTPUT);

	/* Manage point information in relation to current screen configuration.
	* If z data within pressure settings, data will be mapped.
	*/
	if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
		TSPoint temp;                               //Temporary point holder to shift axis element, due to orientation.
													/* Normally x-axis is mapped with width, however, that is the shorter dimension of the screen. Since the
													* screen is oriented in landscape, the height is used in mapping x-axis and vice versa for the y-axis.
													*/
		temp.x = (tft.height() - map(p.x, TS_MINX, TS_MAXX, 0, tft.height()));  // Mapping x-axis position of input.
		temp.y = map(p.y, TS_MINY, TS_MAXY, tft.width(), 0);                    // Mapping y-axis position of input.
		p.x = temp.y;                               //Shifting values due to screen orientation
		p.y = temp.x;
	}

	//Poll the buttons to see which one is pressed and change the state accordingly.
	for (uint8_t poll = 0; poll < 4; poll++) {
		if (buttons[poll].contains(p.x, p.y)) {
			//Serial.print(p.x);Serial.print(", ");Serial.println(p.y);      /******Testing: Check coordinates*******/
			//Serial.print("Pressing: ");Serial.println(buttonLabels[poll]);  /***Testing: Ensure button identified****/
			buttons[poll].press(true);                //Change state of button to pressed
		}
		else {
			buttons[poll].press(false);               //Change state of button to relased
		}
	}

	// Poll to see which button is pressed to give visual feedback.
	for (uint8_t poll = 0; poll<4; poll++) {
		if (buttons[poll].justReleased()) {
			//Serial.print("Released: "); Serial.println(buttonLabels[poll]); /***Testing: Check button released****/
			//Serial.println(F("Normal"));                                    /**Testing: Check "released" condition **/
			//buttons[poll].drawButton();                // draw normal
			selected = (ScreenSelector)8;				//Set screen selector to "none"
			return selected;
		}
		else if (buttons[poll].justPressed()) {
			//Serial.println(F("Invert!"));                                   /**Testing: Check "pressed" condition **/
			buttons[poll].drawButton(true);           // draw inverted
			delay(100);                               // Debounicng 
			selected = (ScreenSelector)poll;		  // Set screen selector to the button pressed values 0-4
			return selected;
		}
		//else { return none; }						 // If no response.
	}
}

/*Draws all Main screen elements of the screen
 @param: none
 @return: none
**********************************************************************************/
void RARA_MainScreen::drawScreen() {
	tft.fillScreen(BLACK);                 // Screen background color.
	drawBatteryIndicator(MAIN_LETTERCOLOR, MAIN_BG_COLOR);
	drawButtons();                         // Draw all screen buttons.
}

/*Draw all Main screen buttons
 @param: none
 @return: none
 *
 * - Private function only to be used for this particular screen layout.
**********************************************************************************/
void RARA_MainScreen::drawButtons() {

	//Designing a grid style layout of buttons.
	for (uint8_t row = 0; row<2; row++) {
		for (uint8_t col = 0; col<2; col++) {

			/* Initializes the buttons in the array with neccessary parameters for drawing.
			* Adafruit_GFX::initButton()
			* parameters:
			*    - *gfx: Adafruit_GFX    &tft, being added to.
			*    - x: int16_t           X-axis pos
			*    - y: int16_t           Y-axis pos
			*    - w: uint8_t           Width
			*    - h: uint8_t           Height
			*    - outline: uint16_t    Button outline color
			*    - fill: uint16_t       Button fill color
			*    - textcolor: uint16_t  Button text color
			*    - label: *char[]        Button label string of characters
			*    - textsize: uint8_t     Button Text size
			*/
			buttons[col + row * 2].initButton(
				&tft,
				BUTTON_X + col * (BUTTON_W + BUTTON_SPACING_X),
				BUTTON_Y + row * (BUTTON_H + BUTTON_SPACING_Y),
				BUTTON_W,
				BUTTON_H,
				WHITE,
				buttonColors[col + row * 2],   //Use the multiplier to ensure contiguous progression through the array.
				WHITE,
				buttonLabels[col + row * 2],
				BUTTON_TEXTSIZE);
			buttons[col + row * 2].drawButton();
		}
	}
}

/*Class destructor, clean up.
**********************************************************************************/
RARA_MainScreen::~RARA_MainScreen() {}


