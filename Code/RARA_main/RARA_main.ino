/* Main Arduino file.

 * Devry Senior Project
 @group Team B
 @project RARA Module
 @author Rodrick Bloomfield 
 @version 1.1 08/26/17
*************************************************************/
#include <EEPROM.h>                     // Library for reading/ writing to storage
#include <RARA_ListScreen.h>            // List screen library
#include <RARA_MainScreen.h>            // Main screen library
#include <RFID_Tag.h>                   // RFID tag class
#include <Toolbox.h>                    // Toolbox data structure
#include <SoftwareSerial.h>             // Used for transmitting to the M6E Nano module
#include "SparkFun_UHF_RFID_Reader.h"   // Library for controlling the M6E Nano module

#define BUZZER1 22
#define BUZZER2 23

SoftwareSerial softSerial(A9, A8);      // Software serial communications to the RFID module(RX, TX)
RARA_Screen screen;
RARA_MainScreen mainScreen;             // Create instance of a mainScreen object.
RARA_ListScreen listScreen;             // Create an instance of a listScreen object.
RFID nano;                              // Create an instance of a RFID reader.
int eeAddress;
int counter;                            // Used for the keeping track of the time counter.
int rssi;                               // Holder for temporary rssi data from the tag.
long freq;                              // Holder for temporary frequency data from the tag.
long timeStamp;                         // Holder for time stamp data from the tag.
byte tagEPCBytes;                       // Holder for the EPC bytes data from the tag.
byte tagDataBytes;                      // Holder for the data bytes data from the tag.
byte tempMes[10][55];
// Byte data for populating test tags.
byte msg[72] = { 0x20, 0x57, 0x72, 0x65, 0x6E, 0x63, 0x68, 0x20, 0x20, 0x20, 0x20, 0x20,
                 0x20, 0x53, 0x63, 0x72, 0x65, 0x77, 0x64, 0x72, 0x69, 0x76, 0x65, 0x72,
                 0x20, 0x50, 0x6C, 0x69, 0x65, 0x72, 0x73, 0x20, 0x20, 0x20, 0x20, 0x20,
                 0x20, 0x33, 0x2F, 0x34, 0x20, 0x57, 0x72, 0x65, 0x6E, 0x63, 0x68, 0x20,
                 0x20, 0x53, 0x63, 0x69, 0x73, 0x73, 0x6F, 0x72, 0x73, 0x20, 0x20, 0x20,
                 0x20, 0x53, 0x6E, 0x69, 0x70, 0x73, 0x2C, 0x43, 0x72, 0x6F, 0x73, 0x73
               };

byte tempEPC[12];
Toolbox catalogBox;                      // Toolbox object for holding catalog information.
Toolbox readBox;                         // Toolbox object for holding elements from read/comparison function.
Toolbox tempBox;                         // Toolbox object for holding temporary data.
Toolbox *ptrBox = &catalogBox;           // Pointer toolbox.
Toolbox viewBox;
RFID_Tag tempTag;                        // RFID tag object for collecting and parsing data.
ScreenSelector selector;                 // Used for managing user input and screen selection.
ScreenSelector selected;                 // Used for managing user input and screen selection.
void(*resetFunc)(void) = 0;              // Software reset of Arduino module, used if RFID reader connection fails.

void setup() {

  Serial.begin(115200);                  // Hardware serial used for debugging and testing.

  while (!Serial);                       //Wait for the serial port to come online

  if (setupNano(38400) == false)         //Configure nano to run at 38400bps
  {
    Serial.println(F("Module failed to respond. Please check wiring."));  // Test: Verify if module failed to respond correcctly.
    resetFunc();
    //while (1);                          //Freeze!
  }

  nano.setRegion(REGION_NORTHAMERICA);    //Set to North America
  nano.setReadPower(2700);                 //Set read power 500= 5.00dBm

  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);
  digitalWrite(BUZZER2, LOW);

  // Setup of timer registers and interrupt to occur every one second.
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();

  mainScreen.initScreen();                // Initialize mainScreen object.
  listScreen.initScreen();                // Initialize listScreen object.
  mainScreen.drawScreen();                // Draw mainScreen.
  

  selector = start;                       // Set selector to initial position.
  selected = none;                        // Set selected to initial position.
  updateBattery(getBatteryPercentage());  // Initial battery percentage on screen

}

void loop() {

// With no user input 
  while (selector == start) {
    selected =  mainScreen.touchResponse();               // MainScreen touch response determines the selected value.

    switch (selected) {                                   // Check value of selected.
      
      case logScan:                                       // User selected log button.
        //Serial.println("I'm here 1");                     // @breakpoint

        // Clear the current catalogbox info for new information
        if (!catalogBox.isEmpty()) {
          //Serial.println("I'm here 1.2");                 // @breakpoint
          catalogBox.clearBox();
          delay(50);  
        }

        readTags();
        ptrBox = &catalogBox;                             
        EEPROM.put(eeAddress, catalogBox);                // Write catalogbox info to storage
        listScreen.setListType(catalog);                  // Set the list type to catalog scheme.
        listScreen.setListSize(catalogBox.getLength());   // Set the amount of data to be displayed with the length of the toolbox.
        listScreen.firstPage();                           // Draw the first page for the toolbox list.
        listScreen.displayTagInfo(catalogBox);            // Display the information in the catalog toolbox.
        selector = logScan;                               // Selector value to logScan
        updateBattery(getBatteryPercentage());            // Draw current battey info on new page
        break;

      case readScan:                                      // User selected check button.
        //Serial.println("I'm here 2");                     // @breakpoint

        // Clear information from tempXox and readBox, if anything in tempbox
        if (!tempBox.isEmpty()) {
          //Serial.println("I'm here 1.2");                 // @breakpoint
          tempBox.clearBox();                             // Clear comparison box before reading.
          readBox.clearBox();                             // Clear results holder before comparison.
          delay(50);
        }

        readTags();
        
        if ((catalogBox.compare(tempBox, readBox))) {     // Compare catalog box to currently read info, if missing tags.
          //Serial.println("I'm here 3");                  // @breakpoint
          ptrBox = &readBox;
          listScreen.setListType(inventory);              // Set the list type to inventory scheme.
          listScreen.setListSize(readBox.getLength());    // Set the amount of data to be displayed with the length of the toolbox.
          listScreen.firstPage();                         // Draw the first page for the toolbox list.
          listScreen.displayTagInfo(readBox);             // Display the information in the read toolbox.
          selector = readScan;                            // Selector value to readScan
          updateBattery(getBatteryPercentage());          // Draw current battey info on new page
        }
        else {                                            // All tags present
          //Serial.println("I'm here 3.5");               // @breakpoint
          mainScreen.drawScreen();
          updateBattery(getBatteryPercentage());          // Draw current battey info on new page
        }
        break;

      case view:                                          // User selected view button
        //Serial.println("I'm here 4");                     // @breakpoint
        EEPROM.get(eeAddress, viewBox);                   // Get saved data from storage
        listScreen.setListType(catalog);                  // Set the list type to catalog scheme.
        listScreen.setListSize(viewBox.getLength());      // Set the amount of data to be displayed with the length of the toolbox.
        listScreen.firstPage();                           // Draw the first page for the toolbox list.
        listScreen.displayTagInfo(viewBox);               // Display the information in the catalog toolbox.
        selector = logScan;                               // selector value to logScan.
        updateBattery(getBatteryPercentage());            // Draw current battey info on new page
        break;

      case settings:                                      // Reset button
        //TODO: Create settings screen and possible added functionality.
        
        for (int i = 0 ; i < viewBox.getLength() ; i++) {
          EEPROM.write(i, 0);
        }
        selector = start;
        selected = mainScreen.touchResponse();
        resetFunc();

        break;
      case previous:
        break;
      case next:
        break;
      case back:
        break;
      case start:
        break;
      case none:
        break;
    }
  }

  // When user is in log or read screens
  while ((selector == logScan) || (selector == readScan)) {  // User chose log, read, or view on mainScreen
    selected = listScreen.touchResponse();                   // The selected value is now determined by the list screen touch repsonse.

    switch (selected) {
      case logScan:
        break;
      case readScan:
        break;
      case view:
        break;
      case settings:
        break;
      case previous:                                          // User selected left (previous page) button.
        //Serial.println("I'm here 5");                       // @breakpoint
        if (listScreen.getCurrentPage() == 1) {               // Check if this is the first page of the list screen.
          //Serial.println("I'm here 6");                     // @breakpoint
          mainScreen.drawScreen();                            // Go back to the main screen.
          selector = start;                                   // Return the selector value to start.
        }
        else {                                                // If not the first page.
          //Serial.println("I'm here 7");                     // @breakpoint
          listScreen.previousPage();                          // Go to the previous page of the list screen.
          updateBattery(getBatteryPercentage());              // Draw current battey info on new page
          listScreen.displayTagInfo(*ptrBox);                 // Display the information from the toolbox.
        }
        break;

      case next:                                              // User selected left (next page) button.
        //Serial.println("I'm here 8");                        // @breakpoint
        // Check if it is not currently on the last page.
        if (listScreen.getCurrentPage() != listScreen.getNumOfPages()) {  
          listScreen.nextPage();                              // Display the next page
          updateBattery(getBatteryPercentage());              // Draw current battey info on new 
          listScreen.displayTagInfo(*ptrBox);                 // Populate page tag info
        }
        break;

      case back:                                              // User selected upper left (back) button
        //Serial.println("I'm here 9");                         // @breakpoint
        selector = start;                                     // Place selector into initial position.
        mainScreen.drawScreen();                              // Draw the main screen
        updateBattery(getBatteryPercentage());                // Draw current battey info on new page
        break;

      case start:
        break;
      case none:
        break;
      default:
        break;
    }
  }
}

//Gracefully handles a reader that is already configured and already reading continuously
boolean setupNano(long baudRate) {
  nano.begin(softSerial);                 // Tell the library to communicate over software serial port

  nano.enableDebugging();                 // Enable debugging for sent and returned messages

  //Test to see if we are already connected to a module
  softSerial.begin(baudRate);             // For this test, assume module is already at our desired baud rate
  while (!softSerial);                    // Wait for port to open

  //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
  while (softSerial.available()) softSerial.read();

  nano.getVersion();

  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE)
  {
    //This happens if the baud rate is correct but the module is doing a ccontinuous read
    nano.stopReading();

    //Serial.println(F("Module continuously reading. Asking it to stop..."));

    delay(1500);
  }
  else
  {
    //The module did not respond so assume it's just been powered on and communicating at 115200bps
    softSerial.begin(115200);               //Start software serial at 115200

    nano.setBaud(baudRate);                 //Tell the module to go to the chosen baud rate. Ignore the response msg

    softSerial.begin(baudRate);             //Start the software serial port, this time at user's chosen baud rate
  }

  //Test the connection
  nano.getVersion();
  if (nano.msg[0] != ALL_GOOD) return (false); //Something is not right

  //The M6E has these settings no matter what
  nano.setTagProtocol(); //Set protocol to GEN2

  nano.setAntennaPort(); //Set TX/RX antenna ports to 1

  return (true); //We are ready to rock
}

// Interrupt routine for timer
ISR(TIMER1_COMPA_vect) {//timer1 interrupt 1Hz
  //Serial.print(counter);Serial.println("s");                // Test: Verify counter
  counter++;
  if (counter == 12) {
    counter = 0;                                              // Reset counter
  }
}

// Get tag information for the catalog toolbox.
void getTagInfoCatalog() {
  if (nano.check() == true)                   //Check to see if any new data has come in from module
  {
    byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp

    if (responseType == RESPONSE_IS_KEEPALIVE)
    {
      Serial.println(F("Scanning"));
    }
    else if (responseType == RESPONSE_IS_TAGFOUND) {

      //If we have a full record we can pull out the fun bits, for more detailed operations
      //rssi = nano.getTagRSSI();             //Get the RSSI for this tag read

      //freq = nano.getTagFreq();             //Get the frequency this tag was detected at

      //timeStamp = nano.getTagTimestamp();   //Get the time this was read, (ms) since last keep-alive message

      tagEPCBytes = nano.getTagEPCBytes();    //Get the number of bytes of EPC from response

      tagDataBytes = nano.getTagDataBytes();

      delay(100);                             // Data propogation delay prior to writing
      for (byte b = 0 ; b < tagEPCBytes ; b++) {
        tempTag.EPC[b] = nano.msg[31 + tagDataBytes + b];   // Gather tag data into Tag data structure
      }

      //tempTag.rssi = rssi;
      //tempTag.frequency = freq;
      //tempTag.timeStamp = timeStamp;
      delay(100);
      catalogBox.add(tempTag);              // Add to toolbox
    }
    else {
      //Unknown response
      nano.printMessageArray();                //Print the response message. Look up errors in tmr__status_8h.html
    }
  }
}

// Get tag information for the temp comparison toolbox.
void getTagInfoInventory() {
  if (nano.check() == true)                   //Check to see if any new data has come in from module
  {
    byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp

    if (responseType == RESPONSE_IS_KEEPALIVE)
    {
      //Serial.println(F("Scanning"));
    }
    else if (responseType == RESPONSE_IS_TAGFOUND) {

      //If we have a full record we can pull out the fun bits
      //rssi = nano.getTagRSSI();             //Get the RSSI for this tag read

      //freq = nano.getTagFreq();             //Get the frequency this tag was detected at

      //timeStamp = nano.getTagTimestamp();   //Get the time this was read, (ms) since last keep-alive message

      tagEPCBytes = nano.getTagEPCBytes();    //Get the number of bytes of EPC from response

      tagDataBytes = nano.getTagDataBytes();

      delay(100);
      for (byte b = 0 ; b < tagEPCBytes ; b++) {
        tempTag.EPC[b] = nano.msg[31 + tagDataBytes + b];
      }

      //tempTag.rssi = rssi;
      //tempTag.frequency = freq;
      //tempTag.timeStamp = timeStamp;
      delay(100);
      tempBox.add(tempTag);
    }
    else {
      //Unknown response
      nano.printMessageArray();                //Print the response message. Look up errors in tmr__status_8h.html
    }
  }
}

// Read tag information based on user selected operation.
void readTags() {
  nano.startReading();
  delay(50);  // Delay to allow tag reading to start.
  counter = 0;
  do {
    switch (selected) {
      // Creating a new catalog
      case logScan:
        //Serial.println("I'm here 10");               // @breakpoint
        getTagInfoCatalog();
        break;
      // Attempting a comparision
      case readScan:
        //Serial.println("I'm here 11");              // @breakpoint
        getTagInfoInventory();
        break;
      default:
        break;
    }

  } while (counter < 3);
  delay(50);
  Serial.println("I'm here 12");                     // @breakpoint
  nano.stopReading();
  setupNano(38400);
}

// Control battery indicator based on screen and analog input.
void updateBattery(int percentage) {
  //Serial.println("Update");
  switch (selector) {
    case start:
      if (percentage > 75) {
        mainScreen.batteryIndicatorBars(4, MAIN_LETTERCOLOR, MAIN_BG_COLOR);
      }
      else if (percentage < 75 && percentage > 50) {
        mainScreen.batteryIndicatorBars(3, MAIN_LETTERCOLOR, MAIN_BG_COLOR);
      }
      else if (percentage < 50 && percentage > 25) {
        mainScreen.batteryIndicatorBars(2, MAIN_LETTERCOLOR, MAIN_BG_COLOR);
      }
      else if (percentage < 25 && percentage > 10) {
        mainScreen.batteryIndicatorBars(1, MAIN_LETTERCOLOR, MAIN_BG_COLOR);
      }
      else if (percentage < 0) {
        mainScreen.batteryIndicatorBars(0, MAIN_LETTERCOLOR, MAIN_BG_COLOR);
      }
      else {
        mainScreen.batteryIndicatorBars(0, MAIN_LETTERCOLOR, MAIN_BG_COLOR);
      }
      break;
      
    case logScan:
      if (percentage > 75) {
        mainScreen.batteryIndicatorBars(4, LIST_LETTER_COLOR, GREEN);
      }
      else if (percentage < 75 && percentage > 50) {
        mainScreen.batteryIndicatorBars(3, LIST_LETTER_COLOR, GREEN);
      }
      else if (percentage < 50 && percentage > 25) {
        mainScreen.batteryIndicatorBars(2, LIST_LETTER_COLOR, GREEN);
      }
      else if (percentage < 25 && percentage > 10) {
        mainScreen.batteryIndicatorBars(1, LIST_LETTER_COLOR, GREEN);
      }
      else if (percentage < 0) {
        mainScreen.batteryIndicatorBars(0, LIST_LETTER_COLOR, GREEN);
      }
      else {
        mainScreen.batteryIndicatorBars(0, LIST_LETTER_COLOR, GREEN);
      }
      break;
    case readScan:
      if (percentage > 75) {
        mainScreen.batteryIndicatorBars(4, LIST_LETTER_COLOR, LIGHTRED);
      }
      else if (percentage < 75 && percentage > 50) {
        mainScreen.batteryIndicatorBars(3, LIST_LETTER_COLOR, LIGHTRED);
      }
      else if (percentage < 50 && percentage > 25) {
        mainScreen.batteryIndicatorBars(2, LIST_LETTER_COLOR, LIGHTRED);
      }
      else if (percentage < 25 && percentage > 10) {
        mainScreen.batteryIndicatorBars(1, LIST_LETTER_COLOR, LIGHTRED);
      }
      else if (percentage < 0) {
        mainScreen.batteryIndicatorBars(0, LIST_LETTER_COLOR, LIGHTRED);
      }
      else {
        mainScreen.batteryIndicatorBars(0, LIST_LETTER_COLOR, LIGHTRED);
      }
      break;
    case none:
      if (percentage > 75) {
        mainScreen.batteryIndicatorBars(4, MAIN_LETTERCOLOR, MAIN_BG_COLOR);
      }
      else if (percentage < 75 && percentage > 50) {
        mainScreen.batteryIndicatorBars(3, MAIN_LETTERCOLOR, MAIN_BG_COLOR);
      }
      else if (percentage < 50 && percentage > 25) {
        mainScreen.batteryIndicatorBars(2, MAIN_LETTERCOLOR, MAIN_BG_COLOR);
      }
      else if (percentage < 25 && percentage > 10) {
        mainScreen.batteryIndicatorBars(1, MAIN_LETTERCOLOR, MAIN_BG_COLOR);
      }
      else if (percentage < 0) {
        mainScreen.batteryIndicatorBars(0, MAIN_LETTERCOLOR, MAIN_BG_COLOR);
      }
      else {
        mainScreen.batteryIndicatorBars(0, MAIN_LETTERCOLOR, MAIN_BG_COLOR);
      }
      break;
    default:
      if (percentage > 75) {
        mainScreen.batteryIndicatorBars(4, LIST_LETTER_COLOR, GREEN);
      }
      else if (percentage < 75 && percentage > 50) {
        mainScreen.batteryIndicatorBars(3, LIST_LETTER_COLOR, GREEN);
      }
      else if (percentage < 50 && percentage > 25) {
        mainScreen.batteryIndicatorBars(2, LIST_LETTER_COLOR, GREEN);
      }
      else if (percentage < 25 && percentage > 10) {
        mainScreen.batteryIndicatorBars(1, LIST_LETTER_COLOR, GREEN);
      }
      else if (percentage < 0) {
        mainScreen.batteryIndicatorBars(0, LIST_LETTER_COLOR, GREEN);
      }
      else {
        mainScreen.batteryIndicatorBars(0, LIST_LETTER_COLOR, GREEN);
      }
      break;
  }
}

int getBatteryPercentage() {
  int sensorValue =  analogRead(A7);    // anolog pin connected to battery
  float voltage = sensorValue * (4.98 / 1023.00) * 2; //Convert into voltage value

  int batteryPercentage;
  //Convert to battery percentage based on voltage value
  if (voltage <= 7.20 && voltage >= 5.41) {
    batteryPercentage = (voltage - 5.40) / 1.8 * 100;
  }
  else if (voltage < 5.41) {
    batteryPercentage = 0;
  }
  else if (voltage >= 7.20) {
    batteryPercentage = 100;
  }

  return batteryPercentage;     // Return percentage value
}




