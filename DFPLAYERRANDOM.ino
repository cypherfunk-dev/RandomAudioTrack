#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

#if (defined(ARDUINO_AVR_UNO) || defined(ESP8266))
#include <SoftwareSerial.h>
SoftwareSerial softSerial(/*rx =*/4, /*tx =*/5);
#define FPSerial softSerial
#else
#define FPSerial Serial1
#endif

DFRobotDFPlayerMini myDFPlayer;
const int buttonPin = D3; // Define the button pin
int totalFiles; // Store the total number of MP3 files
void printDetail(uint8_t type, int value);

void setup()
{
#if (defined ESP32)
FPSerial.begin(9600, SERIAL_8N1, /*rx =*/D1, /*tx =*/D2);
#else
FPSerial.begin(9600);
#endif

Serial.begin(115200);

Serial.println();
Serial.println(F("DFRobot DFPlayer Mini Demo"));
Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

if (!myDFPlayer.begin(FPSerial, /*isACK = */true, /*doReset = */true)) {
Serial.println(F("Unable to begin:"));
Serial.println(F("1.Please recheck the connection!"));
Serial.println(F("2.Please insert the SD card!"));
while(true){
delay(0); // Code to compatible with ESP8266 watch dog.
}
}
Serial.println(F("DFPlayer Mini online."));

myDFPlayer.volume(30); //Set volume value. From 0 to 30
// Get the total number of mp3 files on the SD card
totalFiles = 22;

pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with pull-up resistor
}
void loop()
{
static unsigned long timer = millis();
if (myDFPlayer.available()) {
printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
}


  // Check if button is pressed
if (!digitalRead(buttonPin)) {
// Play a random track
int randomTrack = random(1, totalFiles + 1); // Generate a random number between 1 and total number of files
myDFPlayer.play(randomTrack);
delay(3000);
}
}

void printDetail(uint8_t type, int value){
 switch (type) {
  case TimeOut:
   Serial.println(F("Time Out!"));
   break;
  case WrongStack:
   Serial.println(F("Stack Wrong!"));
   break;
  case DFPlayerCardInserted:
   Serial.println(F("Card Inserted!"));
   break;
  case DFPlayerCardRemoved:
   Serial.println(F("Card Removed!"));
   break;
  case DFPlayerCardOnline:
   Serial.println(F("Card Online!"));
   break;
  case DFPlayerUSBInserted:
   Serial.println("USB Inserted!");
   break;
  case DFPlayerUSBRemoved:
   Serial.println("USB Removed!");
   break;
  case DFPlayerPlayFinished:
   Serial.print(F("Number:"));
   Serial.print(value);
   Serial.println(F(" Play Finished!"));
   break;
  case DFPlayerError:
   Serial.print(F("DFPlayerError:"));
   switch (value) {
    case Busy:
     Serial.println(F("Card not found"));
     break;
    case Sleeping:
     Serial.println(F("Sleeping"));
     break;
    case SerialWrongStack:
     Serial.println(F("Get Wrong Stack"));
     break;
    case CheckSumNotMatch:
     Serial.println(F("Check Sum Not Match"));
     break;
    case FileIndexOut:
     Serial.println(F("File Index Out of Bound"));
     break;
    case FileMismatch:
     Serial.println(F("Cannot Find File"));
     break;
    case Advertise:
     Serial.println(F("In Advertise"));
     break;
    default:
     break;
   }
   break;
  default:
   break;
 }

}
