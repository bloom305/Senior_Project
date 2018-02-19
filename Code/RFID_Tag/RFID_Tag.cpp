/* Implementation of RFID tag class.

 * Devry Senior Project
 @group Team B
 @project RARA Module
 @author Rodrick Bloomfield 
 @version 1.1 08/26/17
*************************************************************/
#include "RFID_Tag.h"

/*Class constructor*/
RFID_Tag::RFID_Tag(){}

/* Overrriden operator function for comparison functions of "is equal to".
 @param1 On the left side of operator, tag information for base comparison.
 @param2 tag On the right side of operator, the tag information that base is compared to.
 @return (boolean) True if tag values are equal, otherwise false.
*/
bool RFID_Tag::operator==(RFID_Tag tag) { 
  bool isEqual = false;                   // Boolean used to keep track of as array is traversed.         
  for (uint8_t a = 0; a < 12; a++) {      // Stepping through the elements of the EPC array.
    if (this->EPC[a] == tag.EPC[a]) {     // Compares if each byte data in the base array  is "equal to" the corresponding tag array.
      isEqual = true;                     // If equal to mark as true and continue to next element.
    } 
    else { return false; }                // If any elements are not equal, immediately return false.
  }
  
  if (isEqual) { return true; }           // If all elements are equal then return true.
  
}

/* Overrriden operator function for comparison functions of "is greate than".
  @param1 On the left side of operator, tag information for base comparison.
  @param2 tag On the right side of operator, the tag information that base is compared to.
  @return (boolean) True if base values are greater than, otherwise false.
*/
bool RFID_Tag::operator>(RFID_Tag tag) { 
  bool EPC_Greater = false;                 // Keeps track of elements that are greater than.
  
  for (uint8_t a = 0; a < 12; a++) {        // Stepping through the elements of the EPC array.
    if (this->EPC[a] > tag.EPC[a]) {        // Check if element comparison is greate than.
      return true;                        
    }
    else if (this->EPC[a] == tag.EPC[a]) {  // Check if element is equal to.
      continue;                             // Go to next iteration of the for-loop.
    }
    else { return false; }                  // If less than immediately return false.
  }

  if (!EPC_Greater) { return false; }       // If all elements all equal then return false.
}

/* Overrriden operator function for comparison functions of "is greate than or equal to".
  @param1 On the left side of operator, tag information for base comparison.
  @param2 tag On the right side of operator, the tag information that base is compared to.
  @return (boolean) True if base values are greater than or equal to, otherwise false.
*/
bool RFID_Tag::operator>=(RFID_Tag tag) { 
  
  for (uint8_t a = 0; a < 12; a++) {        // Stepping through the elements of the EPC array.
    if (this->EPC[a] > tag.EPC[a]) {        // Check if element is greater than.
      return true;
    }
    else if (this->EPC[a] == tag.EPC[a]) {  // Check if element is equal to.
      continue;                             // Go to next iteration of the for-loop.
    }
    else { return false; }                  //If less than immediately return false.
  }

  return true;                              // All elements are equal, then return true.
}

/* Overrriden operator function for comparison functions of "is less than".
  @param1 On the left side of operator, tag information for base comparison.
  @param2 tag On the right side of operator, the tag information that base is compared to.
  @return (boolean) True if base values are less than, otherwise false.
*/
bool RFID_Tag::operator<(RFID_Tag tag) { 
  bool EPC_Less = false;
  
  for (uint8_t a = 0; a < 12; a++) {        // Step through the EPC array.
    if (this->EPC[a] < tag.EPC[a]) {        // Check if element is less than.
      return true;                          // Less than element found, return true.
    }
    else if (this->EPC[a] == tag.EPC[a]) {  // Check if elements are equal.
      continue;                             // Go to next iteration of the for-loop. 
    }
    else { return false; }                  // If greater than immediately return false.
  }

  return false;                             // All elements are equal, return false.
}

/* Overrriden operator function for comparison functions of "is less than or equal to".
  @param1 On the left side of operator, tag information for base comparison.
  @param2 tag On the right side of operator, the tag information that base is compared to.
  @return (boolean) True if base values are less than or equal to, otherwise false.
*/
bool RFID_Tag::operator<=(RFID_Tag tag) { 
  bool EPC_Less = false;
  bool isEqual = false;
  
  for (uint8_t a = 0; a < 12; a++) {        // Step through the EPC array elements
    if (this->EPC[a] < tag.EPC[a]) {        // Check if element is less than.
      return true; 
    }
    else if (this->EPC[a] == tag.EPC[a]) {  // Check if element is equal to.
      continue;                             // Go to next iteration of the for-loop.
    }
    else { return false; }                  // Greater than element, return false.
  }

  return true;                              // All elements are equal, return true.
}

/* Overrriden assignment operator.
  @param1 On the left side of operator, tag information to be copied to.
  @param2 tag On the right side of operator, the tag information is copied from.
  @return none
*/
void RFID_Tag::operator=(RFID_Tag tag) {
  for (uint8_t a = 0; a < EPC_COUNT; a++) {      // Go through the EPC array
    this->EPC[a] = tag.EPC[a];                   // Writes the byte information to each element
  }

  setToChar(this->EPC);                          // Transforms EPC data to object's char array for display.   
  setToString(this->EPC);                        // Transforms EPC data to object's string holder for testing purposes.

/* For future use with additional information from the tag.
  for (uint8_t a = 0; a < sizeof(tag.userData); a++) {
    this->userData[a] = tag.userData[a];
  }

  for (uint8_t a = 0; a < sizeof(tag.TID); a++) {
    this->TID[a] = tag.TID[a];
  }

  this->rssi = tag.rssi;
  this->frequency = tag.frequency;
  this->timeStamp = tag.timeStamp;
*/  
}

/* Sets the EPC hex data to a char array that will be used to write the information to the display.
 @param epcArray[] The array of hex data that needs to be converted.
 @return none
*/
void RFID_Tag::setToChar(byte epcArray[]) {
  // Used for mapping hex values to corresponding character.
  char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
                  'A', 'B', 'C', 'D', 'E', 'F'};

  for (uint8_t a = 0; a < EPC_COUNT; a++) {               // Step through EPC array.
    //Serial.println(this->EPC[a]);
    this->EPC_Char[a * 3] = hex[epcArray[a] / 16];        // 1st position of hex value.
    this->EPC_Char[1 + (a * 3)] = hex[epcArray[a] % 16];  // 2nd position of hex value.
    this->EPC_Char[2 + (a * 3)] = ' ';                    // Space between each hex value.
  }
}

/* Sets the EPC hex data to a string that will be used for testing purposes.
 @param epcArray[] The array of hex data that needs to be converted,
 @return none
*/
void RFID_Tag::setToString(byte epcArray[]) {
  String tempString;                          // Temporary string for manipulating data.
  for (uint8_t a = 0; a < EPC_COUNT; a++) {   // Step through EPC array.
    tempString.concat(char(epcArray[a]));     // Appends char casted hex data to tempString
  }

  this->EPC_String = tempString;              // Transfers tempString value to object string holder.
}

/* Prints the tags EPC data to Serial for testing purposes.
 @param none
 @return none
*/
void RFID_Tag::printTagEPC() {
    Serial.print(F(" epc["));
      for (byte x = 0 ; x < sizeof(this->EPC) ; x++)
      {
        if (this->EPC[x] < 0x10) Serial.print(F("0")); //Pretty print
        Serial.print(this->EPC[x], HEX);
        Serial.print(F(" "));
      }
      Serial.print(F("]"));

      Serial.println();
}

/* Print the tag information to Serial for testing purpose.
 @param none
 @return none
 TODO: Addd additional tag information gathering fucntionality and provide Serial printout, too.*/
void RFID_Tag::printTag() {
  Serial.print(F(" epc["));
  for (byte x = 0 ; x < sizeof(this->EPC) ; x++) {
    if (this->EPC[x] < 0x10) Serial.print(F("0")); //Pretty print
    Serial.print(this->EPC[x], HEX);
    Serial.print(F(" "));
  }
  Serial.print(F("]"));

  Serial.println();
  Serial.println(EPC_String);
  Serial.println();

/* For later use with the additional information gathered from tag.
  Serial.print(F(" epc_Char["));
  for (uint8_t x = 0 ; x < sizeof(this->EPC_Char) ; x++) {
    Serial.print(this->EPC_Char[x]);
    if ((x % 3) != 0) { Serial.print(F(" ")); }
  }
  Serial.print(F("]"));
  Serial.println();

  Serial.print(F(" User Data["));
  for (byte x = 0 ; x < sizeof(this->userData) ; x++) {
    if (this->userData[x] < 0x10) Serial.print(F("0")); //Pretty print
    Serial.print(this->userData[x], HEX);
    Serial.print(F(" "));
  }
  Serial.print(F("]"));

  Serial.println();

  Serial.print(F(" TID["));
  for (byte x = 0 ; x < sizeof(this->TID) ; x++) {
    if (this->TID[x] < 0x10) Serial.print(F("0")); //Pretty print
    Serial.print(this->TID[x], HEX);
    Serial.print(F(" "));
  }
  Serial.print(F("]"));
  Serial.println();

  Serial.print(F(" rssi = ")); Serial.print(this->rssi); Serial.println();
  Serial.print(F(" frequency = ")); Serial.print(this->frequency); Serial.println();
  Serial.print(F(" time stamp = ")); Serial.print(this->timeStamp); Serial.println();
  Serial.println();
*/
}

/* Class destructor*/
RFID_Tag::~RFID_Tag(){}

/* Generate psuedorandom tags for populating purposes and testing.
 @param none
 @return none
 TODO: Pre-populated array of tag info for more controlled testing information.
*/
RFID_Tag RFID_Tag::randomTag() {
  randomSeed(analogRead(random(A6)));
	for (uint8_t a = 0; a < 12; a++) {
		this->EPC[a] = random(32, 127); 
  }
/* Used to populate additional tag information for advance testing.
	for (uint8_t a = 0; a < 64; a++) {
		this->userData[a] = random(32, 127);
	}
  
	for (uint8_t a = 0; a < 20; a++) {
		this->TID[a] = random();
	}

	this->rssi = random();
	this->frequency = random();
  this->timeStamp = random();
*/  
}


