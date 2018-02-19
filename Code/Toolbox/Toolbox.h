/* Toolbox class used as a data structure to store and provide advance functionality of group RFID_Tags. 
   Array list type of data structure.

 * Devry Senior Project
 @group Team B
 @project RARA Module
 @author Rodrick Bloomfield 
 @version 1.1 08/26/17
*************************************************************/
#ifndef _TOOLBOX_h
#define _TOOLBOX_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <RFID_Tag.h>

#define SIZE 10							// Toolbox size maximum, large number can be a drain on dynamic memeory.
class Toolbox
{
public:
	RFID_Tag box[SIZE];					// Array list of RFID Tags.

	Toolbox();							// Constructor
	Toolbox(int);						// Overidden constructor for setting size (not used).
	void operator= (Toolbox);			// Overidden assignment operator
	bool compare(Toolbox, Toolbox &);	// Compare toolbox and get missing tools.
	bool isEmpty();						// Check if array list is empty.
	uint8_t getLength();				// Get the current length of the array list.
	int getSize();						// Get max capacity of toolbox.
	void add(RFID_Tag);					// Append a tag to the toolbox.
	void removeAt(int);					// Remove a tag at a position.
	void clearBox();					// Empty the box.
	void replaceTag(int, RFID_Tag);		// Replace an element at a position.
	void addAt(int, RFID_Tag);			// Add an element at a position.
	void remove(RFID_Tag);				// Remove a particular element from the toolbox.
	RFID_Tag get(int);					// Get the tag at a position.
	Toolbox copy();						// Copy the toolbox.
	void bubbleSort();					// Sorting function.
	void quicksort();					// Sorting function.
	bool binarySearch(RFID_Tag);		// Serching function.
	void printBox();					// Print elements in the data structure.
	~Toolbox();							// Destructor

	void randomBox();					// Generate a random box for testing.
	void randomBox(uint8_t);			// Generate a random box with a number of elements.

private:
	int length = 0;						// Number of elements in structure.
	void quicksort(int, int);			// Sorting function.
	int findPivotLocation(int, int);	// Finds pivot point for sorting functions.
};

#endif

