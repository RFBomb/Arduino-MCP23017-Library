/**-----Multiplexer 7-Segment Display  -------------------------------------
***	Written by Robert Brenckman 8/17/17
***	Feel free to use this class for an easy implementation of a 7-segment display via an MCP23017!!
*** My project originally had a 4-digit display and a single-digit display. This was used to multiplex them.
*** This text must be included in any implementation.
**/

#include "SevenSegmentDisplay_MCP23017.h"

/*	Pin Numbers: 0 - 15, where 0 - 7 is Port A and 8 - 15 is Port B 
* Port B = pins 1-8, Port A = pins 21-28
*/
Adafruit_MCP23017 MCP_Chip;


uint8_t ChipAddress;  /* I2C address for this chip. */
bool CommonAnode = true;  /* Set this value as True/False depending on the type of 7-Segment Display */
int TimeDelay = 100;  /* Time delay in milliseconds before returning to the original call location (so display can update) */
int SequentialTimeDelay = 500; /*Time Delay for displaying an ellipses (....) across the display if the display has no information */
int TurnOffTimeDelay = 10; /*Time to allow the pi to rest after turning all the LEDs off on the display (within the ActivateDisplay Subroutine) */

/* GPIO Values are stored in a string, then concatenated + converted into a 16-digit number uint16_t and sent to the MCP23017 chip */
uint8_t GPIOA;  /*8-bit register for setting the values to GPIOA */
uint8_t GPIOB;  /*8-bit register for setting the values to GPIOB */
uint16_t GPIOBits=0000000000000000;

///----- Variables -- Used when multiplexing the display automagically
String Display0_Char;
String Display1_Char;
String Display2_Char;
String Display3_Char;
String Display4_Char;
String Display5_Char;
String Display6_Char;
String Display7_Char;
//Boolean Variables to set if the decimal point is lit for a given display
bool DecimalPt_Disp0;
bool DecimalPt_Disp1;
bool DecimalPt_Disp2;
bool DecimalPt_Disp3;
bool DecimalPt_Disp4;
bool DecimalPt_Disp5;
bool DecimalPt_Disp6;
bool DecimalPt_Disp7;
bool DisplayCleared = true; //Decimal Point will blink to signal display is active, but has no information.

//----- GPIO identifiers that power the different segments of the displays
//	Pin Numbers: 0 - 15, where 0 - 7 is Port A and 8 - 15 is Port B
//	Port A = pins 21 - 28, Port B = pins 1 - 8 (respectively)
const int Segment_A = 8;
const int Segment_B = 9;
const int Segment_C = 10;
const int Segment_D = 11;
const int Segment_E = 12;
const int Segment_F = 13;
const int Segment_G = 14;
const int Segment_H = 15;

//----- GPIO identifiers that power the different digits of the displays
const int Display0Out = 0;		//Used to set first digit -- 'Display 0'
const int Display1Out = 1;		//Used to set second digit -- 'Display 1'
const int Display2Out = 2;		//Used to set third digit -- 'Display 2'
const int Display3Out = 3;		//Used to set fourth digit -- 'Display 3'

//----- Currently Unused Pins - available for additional multiplexing
const int Display4Out = 4;		//digit 7-segment -- 'Display 4'
const int Display5Out = 5;		//digit 7-segment -- 'Display 5'
const int Display6Out = 6;		//digit 7-segment -- 'Display 6'
const int Display7Out = 7;		//digit 7-segment -- 'Display 7'

///----------Public Stuff---------------------------------------------------------

	//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
	//'''''''   Code for user to use in main program to change display states / initialize   '''''''''''''''''''''
	//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

	/* Initialize the GPIO Pins as outputs. See Code for which pins to utilize.
	* Common Anode: Select if this chip is using a common Anode display or common kathode. 
	* ChipAddress: This is the I2C address for the display chip.
	*/
void SevenSegmentDisplay::SetupSegmentGPIO(bool Anode, uint8_t ChipAddress_ )
	{
	ChipAddress = ChipAddress_;
    /// Setup Chip Communication
    Serial.print("Attempting 7 Segment Display Chip Communication.... \n");
	MCP_Chip.begin(ChipAddress);
    byte error = Wire.endTransmission();
    if (error == 0){Serial.print("Display Chip Communication Established!\n");}
    else{Serial.print("Display Chip Communication Failed :[ \n");}
          
    /// Setup the IO on the chip
		CommonAnode = Anode;		
		SetupGPIO_Outputs(Segment_A);
		SetupGPIO_Outputs(Segment_B);
		SetupGPIO_Outputs(Segment_C);
		SetupGPIO_Outputs(Segment_D);
		SetupGPIO_Outputs(Segment_E);
		SetupGPIO_Outputs(Segment_F);
		SetupGPIO_Outputs(Segment_G);
		SetupGPIO_Outputs(Segment_H);
		SetupGPIO_Outputs(Display0Out);
		SetupGPIO_Outputs(Display1Out);
		SetupGPIO_Outputs(Display2Out);
		SetupGPIO_Outputs(Display3Out);
		SetupGPIO_Outputs(Display4Out);
		SetupGPIO_Outputs(Display5Out);
		SetupGPIO_Outputs(Display6Out);
		SetupGPIO_Outputs(Display7Out);
		ClearDisplay();
	}

/// <summary>
/// Clears the Displays of all digits. Activates the Decimal Point on all displays. 
/// </summary>
void SevenSegmentDisplay::ClearDisplay()
{
	Display0_Char = 99;
	Display1_Char = 99;
	Display1_Char = 99;
	Display3_Char = 99;
	Display4_Char = 99;
	Display5_Char = 99;
	Display6_Char = 99;
	Display7_Char = 99;
	DisplayCleared = true;
}

/*
*	CharacterToDisplay: Select a character to display. Single Characters only. 
		Numbers: MUST BE 0-9. Less than 0/Greater than 9 not accepted. 
		Available Letters: A, B, D, E, F, G, H, J, L, O, P, R, S, U, V, Y.  " - " is also accepted.

*	DisplayNo: Set which digit location to display this in. (In the number '73321' the '2' is DisplayNO number "3".)
*	DecimalPt: Set if you wish to have the decimal appear or not.
*/
void SevenSegmentDisplay::SetDisplayOutput(String CharacterToDisplay, int DisplayNo, bool DecimalPt)
{
	DisplayCleared = false;
	if (DisplayNo == 0)
	{
		Display0_Char = CharacterToDisplay;
		DecimalPt_Disp0 = DecimalPt;
	}
	if (DisplayNo == 1)
	{
		Display1_Char = CharacterToDisplay;
		DecimalPt_Disp1 = DecimalPt;
	}
	if (DisplayNo == 2)
	{
		Display2_Char = CharacterToDisplay;
		DecimalPt_Disp2 = DecimalPt;
	}
	if (DisplayNo == 3)
	{
		Display3_Char = CharacterToDisplay;
		DecimalPt_Disp3 = DecimalPt;
	}
	if (DisplayNo == 4)
	{
		Display4_Char = CharacterToDisplay;
		DecimalPt_Disp4 = DecimalPt;
	}
	if (DisplayNo == 5)
	{
		Display5_Char = CharacterToDisplay;
		DecimalPt_Disp5 = DecimalPt;
	}
	if (DisplayNo == 6)
	{
		Display6_Char = CharacterToDisplay;
		DecimalPt_Disp6 = DecimalPt;
	}
	if (DisplayNo == 7)
	{
		Display7_Char = CharacterToDisplay;
		DecimalPt_Disp7 = DecimalPt;
	}
}

/// <summary>
/// Background Task that should provide a constant loop, always updating the display with the most current settings.
/// </summary>
void SevenSegmentDisplay::LoopThroughDisplays()
{
  Serial.print("\n Setting display to:   ");
	DisplayChar(Display0_Char, 0, DecimalPt_Disp0);
	DisplayChar(Display1_Char, 1, DecimalPt_Disp1);
	DisplayChar(Display2_Char, 2, DecimalPt_Disp2);
	DisplayChar(Display3_Char, 3, DecimalPt_Disp3);
	DisplayChar(Display4_Char, 4, DecimalPt_Disp4);
	DisplayChar(Display5_Char, 5, DecimalPt_Disp5);
	DisplayChar(Display6_Char, 6, DecimalPt_Disp6);
	DisplayChar(Display7_Char, 7, DecimalPt_Disp7);
	Serial.print("\n");
}

void SevenSegmentDisplay::LoopThroughOptionalDisplays(bool D0, bool D1, bool D2,  bool D3,  bool D4,  bool D5,  bool D6,  bool D7) 
{
	if (D0) { DisplayChar(Display0_Char, 0, DecimalPt_Disp0); };
	if (D1) { DisplayChar(Display1_Char, 1, DecimalPt_Disp1); };
	if (D2) { DisplayChar(Display2_Char, 2, DecimalPt_Disp2); };
	if (D3) { DisplayChar(Display3_Char, 3, DecimalPt_Disp3); };
	if (D4) { DisplayChar(Display4_Char, 4, DecimalPt_Disp4); };
	if (D5) { DisplayChar(Display5_Char, 5, DecimalPt_Disp5); };
	if (D6) { DisplayChar(Display6_Char, 6, DecimalPt_Disp6); };
	if (D7) { DisplayChar(Display7_Char, 7, DecimalPt_Disp7); };
}

/// <summary>
/// Sets the delay for how long the outputs should be off before switching to the next digit (in milliseconds).
/// Sets how long each digit should be displayed for. 
/// </summary>
void SevenSegmentDisplay::UpdateTimerVariables(int TimeOffDelay, int StayOnTime)
{
	TurnOffTimeDelay = TimeOffDelay;
	TimeDelay = StayOnTime;
}

///----------Private Stuff---------------------------------------------------------

//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//''''''''  All code below sets the outputs of the display. (Supporting Code)   ''''''''''''''''''''''''''''''
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

//Sets up the GPIO pins as outputs. Sets them to the 'low' state.
void SevenSegmentDisplay::SetupGPIO_Outputs(int GPIO_Output)
{
	MCP_Chip.pinMode(GPIO_Output, OUTPUT);
	MCP_Chip.digitalWrite(GPIO_Output, LOW);
}

//-----This Section toggles the state of the Output Pins
void SevenSegmentDisplay::SetOutputHigh(int GPIO_Output)
{	
  MCP_Chip.digitalWrite(GPIO_Output, HIGH);
}

void SevenSegmentDisplay::SetOutputLow(int GPIO_Output)
{
	MCP_Chip.digitalWrite(GPIO_Output, LOW);
}


/// <summary>
/// Task to update the display
/// </summary>
void SevenSegmentDisplay::DisplayChar(String CharacterToDisplay, int DisplayNo, bool DecimalPt)
{
	//Turns off all segments + displays
	if (CommonAnode = true) { MCP_Chip.writeGPIOAB(KillDisplay); } else{ MCP_Chip.writeGPIOAB(~KillDisplay); }
  	delay(TurnOffTimeDelay);
	
	//Activate the proper display
	ActivateDisplay(DisplayNo);
	//Pick which segments to activate
	DisplayNumber(CharacterToDisplay);
	DisplayLetter(CharacterToDisplay);
	DisplaySpecialCase(CharacterToDisplay);
	//Send the bits to the chip to update the display
	UpdateDisplay();
	if (DecimalPt == true || DisplayCleared == true) { TurnOnSegment_H(); }

	if (DisplayCleared == true)	{	delay(SequentialTimeDelay);} // hold time if only displaying elipses
	else	{	delay(TimeDelay);	} // hold time for multiplexed display
}

void SevenSegmentDisplay::Test(String Char_, int DisplayNo, bool DecimalPt)
{
  DisplayCleared = false;
  DisplayChar(Char_, DisplayNo, DecimalPt);
  uint16_t Readout = MCP_Chip.readGPIOAB();
  uint8_t Readout2 = MCP_Chip.readGPIO(0);
  uint8_t Readout3 = MCP_Chip.readGPIO(1);
  Serial.println("----------------------------");
  Serial.print("Character: "); Serial.println(Char_);
  Serial.print("Display #: "); Serial.println(DisplayNo);
  Serial.print ("16 Bit Value:      "); Serial.println(Readout,BIN);
  Serial.print ("Expected:          "); Serial.println(GPIOBits);
  Serial.print ("8 Bit GPIOA Value:         "); Serial.println(Readout2,BIN);
  Serial.print ("Expected: "); Serial.println(GPIOA);
  Serial.print ("8 Bit GPIOB Value: "); Serial.println(Readout3,BIN);
  Serial.print ("Expected: "); Serial.println(GPIOB);
}

/*Combines the GPIOA and GPIOB statements into GPIOBits. THen passes it through to the chip.*/
void SevenSegmentDisplay::UpdateDisplay()
{
	// Create GPIOBits from GPIOB+GPIOA
	GPIOBits = word((byte(GPIOB) * 256) + byte(GPIOA));
	MCP_Chip.writeGPIOAB(GPIOBits);
}

//-----Pick which display to use
void SevenSegmentDisplay::ActivateDisplay(int DisplayNumber)
{
	//Turns Power on/off depending on if multiplexing or not. Also sets which digit to use if multiplexing.
	//The logic for this is opposite the logic for the segments because this is the COMMON
	if		(DisplayNumber == 0){TurnOnDisplay0();}
	else if (DisplayNumber == 1){TurnOnDisplay1();}
	else if (DisplayNumber == 2){TurnOnDisplay2();}
	else if (DisplayNumber == 3){TurnOnDisplay3();}
	else if (DisplayNumber == 4){TurnOnDisplay4();}
	else if (DisplayNumber == 5){TurnOnDisplay5();}
	else if (DisplayNumber == 6){TurnOnDisplay6();}
	else if (DisplayNumber == 7){TurnOnDisplay7();}
}

//Turns the outputs for each segment off.
void SevenSegmentDisplay::TurnAllSegmentsoff()
{
	/*
	if (CommonAnode == true)
	{
		SetOutputHigh(Segment_A);
		SetOutputHigh(Segment_B);
		SetOutputHigh(Segment_C);
		SetOutputHigh(Segment_D);
		SetOutputHigh(Segment_E);
		SetOutputHigh(Segment_F);
		SetOutputHigh(Segment_G);
		SetOutputHigh(Segment_H);
   
	}
	else
	{
		SetOutputLow(Segment_A);
		SetOutputLow(Segment_B);
		SetOutputLow(Segment_C);
		SetOutputLow(Segment_D);
		SetOutputLow(Segment_E);
		SetOutputLow(Segment_F);
		SetOutputLow(Segment_G);
		SetOutputLow(Segment_H);
	}
	*/
}

//Turns off all displays based on Common Anode/Cathode
void SevenSegmentDisplay::TurnOffAllDisplays()
{

	/*
	if (CommonAnode == true)
	{
		SetOutputLow(Display0Out);
		SetOutputLow(Display1Out);
		SetOutputLow(Display2Out);
		SetOutputLow(Display3Out);
		SetOutputLow(Display4Out);
		SetOutputLow(Display5Out);
		SetOutputLow(Display6Out);
		SetOutputLow(Display7Out);
	}
	else
	{
		SetOutputHigh(Display0Out);
		SetOutputHigh(Display1Out);
		SetOutputHigh(Display2Out);
		SetOutputHigh(Display3Out);
		SetOutputHigh(Display4Out);
		SetOutputHigh(Display5Out);
		SetOutputHigh(Display6Out);
		SetOutputHigh(Display7Out);
	}
	*/
}

//----- Sets the Segments/Display high / low depending on CommonAnode or not -----
void SevenSegmentDisplay::TurnOnDisplay0()
{
	GPIOA = TurnOnDisp0;
	//if (CommonAnode == true)	{	SetOutputHigh(Display0Out);	}
	//else	{	SetOutputLow(Display0Out);}
}

void SevenSegmentDisplay::TurnOnDisplay1()
{
	GPIOA = TurnOnDisp1;
	//if (CommonAnode == true)	{	SetOutputHigh(Display1Out);	}
	//else	{	SetOutputLow(Display1Out);}
}

void SevenSegmentDisplay::TurnOnDisplay2()
{
	GPIOA = TurnOnDisp2;
	//if (CommonAnode == true)	{	SetOutputHigh(Display2Out);	}
	//else	{	SetOutputLow(Display2Out);}
}

void SevenSegmentDisplay::TurnOnDisplay3()
{
	GPIOA = TurnOnDisp3;
	//if (CommonAnode == true)	{	SetOutputHigh(Display3Out);	}
	//else	{	SetOutputLow(Display3Out);}
}

void SevenSegmentDisplay::TurnOnDisplay4()
{
	GPIOA = TurnOnDisp4;
	//if (CommonAnode == true)	{	SetOutputHigh(Display4Out);	}
	//else	{	SetOutputLow(Display4Out);}
}

void SevenSegmentDisplay::TurnOnDisplay5()
{
	GPIOA = TurnOnDisp5;
	//if (CommonAnode == true)	{	SetOutputHigh(Display5Out);	}
	//else	{	SetOutputLow(Display5Out);}
}

void SevenSegmentDisplay::TurnOnDisplay6()
{
	GPIOA = TurnOnDisp6;
	//if (CommonAnode == true)	{	SetOutputHigh(Display6Out);	}
	//else	{	SetOutputLow(Display6Out);}
}

void SevenSegmentDisplay::TurnOnDisplay7()
{
	GPIOA = TurnOnDisp7;
	//if (CommonAnode == true)	{	SetOutputHigh(Display7Out);	}
	//else	{	SetOutputLow(Display7Out);}
}

void SevenSegmentDisplay::TurnOnSegment_A()
{
	if (CommonAnode == false)	{		SetOutputHigh(Segment_A);	}
	else	{		SetOutputLow(Segment_A);	}
}

void SevenSegmentDisplay::TurnOnSegment_B()
{
	if (CommonAnode == false)	{		SetOutputHigh(Segment_B);	}
	else	{	SetOutputLow(Segment_B);}
}

void SevenSegmentDisplay::TurnOnSegment_C()
{
	if (CommonAnode == false)	{	SetOutputHigh(Segment_C);	}
	else	{	SetOutputLow(Segment_C);}
}

void SevenSegmentDisplay::TurnOnSegment_D()
{
	if (CommonAnode == false)	{	SetOutputHigh(Segment_D);	}
	else	{	SetOutputLow(Segment_D);}
}

void SevenSegmentDisplay::TurnOnSegment_E()
{
	if (CommonAnode == false)	{	SetOutputHigh(Segment_E);	}
	else	{	SetOutputLow(Segment_E);}
}

void SevenSegmentDisplay::TurnOnSegment_F()
{
	if (CommonAnode == false)	{	SetOutputHigh(Segment_F);	}
	else	{	SetOutputLow(Segment_F);}
}

void SevenSegmentDisplay::TurnOnSegment_G()
{
	if (CommonAnode == false)	{	SetOutputHigh(Segment_G);	}
	else	{	SetOutputLow(Segment_G);}
}

void SevenSegmentDisplay::TurnOnSegment_H()
{
	if (CommonAnode == false)	{		SetOutputHigh(Segment_H);	}
	else	{	SetOutputLow(Segment_H);}
}

//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
//''''''''  Code to run everything / decide what is on and off  '''''''''''''''''''''''''''''''''''''''''''''''''
//''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''


	/// <summary>
	/// Sets the specified display to a number from 0-9
	/// </summary>
	/// <param name="NumberToDisplay">MUST BE 0-9. Less than 0/Greater than 9 not accepted. Will not display anything.</param>
	void SevenSegmentDisplay::DisplayNumber(String NumberToDisplay)
	{
		if (NumberToDisplay == "0")	{	SetDisplayTo_0();	}
		if (NumberToDisplay == "1")	{	SetDisplayTo_1();	}
		if (NumberToDisplay == "2")	{	SetDisplayTo_2();	}
		if (NumberToDisplay == "3")	{	SetDisplayTo_3();	}
		if (NumberToDisplay == "4")	{	SetDisplayTo_4();	}
		if (NumberToDisplay == "5")	{	SetDisplayTo_5();	}
		if (NumberToDisplay == "6")	{	SetDisplayTo_6();	}
		if (NumberToDisplay == "7")	{	SetDisplayTo_7();	}
		if (NumberToDisplay == "8")	{	SetDisplayTo_8();	}
		if (NumberToDisplay == "9")	{	SetDisplayTo_9();	}
	}

	/*  Sets the specified display to a specified letter.
  * Available Letters: A, B, D, E, F, G, H, J, L, O, P, R, S, U, V, Y
  */
	void SevenSegmentDisplay::DisplayLetter(String LetterToDisplay)
	{
		if (LetterToDisplay == "A"){  SetDisplayTo_A();	}
		if (LetterToDisplay == "B"){  SetDisplayTo_B();	}
		if (LetterToDisplay == "C"){  SetDisplayTo_C();	}
		if (LetterToDisplay == "D"){  SetDisplayTo_D();	}
		if (LetterToDisplay == "E"){  SetDisplayTo_E();	}
		if (LetterToDisplay == "F"){  SetDisplayTo_F();	}
		if (LetterToDisplay == "G"){  SetDisplayTo_G();	}
		if (LetterToDisplay == "H"){  SetDisplayTo_H();	}
		if (LetterToDisplay == "J"){  SetDisplayTo_J();	}
		if (LetterToDisplay == "L"){  SetDisplayTo_L();	}
		if (LetterToDisplay == "O"){  SetDisplayTo_O();	}
		if (LetterToDisplay == "P"){  SetDisplayTo_P();	}
		if (LetterToDisplay == "R"){  SetDisplayTo_R();	}
		if (LetterToDisplay == "S"){  SetDisplayTo_S();	}
		if (LetterToDisplay == "U"){  SetDisplayTo_U();	}
		if (LetterToDisplay == "V"){  SetDisplayTo_V();	}
    if (LetterToDisplay == "Y"){  SetDisplayTo_Y(); }
	}


  /* Used to display a "Special Case" on the display, such as opposing corners or 3 dashes (segment A+G+D). 
   *  Available Special Cases: 
    * TRBL - Top Right Bottom Left corner
    * TLBR - Top Left Bottom Right corner
    * 3D  - three horizontal dashes (stacked in a single display)
    * AD  - Top and Bottom Dashes
    * BE  - Segments B and E
    * FC  - Segments F and C
    * (-) - This is a dash (segment G)
    * (!) - Exclamation Point (segment B and H)
    */
  void SevenSegmentDisplay::DisplaySpecialCase(String SpecialCase)
  {
	  if (SpecialCase == "TRBL") { GPIOB = SetDispAsTopRightBotLeftCorner; }	//TurnOnSegment_A(); TurnOnSegment_F(); TurnOnSegment_C(); TurnOnSegment_D(); } // top right bottom left
    if (SpecialCase == "TLBR"){ GPIOB = SetDispAsTopLeftBotRightCorner; }	//TurnOnSegment_A();  TurnOnSegment_B();  TurnOnSegment_D();  TurnOnSegment_E();} // Top left bottom right
	if (SpecialCase == "3D") { GPIOB = SetDispAsThreeDashes; } // TurnOnSegment_A();  TurnOnSegment_G();  TurnOnSegment_D();  } // three dashes
    if (SpecialCase == "AD"){ GPIOB = SetDispAsAD; } // TurnOnSegment_A();  TurnOnSegment_D();} // Top and Bottom dashes
    if (SpecialCase == "BE"){ GPIOB = SetDispAsBE; } // TurnOnSegment_B();  TurnOnSegment_E();}
    if (SpecialCase == "FC"){ GPIOB = SetDispAsFC; } // TurnOnSegment_F();  TurnOnSegment_C();}
    if (SpecialCase == "-") { GPIOB = SetDispAsMiddleDash; } // TurnOnSegment_G();}
   if (SpecialCase == "!") { GPIOB = SetDispAsExclamation; } // TurnOnSegment_B(); TurnOnSegment_H(); }
  }
	

		//-----   Number Section  -----
	void SevenSegmentDisplay::SetDisplayTo_0()
	{
		/*
		Serial.print("-0");
		TurnOnSegment_A();
		TurnOnSegment_B();
		TurnOnSegment_C();
		TurnOnSegment_D();
		TurnOnSegment_E();
		TurnOnSegment_F();
		*/
		GPIOB = SetDispAs0;
	}

	void SevenSegmentDisplay::SetDisplayTo_1()
	{
    ///Serial.print("-1");
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
		GPIOB = SetDispAs1;
	}

	void SevenSegmentDisplay::SetDisplayTo_2()
	{
		/*
		///Serial.print("-2");
		TurnOnSegment_A();
		TurnOnSegment_B();
		TurnOnSegment_D();
		TurnOnSegment_E();
		TurnOnSegment_G();
		*/
		GPIOB = SetDispAs2;
	}

	void SevenSegmentDisplay::SetDisplayTo_3()
	{
		///Serial.print("-3");
	//	TurnOnSegment_A();
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_D();
	//	TurnOnSegment_G();
		GPIOB = SetDispAs3;
	}

	void SevenSegmentDisplay::SetDisplayTo_4()
	{
    ///Serial.print("-4");
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAs4;
	}

	void SevenSegmentDisplay::SetDisplayTo_5()
	{
    ///Serial.print("-5");
	//	TurnOnSegment_A();
	//	TurnOnSegment_C();
	//	TurnOnSegment_D();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAs5;
	}
	
	void SevenSegmentDisplay::SetDisplayTo_6()
	{
    ///Serial.print("-6");
	//	TurnOnSegment_A();
	//	TurnOnSegment_C();
	//	TurnOnSegment_D();
	//	TurnOnSegment_E();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAs6;
	}

	void SevenSegmentDisplay::SetDisplayTo_7()
	{
    ///Serial.print("-7");
	//	TurnOnSegment_A();
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
		GPIOB = SetDispAs7;
	}

	void SevenSegmentDisplay::SetDisplayTo_8()
	{
    ///Serial.print("-8");
	//	TurnOnSegment_A();
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_D();
	//	TurnOnSegment_E();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAs8;
	}

	void SevenSegmentDisplay::SetDisplayTo_9()
	{
    ///Serial.print("-9");
	//	TurnOnSegment_A();
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAs9;
	}


	//-----   Alphabet Section  -----
	void SevenSegmentDisplay::SetDisplayTo_A()
	{
    ///Serial.print("-A");
	//	TurnOnSegment_A();
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_E();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAsA;
	}

	void SevenSegmentDisplay::SetDisplayTo_B()
	{
    ///Serial.print("-B");
		SetDisplayTo_8();
	}

	void SevenSegmentDisplay::SetDisplayTo_C()
	{
    ///Serial.print("-C");
	//	TurnOnSegment_A();
	//	TurnOnSegment_D();
	//	TurnOnSegment_E();
	//	TurnOnSegment_F();
		GPIOB = SetDispAsC;
	}

	void SevenSegmentDisplay::SetDisplayTo_D()
	{
    ///Serial.print("-D");
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_D();
	//	TurnOnSegment_E();
	//	TurnOnSegment_G();
		GPIOB = SetDispAsD;
	}

	void SevenSegmentDisplay::SetDisplayTo_E()
	{
    ///Serial.print("-E");
	//	TurnOnSegment_A();
	//	TurnOnSegment_D();
	//	TurnOnSegment_E();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAsE;
	}

	void SevenSegmentDisplay::SetDisplayTo_F()
	{
    ///Serial.print("-F");
	//	TurnOnSegment_A();
	//	TurnOnSegment_E();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAsF;
	}

	void SevenSegmentDisplay::SetDisplayTo_G()
	{
    ///Serial.print("-G");
	//	TurnOnSegment_A();
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_D();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAsF;
	}

	void SevenSegmentDisplay::SetDisplayTo_H()
	{
    ///Serial.print("-H");
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_E();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAsH;
	}

	void SevenSegmentDisplay::SetDisplayTo_J()
	{
    ///Serial.print("-J");
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_D();
	//	TurnOnSegment_E();
		GPIOB = SetDispAsJ;
	}

	void SevenSegmentDisplay::SetDisplayTo_L()
	{
    ///Serial.print("-L");
	//	TurnOnSegment_D();
	//	TurnOnSegment_E();
	//	TurnOnSegment_F();
		GPIOB = SetDispAsL;
	}

	void SevenSegmentDisplay::SetDisplayTo_O()
	{
    ///Serial.print("-O");
		SetDisplayTo_0();
	}

	void SevenSegmentDisplay::SetDisplayTo_P()
	{
    ///Serial.print("-P");
	//	TurnOnSegment_A();
	//	TurnOnSegment_B();
	//	TurnOnSegment_E();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAsP;
	}

	void SevenSegmentDisplay::SetDisplayTo_R()
	{
    ///Serial.print("-R");
	//	TurnOnSegment_A();
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_E();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAsR;
	}

	void SevenSegmentDisplay::SetDisplayTo_S()
	{
    ///Serial.print("-S");
		SetDisplayTo_5();
	}

	void SevenSegmentDisplay::SetDisplayTo_U()
	{
    ///Serial.print("-U");
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_D();
	//	TurnOnSegment_E();
	//	TurnOnSegment_F();
		GPIOB = SetDispAsU;
	}

	void SevenSegmentDisplay::SetDisplayTo_V()
	{
    ///Serial.print("-V");
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_D();
	//	TurnOnSegment_E();
	//	TurnOnSegment_F();
		GPIOB = SetDispAsU;
	}

	void SevenSegmentDisplay::SetDisplayTo_Y()
	{
    ///Serial.print("-Y");
	//	TurnOnSegment_B();
	//	TurnOnSegment_C();
	//	TurnOnSegment_F();
	//	TurnOnSegment_G();
		GPIOB = SetDispAsY;
	}


	//-----   Custom Segment Display Section  -----
	//-----   Put any irregular code down here. (Like simulating a device spasm and flickering)



