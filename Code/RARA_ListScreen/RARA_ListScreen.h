/*Draws elements for list screeen. Elements may vary based on type of list.

 * Devry Senior Project
 @group Team B
 @project RARA Module
 @author Rodrick Bloomfield 
 @version 1.1 08/26/17
*************************************************************/
#ifndef _RARA_LISTSCREEN_h
#define _RARA_LISTSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "RARA_Screen.h"
#include <string.h>  
#include <Toolbox.h>     //Custome container class for data collection and display.

/* Declarations used for frequently used constants when dealing with screeen layout and elements.
 * subsection - list blocks, main containers
 * subsection - data blocks, contains the tag information
 * subsection - Footer details, including navigation buttons.
 * subsection - Title details.
 * subsection - Back to main screen button details.
***********************************************************/
/*List block definitions*/
#define BLOCK_X 5
#define BLOCK_Y 24
#define BLOCK_W 150
#define BLOCK_H 40
#define BLOCK_SPACING_X 12
#define BLOCK_SPACING_Y 6

/*Data block definitions*/
#define DATA_BLOCK_X (BLOCK_X + 24)
#define DATA_BLOCK_Y (BLOCK_Y + 3)
#define DATA_BLOCK_W 122
#define DATA_BLOCK_H 12
#define DATA_BLOCK_SPACING_X 35
#define DATA_BLOCK_SPACING_Y 21
#define LABEL_TEXTSIZE 1.8
#define DATA_TEXTSIZE 2

/*Footer layout and element definitions*/
#define FOOTER_Y 210
#define FOOTER_W 320
#define FOOTER_H 30
#define FOOTER_NUM_BLOCK_X 125
#define FOOTER_NUM_BLOCK_Y 215
#define FOOTER_NUM_BLOCK_W 20
#define FOOTER_NUM_BLOCK_H 20
#define FOOTER_NUM_BLOCK_SPACING_X 32
#define FOOTER_TEXTSIZE 2
#define NAV_BUTTON_X 0
#define NAV_BUTTON_Y 210
#define NAV_BUTTON_W 30
#define NAV_BUTTON_H 30
#define NAV_BUTTON_SPACING_X 259
#define TRI_BUTTON_X 5
#define TRI_BUTTON_Y 220
#define TRI_BUTTON_W 18
#define TRI_BUTTON_H 22
#define TRI_BUTTON_SPACING_X 273

/*Title layout definitions*/
#define TITLE_X 55
#define TITLE_Y 2
#define TITLE_SPACING 16
#define TITLE_TEXTSIZE 3

/*Back  button layout definitions*/
#define BACK_BUTTON_X 1
#define BACK_BUTTON_Y 1
#define BACK_BUTTON_W 40
#define BACK_BUTTON_H 22
#define BACK_TRI_X 2
#define BACK_TRI_Y 7
#define BACK_TRI_W 16
#define BACK_TRI_H 11

#define LIST_LETTER_COLOR BLACK

#define BG_COLOR setBGColor()
#define FOOTER_COLOR setFooterColor()

//enumeration of list type for management of switching list screen elements. 
enum listType { catalog, inventory };			//Determines the list screen color scheme.
enum direction {left, right, goBack, both };	//Determines the buttons pressed by user.

/* Multipurpose list screen class for the RARA module GUI.
**********************************************************************************/
class RARA_ListScreen : public RARA_Screen
{
public:
	ScreenSelector selected = none;

	RARA_ListScreen();					// Class constructor
	RARA_ListScreen(listType);			// Overloaded constructor(1) to setup list type.
	~RARA_ListScreen();					// Class destructor

	//Accessors
	uint8_t getListSize();				// Access list size.
	uint8_t getCurrentPage();			// Access value of current page number to be displayed.
	uint8_t getNumOfPages();			// Access value of the total number of list pages.
	uint8_t getLastPageAmount();		// Access the amount of data elements to be displayed on the last page.
	String getTitle();					// Access the title to be displayed on for the list screen.
	uint8_t getCurrentToolNum();		// Access the value of the last displayed data element.


	//Mutators
	void setListSize(uint8_t size);		// Set the list size value.
	void setCurrentPage(uint8_t page);	// Set the current page value.
	void setNumOfPages();				// Set the number of pages value.
	void setLastPageAmount();			// Set the value of the number of elements on the last page.
	void setListType(listType);			// Set the list type to be displayed.
	void setCurrentToolNum(uint8_t);	// Set the value of data element last accessed.

	void drawScreen();					// Draw all screen elements.
	ScreenSelector touchResponse();		// Gathers user input information and handles it.

	//Display and update data information elements
	void updateNumOfPages();			// Draw the total number of pages value displayed on screen.
	void updatePageNum();				// Draw the current page number value on screen.
	void changeCurrentPage(int);		// Update current page value by an offset value.
	void changeCurrentToolNum(int);		// Update current tool displayed value by an offset value.
	void displayTagInfo(Toolbox);		// Display tag data in data blocks. 
	void firstPage();					// Display and update data for the first page of a list.
	void nextPage();					// Display and update data for any subsequent press of the next button.
	void previousPage();				// Display and update data for any press of the previous button.

private:
	uint8_t currentTool;
	uint8_t listSize;											// Size of incoming RFID tag list.
	uint8_t currentPage = 1;									// THe current list page displayed.
	uint8_t numOfPages = 1;										// The # of pages needed to display the list.
	uint8_t numOfItemsOnLastPage;								// # of items displayed on last page.
	listType list = catalog;									// List type either catalog or missing tools list.
	Adafruit_GFX_Button navButtons[3];							// Array to hold button elements.
	char navButtonLabels[3][10] = { "Left", "Right", "bck"};	// Navbutton labels for positional reference during testing.
	String dataLabels[2] = { "EPC:","User Data:" };				// Datablock labels 
	String titles[2] = { "Current Tools", "Missing Tools" };	// Array holding possible title labels.
	//byte tags[10];											// Tags list (temporary holder).

	uint16_t setBGColor();										// Set the background color of list screen.
	uint16_t setFooterColor();									// Set the footer color of the list screen.
	void drawBackground();										// Draw all background elements.
	void drawTitle();											// Draw the title on screen.
	void drawFooter(uint16_t color);							// Draw footer elements on screen.
	void drawNavButtons(uint16_t color, bool, direction);		// Draw navigation buttons on screen.
	void drawListBlocks();										// Draw list element blocks.
	void drawDataLabels(uint8_t row, uint8_t col);				// Draw list element data labels.
	void drawDataBlocks(uint8_t row, uint8_t col);				// Draw list element data blocks.
};
#endif
