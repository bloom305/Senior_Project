/*The base class for all UI screens for the RARA module.
 * Establishes all the connections,
 * communication, and base
 * design parameters for the all Graphical User Interface(GUI).

 * Devry Senior Project
 @group Team B
 @project RARA Module
 @author Rodrick Bloomfield 
 @version 1.1 08/26/17
*************************************************************/
#ifndef _RARA_SCREEN_h
#define _RARA_SCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

/*Core libraries for the touch screen and GUI design.
******************************************************/
#include <Adafruit_GFX.h>		// Core graphics library
#include <Adafruit_TFTLCD.h>	// Hardware-specific library
#include <TouchScreen.h>		// Touch Screen library
#include <pins_arduino.h>		// Arduino core pin assignments
#include <String.h>

/* Declarations used for frequently used constants when dealing with the TFT.
 * subsection - Pin assignments
 * subsection - Touch screen values
 * subsection - Colors
*********************************************************************************/
// TFT Screen pin constants.
#define LCD_CS A3               // Bus Select pin of LCD mapped to Analog Pin 3
#define LCD_CD A2               // Bus Command pin of LCD mapped to Analog Pin 2
#define LCD_WR A1               // Bus Write pin of LCD mapped to Analog Pin 1
#define LCD_RD A0               // Bus Read pin of LCD mapped to Analog Pin 0
#define LCD_RESET A4            // Bus Reset pin of LCD mapped to Analog Pin 4
#define YP A3                   // Touch Screen interface, shared pin.
#define XM A2                   // Touch Screen interfacing pin, shared pin.
#define YM 9                    // Touch Screen interfacing pin.
#define XP 8                    // Touch Screen interfacing pin.

// Touch Screen parameters. Primarily used in mapping input.
#define TS_MINX 120             // Minimum x-axis value for the touch screen as per calibration test.
#define TS_MAXX 900             // Maximum x-axis value for the touch screen as per calibration test.
#define TS_MINY 70              // Minimum y-axis value for the touch screen as per calibration test.
#define TS_MAXY 920             // Maxmum y-axis value for the touch screen as per calibration test.
#define MINPRESSURE 10          // Minimum z-axis value allowed to avoid false interaction, aides in debouncing.
#define MAXPRESSURE 1000        // Maxmum z-axis value allowed to avoid false interaction, aides in debouncing.
#define IDENTIFIER 0x9341       // Chip identifier, possible use during setup.

// Color definitions
#define BLACK       0x0000      /*   0,   0,   0 */
#define NAVY        0x000F      /*   0,   0, 128 */
#define DARKGREEN   0x03E0      /*   0, 128,   0 */
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY   0xC618      /* 192, 192, 192 */
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define BLUE        0x001F      /*   0,   0, 255 */
#define GREEN       0x07E0      /*   0, 255,   0 */
#define CYAN        0x07FF      /*   0, 255, 255 */
#define RED         0xF800      /* 255,   0,   0 */
#define LIGHTRED    0xFBEF      /* 253, 124,   0 */
#define MAGENTA     0xF81F      /* 255,   0, 255 */
#define YELLOW      0xFFE0      /* 255, 255, 120 */
#define WHITE       0xFFFF      /* 255, 255, 255 */
#define ORANGE      0xFD20      /* 255, 165,   0 */
#define GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define PINK        0xF81F

/*Enumeration for managing GUI input and selection*/
enum ScreenSelector{logScan, readScan, view, settings, previous, next, back, start, none};

/* Base Screen class for the RARA module GUI.
**********************************************************************************/
class RARA_Screen
{

public:
	/*Initalizer of the Adafruit TFT constructor. Identifying configurable controlling pins. Data bus pins as follows:
	// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
	// For the Arduino Uno, Duemilanove, Diecimila, etc.:
	//   D0 connects to digital pin 8  (Notice these are
	//   D1 connects to digital pin 9   NOT in order!)
	//   D2 connects to digital pin 2
	//   D3 connects to digital pin 3
	//   D4 connects to digital pin 4
	//   D5 connects to digital pin 5
	//   D6 connects to digital pin 6
	//   D7 connects to digital pin 7
	// For the Arduino Mega, use digital pins 22 through 29
	// (on the 2-row header at the end of the board).
	*/
	Adafruit_TFTLCD tft = Adafruit_TFTLCD(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

	//Initalizer of the TouchScreen Constructor. Identy configurable controlling pins. 
	TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

	RARA_Screen();                 //Class Contructor
	void initScreen();             //Establish serial connections and exchanges information with the screen.

	void drawBatteryIndicator(uint16_t, uint16_t);
	void batteryIndicatorBars(uint8_t, uint16_t, uint16_t);
								   
	~RARA_Screen();					//Class Destructor, cleanup.

protected:

	// An array of colors to facilitate mass button creation in inheriting classes.
	uint16_t buttonColors[15] = { BLUE, GREEN, RED,
		DARKGREY, BLUE, BLUE,
		BLUE,  BLUE,  BLUE,
		BLUE,  BLUE,  BLUE,
		ORANGE,  BLUE,  ORANGE };

private:

};

#endif

