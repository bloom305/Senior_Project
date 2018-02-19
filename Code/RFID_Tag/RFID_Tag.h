/* Data class for information from the RFID tag response.

 * Devry Senior Project
 @group Team B
 @project RARA Module
 @author Rodrick Bloomfield 
 @version 1.1 08/26/17
*************************************************************/
#ifndef _RFID_TAG_h
#define _RFID_TAG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <time.h>
#include <String.h>

// Data array size constants.
#define EPC_COUNT 12
#define MAX_USER_DATA 64
#define EPC_CHAR_COUNT ((EPC_COUNT *2)+(EPC_COUNT - 1))
#define TID_COUNT 20
#define TID_CHAR_COUNT ((TID_COUNT * 2) + (TID_COUNT -1))

class RFID_Tag
{
public:
	//String message;

	//byte EPC_Bytes;
	byte EPC[EPC_COUNT];						// Array of EPC data in hex.
	String EPC_String;
	char EPC_Char[EPC_CHAR_COUNT];				// EPC data in char array for dsiplay.

	//byte tagDataBytes;						
	//byte userData[MAX_USER_DATA];
	//String userDataString;
	//char userDataString_Char[MAX_USER_DATA];

	//byte TID_Bytes;
	//byte TID[TID_COUNT];
	//char TID_Char[TID_CHAR_COUNT];

	//int rssi;
	//long frequency;
	//long timeStamp;

	RFID_Tag();
	bool operator== (RFID_Tag);					// "is equal to" operator for comparison functions.
	bool operator> (RFID_Tag);					// "greater than" operator for comparison functions.
	bool operator>= (RFID_Tag);					// "greater than or equal to" operator for comparison functions.
	bool operator< (RFID_Tag);					// "less than" operator for comparison functions.
	bool operator<= (RFID_Tag);					// "less than or equal to" operator for comparison functions.
	void operator= (RFID_Tag);					// assignment operator.
	void printTagEPC();							// Print the tag EPC data in Serial for testing.
	void printTag();							// Print tag data in Serial for testing purposes.
	~RFID_Tag();

	RFID_Tag randomTag();						// Generate random tag data testing purposes.

private:
	void setToChar(byte []);					// Set the EPC array to character array for display purposes.
	void setToString(byte []);
};

#endif

