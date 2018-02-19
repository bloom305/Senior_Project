/* Mainscreen GUI class. Draws screen eleements and interprets user input.

 * Devry Senior Project
 @group Team B
 @project RARA Module
 @author Rodrick Bloomfield 
 @version 1.1 08/26/17
*************************************************************/
#ifndef _RARA_MAINSCREEN_h
#define _RARA_MAINSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "RARA_Screen.h"

/* Declarations used for frequently used constants when dealing with screeen layout and elements.
 * subsection - Buttons parameters
 * - Screen states (possible use)
*********************************************************************************/
#define BUTTON_X 75                   //Starting x-axis position of the first button on of the screen.
#define BUTTON_Y 75                   //Starting y-axis position of the first button on of the screen.
#define BUTTON_W 146                  //Uniform width of all Main screen buttons.
#define BUTTON_H 46                   //Uniform height of all Main screen buttons.
#define BUTTON_SPACING_X 25           //Uniform spacing along the x-axis for all Main screen buttons.
#define BUTTON_SPACING_Y 47           //Uniform spacing along the y-axis for all Main screen buttons.
#define BUTTON_TEXTSIZE 2             //Uniform text size for all Main screen buttons.

#define CREATE_LOG 0                  //Possible button or state assignment.
#define INVENTORY 1                   //Possible button or state assignment.
#define VIEW_LOG 2                    //Possible button or state assignment.
#define SETTINGS 3                    //Possible button or state assignment.
#define MAIN_LETTERCOLOR WHITE
#define MAIN_BG_COLOR BLACK

/* Main Screen (start-up screen) class for the RARA module GUI.
**********************************************************************************/
class RARA_MainScreen : public RARA_Screen
{
public:
	Adafruit_GFX_Button buttons[4];      //Array to hold predesigned rounded buttons that will be used on screen.
	char buttonLabels[4][10] = { "Log", "Check", "View Log", "Clear" }; //Labels for the buttons.
	ScreenSelector selected	;
	RARA_MainScreen();                   // Class constructor.
	ScreenSelector touchResponse();     // Gathers and handles the user input.
	void drawScreen();                   // Draw all elements on the screen.
	~RARA_MainScreen();                  // Class destructor.

private:
	void drawButtons();                  // Initializes and draws all of the Main Screens buttons.

};

#endif

