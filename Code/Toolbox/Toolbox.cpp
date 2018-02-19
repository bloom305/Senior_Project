/* Implementation of toolbox class.

 * Devry Senior Project
 @group Team B
 @project RARA Module
 @author Rodrick Bloomfield 
 @version 1.1 08/26/17
*************************************************************/
#include "Toolbox.h"

/* Class constructor*/
Toolbox::Toolbox(){
	length = 0;
}

/* Overiddent class constructor for setting toolbox size. Currently not used.
 @param size The desired maximum size of the toolbox
 TODO: Implement the ability to change and manage the max size of the box.*/
Toolbox::Toolbox(int size){}

/* Overidden assignment operator.
 @param1 Left hand side, the toolbox that information will be copied to.
 @param2 box Right hand side, the toolbox that information will be copied from.
 @return none
*/
void Toolbox::operator= (Toolbox box) {
	this->clearBox();								// Clears the box to be written to.										
	for (uint8_t a = 0; a < box.getLength(); a++) {	// Step through all the elements in the box to be copied from.
		this->add(box.box[a]);						// Add each element.
	}
}

/* Compares two toolboxs that should be equal and places the missing elements in another toolbox.
 @param1 box1 The box to be searched to make sure it has all the same elements of toolbox calling the function.
 @param2 box2 The box that will hold all the elements that are missing.
 @return 	  If a missing item is found return true, otherwise false.
*/
bool Toolbox::compare(Toolbox box1, Toolbox &box2) {
	bool noMatch = false;										// Keep track whether a missing item is found.
	box2.clearBox();											// Ensure box to hold missing items is clear.

	for (uint8_t a = 0; a < this->getLength(); a++) {			// Step through the array of the toolbox calling this fucntion.
		if (box1.binarySearch(this->box[a])) { 					// Check if box1 contains the element of calling toolbox.
			//Serial.println("Found in search");				// Test: Verify that a tool is found in the search.
		}
		else {
			//Serial.println("Added a tool to the missing box");// Test: Verify that a tool was not found and is beeing added to holder box.
			box2.add(this->box[a]);								// Add tool to the holder toolbox.
			noMatch = true;										// Missing tool found.
		}
	}

	//box2.printBox();											// Test: Display the tools that were found to be missing.
	if (noMatch) { return true; }
	else { return false; }
}

/* Check if the toolbox is empty.
 @param none
 @return True if the element count is zero, else false.
*/
bool Toolbox::isEmpty() { return length == 0; }

/* Access the length of the array list.
 @param none
 @return The current number of the array list length.
*/
uint8_t Toolbox::getLength() { return length; }
 
/* Access the maximum size of the toolbox
 @param none
 @return The maximum allowable size of the array list.
*/ 
int Toolbox::getSize() { return SIZE; }

/* Adds a RFID tag element to the toolbox. Only adds element if a similar entry was not made.
 @param tool The tag information that is to be added to the toolbox.
 @return none
*/
void Toolbox::add(RFID_Tag tool){
	
	if (length >= SIZE) {
		Serial.println(F("list is full"));						// Test: Message to inform the maximum size has been reached.
	}
	else if (length < SIZE && !(binarySearch(tool))) {			// Check if tag below maximum and whether the element is already in the toolbox.
		box[length] = tool;										// Append the tag info to the toolbox.
		
		/*Test: Message for verifying the information was added, current number of tools, and the data added.
		
		Serial.println("Added a tool to the box");
		Serial.print("Num of Tools: "); Serial.println(length + 1);
		box[length].printTagEPC();Serial.println();
		*/
		
		length ++;												// Increment the number of tools in the toolbox.
	}
	else { 
		Serial.println("That tag is already in the box"); 		// Test: Duplicate information not added.
	}	
}

/* Removes tag information from the toolbox at a location.
 @param pos The location that the information is to be removed from.
 @return none
*/
void Toolbox::removeAt(int pos){
	for (int b = pos; b < length - 1; b++) {	// Steps through the toolbox from position.
		box[b] = box[b + 1];					// Overwrites the data of subsequent elements to fill the gap.	
	}
	length--;									// Decremnt the length of the list array.
}

/* Clears the content of the toolbox by resetting the counter to zero.
 @param none
 @return none
*/
void Toolbox::clearBox() { length = 0; }

/* Replace tag information at a location with the provided tag information.
 @param1 location The position that the information is going to be writtent at.
 @param2 item The tag that will be writtent to the position.
 @return none
*/
void Toolbox::replaceTag(int location, RFID_Tag item){
	if (location < 0 || location >= length) {			// Check if location is within the bounds of current toolbox data elements.
		//Serial.println(F("Error: Invalid location"));	// Test: Verify only valid locations can be written to.
	}
	else {
		box[location] = item;							// Assigns data to the location in the toolbox.
	}
}

/* Add tag information at a location within current toolbox size. Does not account for repeated element data.
 @param1 location The position that the information will be added at.
 @param2 item The item that will be added a selected position.
*/
void Toolbox::addAt(int location, RFID_Tag item){
	if (location < 0 || location >= length) {			// Check if position is within the bounds of current toolbox data elements.
		//Serial.println(F("Error: Invalid location"));	// Test: Verify only valid locations will be written to.
	}
	else if (length == SIZE) {							// Check if array is already at maximum capacity.
		//Serial.println(F("Error: Array is full"));		// Test: Verify that array cannot be written past maximum capacity.
	}
	else {
		for (int c = length; c > location; c--) {		// Step through the toolbox in reverse from the end to position information to be added at.
			box[c] = box[c - 1];						// Write data to backend of array list to create gap to avoid data loss.
		}
		box[location] = item;							// Write data to location.
		length++;										// Increment the toolbox length.
	}
}

/* Remove a particular tag from the toolbox. Does not account for repeats 
 @param item The item to be removed.
 @return none
*/
void Toolbox::remove(RFID_Tag item){
	for (int a = 0; a < length; a++) {	// Step through the toolbox.
		if (box[a] == item) {			// Check if item is a match for an element in the array list.
			removeAt(a);				// Remove at that location.
			a--;						// Make sure the tag was removed.
		}
	}
}

/* Get the element at a location.
 @param location The position in the array list t of interest.
 @return RFID tag at the given location.
*/
RFID_Tag Toolbox::get(int location){
	RFID_Tag foundItem;									// Temporary holder for tag data, once found.

	if (location < 0 || location >= length) {			// Check if location is within the current bounds of the toolbox.
		//Serial.println(F("Error: invalid location"));	// Test: Verify only location within toolbox bounds can be accessed.
	}
	else {
		foundItem = box[location];
	}
	return foundItem;
}

/* Copy a toolbox
 @param none
 @return The copied information in a new toolbox.
*/
Toolbox Toolbox::copy(){
	Toolbox newBox;						//Holder toolbox object

	for (int a = 0; a < length; a++) {	// Step through toolbox elements
		newBox.add(this->box[a]);		// Add element into holder object
	}

	return newBox;						// Return object
}

/* Sort elements in the array list.
 @param none
 @return none 
*/
void Toolbox::bubbleSort(){
	RFID_Tag temp;
	for (int a = 0; a < length - 1; a++) {
		for (int b = 0; b < length - a - 1; b++) {
			if (box[b] > box[b + 1]) {
				temp = box[b];
				box[b] = box[b + 1];
				box[b + 1] = temp;
			}
		}
	}
}

/* Recursive sort of array list, public member.
 @param none
 @return none
*/
void Toolbox::quicksort() { quicksort(0, length - 1); }

/* Search the array for a matching tag element, elements must be sorted.
 @param item The tag that to be identified whether it is in the array list.
 @return 	 If tag is found return true, otherwise false.
*/
bool Toolbox::binarySearch(RFID_Tag item){
	quicksort();

	int first = 0, last = length - 1, pivot;	// Positional indicators used to traverse the array.
	bool found = false;							// Indicate if item is found.

	while (first <= last && !found) {			// Did not travers the entire array and item not found.
		pivot = (first + last) / 2;				// Seperation point of the array list.
		if (box[pivot] == item)					// Check if item matches midpoint element.
			found = true;						
		else if (item < box[pivot])				// Check if item is less than pivot point element.
			last = pivot - 1;					// Concetrate on the lower half of the array list.
		else
			first = pivot + 1;					// Concentrate on the upper part of the list array.
	}

	if (found)									// Check if the element is found.
		return true;
	else
		return false;
}

/* Print tag information for all elements in the toolbox.
 @param none
 @return none
*/
void Toolbox::printBox() {
	Serial.println("*******************************************");
	for (uint8_t a = 0; a < this->getLength(); a++) {
		this->box[a].printTag();
	}
	Serial.println("*******************************************");
	Serial.println();
}

/* Class destructor */
Toolbox::~Toolbox(){}

/* Generate random toolbox for the maximum capacity of the toolbox.
 @param none
 @return none
*/
void Toolbox::randomBox() {
	RFID_Tag temp;
	for (uint8_t a = 0; a < SIZE; a++){
		add(temp.randomTag());
	}
}

/* Generate random toolbox for a set amount of elements
 @param tools The number of tools desired in the toolbox.
 @return none
*/
void Toolbox::randomBox(uint8_t tools) {
	RFID_Tag temp;
	for (uint8_t a = 0; a < tools; a++){
		add(temp.randomTag());
		//box[a] = temp.randomTag();
	}
}

/* Recursive sort function. Private member.
 @param1 begin The start of array area to be sorted.
 @param2 end   The end of array area to be sorted.
 @return none
*/
void Toolbox::quicksort(int begin, int end){
	RFID_Tag temp;									// Holder element, used to re-arrange array list.
	int pivot = findPivotLocation(begin, end);		// Midpoint of the array list.

	//Exchange the pivot element with the end element.
	temp = box[pivot];							
	box[pivot] = box[end];
	box[end] = temp;
	pivot = end;								

	int i = begin, j = end - 1;						
	bool iterationCompleted = false;				

	while (!iterationCompleted) {					
		while (box[i] < box[pivot])					
			i++;
		while ((j >= 0) && (box[pivot] < box[j]))	
			j--;
		if (i < j) {							
			temp = box[i];							
			box[i] = box[j];
			box[j] = temp;

			i++;
			j--;
		}
		else {
			iterationCompleted = true;
		}
	}

	temp = box[i];
	box[i] = box[pivot];
	box[pivot] = temp;

	if (begin < i - 1)
		quicksort(begin, i - 1);
	if (i + 1 < end)
		quicksort(i + 1, end);
}

/* Find the pivot point of the array list.
 @param1 b The beginning point of the array list.
 @param2 e The end point of the array list.
 @return   The pivot point value.
*/
int Toolbox::findPivotLocation(int b, int e){ return (b + e) / 2; }

