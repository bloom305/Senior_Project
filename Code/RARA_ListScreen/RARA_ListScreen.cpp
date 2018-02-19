/*Implementation of list screen class.

 * Devry Senior Project
 @group Team B
 @project RARA Module
 @author Rodrick Bloomfield 
 @version 1.1 08/26/17
*************************************************************/
#include "RARA_ListScreen.h"

/*Class constructor
*******************/
RARA_ListScreen::RARA_ListScreen() {}

/*Class constructor(overloaded)
 @param type The type of list to be displayed.

 * - Used to instantiate a specific type of list screen
*/
RARA_ListScreen::RARA_ListScreen(listType type) { list = type; }

/********************************GUI drawing functions***************************/

/* Set the backgound color based on the type of list to be displayed.
 @param none
 @return 16-bit color that will fill the backbgound of the screen.
*/
uint16_t RARA_ListScreen::setBGColor() {
	if (list == catalog) {
		return GREEN;
	}
	else if (list == inventory) {
		return LIGHTRED;
	}
}

/* Set the footer color based on the type of list to be dsiplayed
 @param none
 @return 16-bit color that will fill the footer background on the screen.
*/
uint16_t RARA_ListScreen::setFooterColor() {
	if (list == catalog) {
		return OLIVE;
	}
	else if (list == inventory) {
		return RED;
	}
}

/*Draws all List screen elements of the screen
 @param none
 @return none
********************************************/
void RARA_ListScreen::drawScreen() {
	drawBackground();
	drawTitle();
	drawListBlocks();
}

/*Draws all background elements based on the type of list screen.
 @param none
 @return none
*****************************************/
void RARA_ListScreen::drawBackground() {
	tft.fillScreen(BG_COLOR);
	drawBatteryIndicator(LIST_LETTER_COLOR, BG_COLOR);
	drawFooter(FOOTER_COLOR);
}

/*Draws title based on the type of list screen.
 @param none
 @return none
*****************************************/
void RARA_ListScreen::drawTitle() {
	char titleCharArray[15];		//convert string to array of chars for writing to the screen
									//Determine the type of list and write the corresponding title
	if (list == catalog) {
		titles[list].toCharArray(titleCharArray, titles[list].length() + 1);
		for (uint8_t character = 0; character < titles[list].length(); character++) {
			tft.drawChar(
				TITLE_X + character * TITLE_SPACING,
				TITLE_Y,
				titleCharArray[character],
				BLACK,
				GREEN,
				TITLE_TEXTSIZE
			);
		}
	}
	else if (list == inventory) {
		titles[list].toCharArray(titleCharArray, titles[list].length() + 1);
		for (uint8_t character = 0; character < titles[list].length(); character++) {
			tft.drawChar(
				TITLE_X + character * TITLE_SPACING,
				TITLE_Y,
				titleCharArray[character],
				BLACK,
				LIGHTRED,
				TITLE_TEXTSIZE
			);
		}
	}
}

/* Draws all footer elements based on the type of list screen.
 @param color The 16-bit color that the displayed footer background will be.
 @return none
*****************************************/
void RARA_ListScreen::drawFooter(uint16_t color) {

	tft.fillRect(0, FOOTER_Y, FOOTER_W, FOOTER_H, color);
	for (uint8_t a = 0; a < 2; a++) {
		//Parameters: x-pos, y-pos, width,height, color
		tft.fillRect(
			FOOTER_NUM_BLOCK_X + a * (FOOTER_NUM_BLOCK_W + FOOTER_NUM_BLOCK_SPACING_X),
			FOOTER_NUM_BLOCK_Y,
			FOOTER_NUM_BLOCK_W,
			FOOTER_NUM_BLOCK_H,
			WHITE
		);
	}

	//Parameters: x-pos, y-pos, char, fill, background, text size
	tft.drawChar(
		155,
		FOOTER_NUM_BLOCK_Y + 2,
		'/',
		BLACK,
		color,
		FOOTER_TEXTSIZE
	);

	drawNavButtons(color, false, both);	
}

/*Draws all navigation button elements for the footer.
 @param1 color The 16-bit color for the background of the buttons in normal and button color in invers.
 @param2 invert Whether the button is to be displayed in normal or inverted color scheme.
 @param3 whichOne The button to be effected by the function.
 @return none
*****************************************/
void RARA_ListScreen::drawNavButtons(uint16_t color, bool invert = false, direction whichOne = both) {
	uint16_t out_color[3] = { color, color, BG_COLOR }, // Color array for outline of color
			fill_color[3] = { color, color, BG_COLOR },	// Color array for fill of color
			text_color[3] = { color, color, BG_COLOR },	// Color array for text of color
			left_color = BLACK, 						// Button color 
			right_color = BLACK,						// Button color 
			goBack_color = BLACK;						// Button color 

	if (invert == true && whichOne == left){ 			// Inverted color scheme for left button.
		fill_color[0] = BLACK;
		text_color[0] = BLACK;
		left_color = color;
	}
	else if (invert == true && whichOne == right) {		// Inverted color scheme for right button.
		fill_color[1] = BLACK;
		text_color[1] = BLACK;
		right_color = color;
	}
	else if (invert == true && whichOne == goBack) {	// Inverted color scheme for back button.
		fill_color[2] = BLACK;
		text_color[2] = BLACK;
		goBack_color = BG_COLOR;
	}

	if (whichOne == left){								// Draw the left button
		navButtons[left].initButtonUL(
			&tft,
			NAV_BUTTON_X + left *(NAV_BUTTON_W + NAV_BUTTON_SPACING_X),
			NAV_BUTTON_Y,
			NAV_BUTTON_W,
			NAV_BUTTON_H,
			out_color[left],
			fill_color[left],
			text_color[left],
			navButtonLabels[left],
			1
		);
		navButtons[left].drawButton();	
	} 
	else if (whichOne == right) {						// Draw the right button
		navButtons[right].initButtonUL(
			&tft,
			NAV_BUTTON_X + right *(NAV_BUTTON_W + NAV_BUTTON_SPACING_X),
			NAV_BUTTON_Y,
			NAV_BUTTON_W,
			NAV_BUTTON_H,
			out_color[right],
			fill_color[right],
			text_color[right],
			navButtonLabels[right],
			1
		);
		navButtons[right].drawButton();
	}
	else if (whichOne == goBack) {						// Draw the back button
		navButtons[goBack].initButtonUL(
			&tft,
			BACK_BUTTON_X,
			BACK_BUTTON_Y,
			BACK_BUTTON_W,
			BACK_BUTTON_H,
			out_color[goBack],
			fill_color[goBack],
			text_color[goBack],
			navButtonLabels[goBack],
			1
		);
		navButtons[goBack].drawButton();
	}
	else {												// Draw all buttons
		for (uint8_t b = 0; b < 3; b++) {
			/* Initializes dimensions and characteristics of buttons
			*Parameters: element, x-pos, y-pos, width, height, outline color, fill color, text color, char array for button label, text size.
			*/
			if (b < 2){
				navButtons[b].initButtonUL(
					&tft,
					NAV_BUTTON_X + b *(NAV_BUTTON_W + NAV_BUTTON_SPACING_X),
					NAV_BUTTON_Y,
					NAV_BUTTON_W,
					NAV_BUTTON_H,
					out_color[b],
					fill_color[b],
					text_color[b],
					navButtonLabels[b],
					1
				);
				navButtons[b].drawButton();		//Draws buttons
			}
			else if (b == 2 ) {
				navButtons[b].initButtonUL(
					&tft,
					BACK_BUTTON_X,
					BACK_BUTTON_Y,
					BACK_BUTTON_W,
					BACK_BUTTON_H,
					out_color[b],
					fill_color[b],
					text_color[b],
					navButtonLabels[b],
					1
				);
				navButtons[b].drawButton();		//Draws buttons
			}
		}
			
	}

	/* Draw direcctional indicators to be overlaid over the nav buttons.
	 * Parameters: x1-pos, y1-pos, x2-pos, y2-pos, x3-pos, y3-pos, fill color
	*/
	//Left buttons
	tft.fillTriangle(
		TRI_BUTTON_X,
		TRI_BUTTON_Y + 6,
		TRI_BUTTON_X + TRI_BUTTON_W,
		TRI_BUTTON_Y - 6,
		TRI_BUTTON_X + TRI_BUTTON_W,
		TRI_BUTTON_Y + 18,
		left_color
	);

	//rigth buttons
	tft.fillTriangle(
		TRI_BUTTON_X + TRI_BUTTON_W + TRI_BUTTON_SPACING_X,
		TRI_BUTTON_Y - 6,
		TRI_BUTTON_X + TRI_BUTTON_W + TRI_BUTTON_SPACING_X + 18,
		TRI_BUTTON_Y + 6,
		TRI_BUTTON_X + TRI_BUTTON_W + TRI_BUTTON_SPACING_X,
		TRI_BUTTON_Y + 18,
		right_color
	);

	//goBack buttons, has triangle and rectangle combined into single element
	tft.fillTriangle(
		BACK_TRI_X,
		BACK_TRI_Y + 3,
		BACK_TRI_X + BACK_TRI_W,
		BACK_TRI_Y - 6,
		BACK_TRI_X + BACK_TRI_W,
		BACK_TRI_Y + 15,
		goBack_color
	);

	tft.fillRect(
				BACK_TRI_X +BACK_TRI_W,
				BACK_TRI_Y - 1,
				BACK_TRI_W,
				BACK_TRI_H,
				goBack_color
	);
}

/*Draws all list blocks that holds individual data for each tag.
 @param none
 @return none
*******************************/
void RARA_ListScreen::drawListBlocks() {
	//For single column viewing, four entries per page.
	uint8_t col = 0;
	for (uint8_t row = 0; row < 4; row++) {
		tft.fillRect(
				BLOCK_X + col * (BLOCK_W + BLOCK_SPACING_X) + 30,
				BLOCK_Y + row * (BLOCK_H + BLOCK_SPACING_Y),
				BLOCK_W + 100,
				BLOCK_H,
				LIGHTGREY);
			drawDataLabels(row, col);
			drawDataBlocks(row, col);		
	}

	//Multi-column display for 8-entries per page, more suited for larger screens.
	/*
	for (uint8_t row = 0; row < 4; row++) {
		for (uint8_t col = 0; col < 2; col++) {
			tft.fillRect(
				BLOCK_X + col * (BLOCK_W + BLOCK_SPACING_X),
				BLOCK_Y + row * (BLOCK_H + BLOCK_SPACING_Y),
				BLOCK_W,
				BLOCK_H,
				LIGHTGREY);
			drawDataLabels(row, col);
			drawDataBlocks(row, col);
		}
	}
	*/
}

/*Draws all data labels for the list blocks that holds individual data.
 @param1 row Pass positional reference information of list block.
 @param2 col Pass positional reference information of list block, used for multi column design.
 @return none.
********************************/
void RARA_ListScreen::drawDataLabels(uint8_t row, uint8_t col) {
	//Manuvers through data label array and prints titles.

	//For single column display.
	for (uint8_t label = 0; label < 2; label++) {
		char dataLabelsCharArray[15];
		dataLabels[label].toCharArray(dataLabelsCharArray, dataLabels[label].length());
		for (uint8_t character = 0; character < dataLabels[label].length(); character++) {
			tft.drawChar(
				BLOCK_X + 2 + col * (BLOCK_W + BLOCK_SPACING_X) + character * 6 + 30,
				BLOCK_Y + 5 + row * (BLOCK_H + BLOCK_SPACING_Y) + label * 20,
				dataLabelsCharArray[character],
				BLACK,
				LIGHTGREY,
				LABEL_TEXTSIZE
			);
		}
	}

}

/*Draws all data blocks for the list blocks that holds individual data.
 @param1 row Pass positional reference information of list block.
 @param2 col Pass positional reference information of list block, used for multi column design.
 @return none.
**********************************/
void RARA_ListScreen::drawDataBlocks(uint8_t row, uint8_t col) {
	//For single column display.
	for (uint8_t datablock = 0; datablock < 2; datablock++) {
		tft.fillRect(
			DATA_BLOCK_X + col * (BLOCK_W + BLOCK_SPACING_X) + 30,
			DATA_BLOCK_Y + row * (BLOCK_H + BLOCK_SPACING_Y) + datablock * DATA_BLOCK_SPACING_Y,
			DATA_BLOCK_W /*- datablock * DATA_BLOCK_SPACING_X*/ + 90,
			DATA_BLOCK_H,
			WHITE
		);
	}

	//For multi-column display.
	/*
	for (uint8_t datablock = 0; datablock < 2; datablock++) {
		tft.fillRect(
			DATA_BLOCK_X + col * (BLOCK_W + BLOCK_SPACING_X) + datablock * DATA_BLOCK_SPACING_X,
			DATA_BLOCK_Y + row * (BLOCK_H + BLOCK_SPACING_Y) + datablock * DATA_BLOCK_SPACING_Y,
			DATA_BLOCK_W - datablock * DATA_BLOCK_SPACING_X,
			DATA_BLOCK_H,
			WHITE
		);
	}
	*/
}

/*Gathers and handles the user input.
 @param none
 @return Screen selector value to determine action user input.
 *
 * - Use in the loop to poll screen for user interaction.
*****************************/
ScreenSelector RARA_ListScreen::touchResponse() {
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
	for (uint8_t poll = 0; poll < 3; poll++) {
		if (navButtons[poll].contains(p.x, p.y)) {
			//Serial.print(p.x);Serial.print(", ");Serial.println(p.y);      /******Testing: Check coordinates*******/
			//Serial.print("Pressing: ");Serial.println(buttonLabels[poll]);  /***Testing: Ensure button identified****/
			navButtons[poll].press(true);                //Change state of button to pressed
		}
		else {
			navButtons[poll].press(false);               //Change state of button to relased
		}
	}

	// Poll to see which button is pressed to give visual feedback.
	for (uint8_t poll = 0; poll < 3; poll++) {
		if (navButtons[poll].justReleased()) {
			//Serial.print("Released: "); Serial.println(buttonLabels[poll]); /***Testing: Check button released****/
			//Serial.println(F("Normal"));                                    /**Testing: Check "released" condition **/

			drawNavButtons(FOOTER_COLOR, false, (direction)poll);
			selected = (ScreenSelector)8;					// Screen slecctor value of "none".
			return selected;
			//navButtons[poll].drawButton();                // draw normal
		}
		else if (navButtons[poll].justPressed()) {
			//Serial.println(F("Invert!"));                                   /**Testing: Check "pressed" condition **/
			//navButtons[poll].drawButton(true);  
			drawNavButtons(FOOTER_COLOR, true, (direction)poll);	// draw inverted
			delay(100);                               				// Debounicng 
			selected = (ScreenSelector) (4 + poll);					// Screen selector value based on user input, offset for values 4-7.
			return selected;
		}
	}

}

/*Class destructor, clean up.*/
RARA_ListScreen::~RARA_ListScreen() {}

/************************************Accessor**********************************/

/* Access the size of the list of RFIDS.
 @param none
 @return The value of how many data elements are to be displayed.
*****************************/
uint8_t RARA_ListScreen::getListSize() { return listSize; }

/*Access the current page number displayed.
 @param none
 @return The value of the current page of the list to be displayed.
*****************************/
uint8_t RARA_ListScreen::getCurrentPage() { return currentPage; }

/*Access the total number of pages.
 @param none
 @return The value of the total numbe pages needed to display list.
*****************************/
uint8_t RARA_ListScreen::getNumOfPages() { return numOfPages; }

/*Access the number of entries to be displayed on the last page of the list.
 @param none
 @return The value of the amount of data elements to be displayed on the last page.
*****************************/
uint8_t RARA_ListScreen::getLastPageAmount() { return numOfItemsOnLastPage; }

/*Access the title to be displayed at the top of the screen, telling the type of list.
 @param none
 @return The string of tht title to be displayed based on the list type.
*****************************/
String RARA_ListScreen::getTitle() { return titles[list]; }

/* Access the last tool to be displayed on screen.
 @param none
 @return The integer value of the last element in the list ot be displayed. 
*/
uint8_t RARA_ListScreen::getCurrentToolNum() { return currentTool; }

/**********************************Mutators************************************/

/*Sets the size of the list of RFIDS.
 @param size The value of how many data elements in the list to be displayed.
 @return none
*****************************/
void RARA_ListScreen::setListSize(uint8_t size) { listSize = size; }

/* Sets the current page number to be displayed.
 @param page The current page of the list.
 @return none
*****************************/
void RARA_ListScreen::setCurrentPage(uint8_t page) { currentPage = page; }

/*Sets the total number of pages for the list.
 @param none
 @return none
*****************************/
void RARA_ListScreen::setNumOfPages() { 
	if ((listSize > 4) && (listSize % 4 != 0)) { 	// The list size is greater than 4 and cannot be evenly divided by four. 
		numOfPages = (listSize / 4 + 1); 			// Add another page for the overflow.
	}
	else if (listSize <= 4) { 						// The list is less than or equal to four elements.
		numOfPages = 1; 
	}
	else { 											// The list is greater than four and is a multiple of four.
		numOfPages = listSize / 4; 
	}
}

/*Sets the number of tags to be listed on the last page
 @param none
 @return The interger value of number of elements to be displayed on the last page.
*****************************/
void RARA_ListScreen::setLastPageAmount() { numOfItemsOnLastPage = listSize % 4; }

/*Sets the title to be displayed.
 @param type The type of list that is going to be displayed.
 @return none
*****************************/
void RARA_ListScreen::setListType(listType type) { list = type; }

/* Sets the value of the last tool displayed.
 @param num The value of the last tool to be displayed.
 @return none
*/
void RARA_ListScreen::setCurrentToolNum(uint8_t num) { currentTool = num; }

/******************************Update Data Elements********************************/

/* Updates the displayed value of total number of pages to display lsit elements.
 @param none
 @return none
*/
void RARA_ListScreen::updateNumOfPages() {
	String stringPages = (String)numOfPages;  	// Cast number of pages to string for character conversion.
	char charPages[2];						 	// Character arrays to hold number of pages string.
	stringPages.toCharArray(charPages, 2, 0); 	// Conversion of string to char array.
	
	//Draw the characters
	tft.drawChar(
		FOOTER_NUM_BLOCK_X + FOOTER_NUM_BLOCK_W + FOOTER_NUM_BLOCK_SPACING_X + 4,
		FOOTER_NUM_BLOCK_Y + 2,
		charPages[0],
		BLACK,
		WHITE,
		FOOTER_TEXTSIZE
	);
}

/* Updates the displayed value of the current page of the list.
 @param none
 @return none
*/
void RARA_ListScreen::updatePageNum() {
	String stringPages = (String)currentPage;	// Cast current page to string for character conversion.
	char charPages[2];							// Character array to hold the number to be displayed.
	stringPages.toCharArray(charPages, 2, 0);	// Conversion of string to char array.
	
	tft.drawChar(
		FOOTER_NUM_BLOCK_X + 4,
		FOOTER_NUM_BLOCK_Y + 2,
		charPages[0],
		BLACK,
		WHITE,
		FOOTER_TEXTSIZE
	);
}

/* Augment the last data element to be displayed by an offset.
 @param changeBy The offfset for the last element to be displayed.
 @return none
*/
void RARA_ListScreen::changeCurrentToolNum(int changeBy) { currentTool += changeBy; }

/* Augment the current page to be displayed by an offset.
 @param changeBy The offfset for the current page to be displayed.
 @return none
*/
void RARA_ListScreen::changeCurrentPage(int changeBy) { currentPage += changeBy; }

/* Update the tag information on the screen.
 @param boxToDisplay The data structure to be displayed in the list.
 @return none
*/
void RARA_ListScreen::displayTagInfo(Toolbox boxToDisplay) {
	uint8_t blocksTofill;
	if ((boxToDisplay.getLength() - currentTool) < 4) { blocksTofill = (boxToDisplay.getLength() - currentTool); }
	else if (boxToDisplay.getLength() < 4) { blocksTofill = boxToDisplay.getLength(); }
	else  { blocksTofill = 4; }

	for (uint8_t a = 0; a < blocksTofill; a++) {
		for (uint8_t character = 0; character < EPC_CHAR_COUNT; character++) {
			tft.drawChar(
				DATA_BLOCK_X + 32 + character * 6,
				DATA_BLOCK_Y + 2 + a * (BLOCK_H + BLOCK_SPACING_Y),
				boxToDisplay.box[(4 * (currentPage - 1)) + a].EPC_Char[character],
				BLACK,
				BLACK,
				LABEL_TEXTSIZE
			);
		}
	}

	for (uint8_t a = 0; a < blocksTofill; a++) {
		for (uint8_t character = 0; character < EPC_COUNT; character++) {
			tft.drawChar(
				DATA_BLOCK_X + 32 + character * 6,
				DATA_BLOCK_Y + 4 + a * (BLOCK_H + BLOCK_SPACING_Y) + 20,
				(char)boxToDisplay.box[(4 * (currentPage - 1)) + a].EPC[character],
				BLACK,
				BLACK,
				LABEL_TEXTSIZE
			);
		}
		//changeCurrentToolNum(1);
	}	
}

/* Dsiplay and update data to be displayed for first page of list.
 @aparam none
 @return none
*/
void RARA_ListScreen::firstPage() {
	setCurrentPage(1);				// Set the current page value to be displayed.
	setCurrentToolNum(0);			// Start from the beggining of the tool list.
	setNumOfPages();				// Calculate the number of pages needed to display list.
	drawScreen();					// Draw the static visual elements.
	updateNumOfPages();				// Display the total number of pages to dsiplay the list.
	updatePageNum();				// Display the current page.
}

/* Dsiplay and update data to be displayed when pressing the next button.
 @aparam none
 @return none
*/
void RARA_ListScreen::nextPage() {
	changeCurrentPage(1);			// Increment the page to be displayed by one.
	changeCurrentToolNum(4);		// Increment the tool number by the amount dsiplayed on the previous page.
	drawScreen();					// Draw the static visual elements.
	updateNumOfPages();				// Display the total number of pages to dsiplay the list.
	updatePageNum();				// Dsiplay the current page.
}

/* Dsiplay and update data to be displayed when pressing the previous button.
 @aparam none
 @return none
*/
void RARA_ListScreen::previousPage() {
	changeCurrentPage(-1);			// Decrement current page number by one.
	changeCurrentToolNum(-4);		// Decrement the tool number by the beginning value of the previous page.
	drawScreen();					// Draw the static visual elements.
	updateNumOfPages();				// Display the total number of pages to dsiplay the list.
	updatePageNum();				// Display the current page.
}