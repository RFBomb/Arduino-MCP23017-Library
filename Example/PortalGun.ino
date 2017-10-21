#include <Arduino.h>
#include <Wire.h>
#include <unistd.h>
#include "SevenSegmentDisplay_MCP23017.h"
#include "I2C_Input_MCP23017.h"

SevenSegmentDisplay SSD;
I2C_Input_MCP23017 INP;

///Setup Variables
static int LiveLED = 13;	/* On-Board LED to blink to denote program running */
static int DisplayLoopNo = 20; /* Number of times to loop the display with current settings before checking for a different input */
static int StayOnTime = 4; /* How long each display should stay on when multiplexing (in milliseconds). */
static int TurnOffTime = 0; /*Sets the delay for how long the outputs should be off before switching to the next digit (in milliseconds). */
int NewInputNo;		/* Input Number that to then decide which characters to display */
int OldInputNo =99;		/* Previous	Input Number that decide which characters to display */


void setup()
{
	/// Initialize Comm Links
	Wire.setClock(400000);	/// Baud rate for I2C
	Serial.begin(9600);		/// Com
	/// Setup Display Chip IO
	SSD.SetupSegmentGPIO(true, 0x20);
	SSD.UpdateTimerVariables(TurnOffTime, StayOnTime);
	/// Setup On-Board LED
	pinMode(LiveLED, OUTPUT);
	digitalWrite(LiveLED, HIGH);
	/// Set Input Chip
	INP.SetupChipIO(0x21);
	NewInputNo = INP.GetFirstIO();
	Serial.println("Initialization Complete. Begin Main Loop....");
}

void loop()
{  
	//SetAllDigits("L", "O", "V", "E", false, false, false, false);
	//DisplayLoop();
	//SetAllDigits("Y", "O", "U", "!", false, false, false, false);
	CheckInputs();
	DisplayLoop();

}


void SetAllDigits(String Digit0, String Digit1, String Digit2, String Digit3, bool Dec0, bool Dec1, bool Dec2, bool Dec3)
{
	SSD.SetDisplayOutput(Digit0, 0, Dec0);
	SSD.SetDisplayOutput(Digit1, 1, Dec1);
	SSD.SetDisplayOutput(Digit2, 2, Dec2);
	SSD.SetDisplayOutput(Digit3, 3, Dec3);
	
}

void DisplayLoop(){
  for (int x=0; x<DisplayLoopNo; x++) // Set starting X value ; X is less than DisplayLoopNo; Increment X
  {SSD.LoopThroughOptionalDisplays(true,true,true,true,false,false,false,false);}
  }

void CheckInputs() {
	Serial.println("Checking for new input..................");
	Serial.println(OldInputNo);
	NewInputNo = INP.GetFirstIO();
	Serial.println(NewInputNo);
	if (NewInputNo != OldInputNo)
	{switch (NewInputNo) {
		case 1:	SetAllDigits("0", "A", "6", "0", false, false, false, false); break;
		case 2:	SetAllDigits("0", "P", "6", "9", false, false, false, false); break;
		case 3:		SetAllDigits("0", "4", "5", "C", false, false, false, false); break;
		case 4:		SetAllDigits("J", "C", "9", "4", false, false, false, false); break;
		default: 
			SetAllDigits("H", "V", "1", "7", false, false, false, false);
			break;
		}
	}
	OldInputNo = NewInputNo;
}


