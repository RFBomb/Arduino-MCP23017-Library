#pragma once
#ifndef _Adafruit_MCP23017_H_	
	#include <Adafruit_MCP23017.h>
#endif
#include <Arduino.h> // Need for serial output and 'delay' function

class SevenSegmentDisplay
{
public:
	/*
	*	CharacterToDisplay: Select a character to display. Single Characters only.	
	*	Numbers: MUST BE 0-9. Less than 0/Greater than 9 not accepted.	
	*	Available Letters: A, B, D, E, F, G, H, J, L, O, P, R, S, U, V, Y.  " - " is also accepted.
	*	DisplayNo: Set which digit location to display this in. (In the number '73321' the '2' is DisplayNO number "3".)
	*	DecimalPt: Set if you wish to have the decimal appear or not.
	*/
	void SetDisplayOutput(String CharacterToDisplay, int DisplayNo = 0, bool DecimalPt = false);
	/**Clears the Displays of all digits. Activates the Decimal Point on all displays. */
	void ClearDisplay();
	/** Updating all of the displays with the most current settings.*/
	void LoopThroughDisplays();
	/**Instead of looping through all 8 displays, pick and choose which displays to multiplex.*/
	void LoopThroughOptionalDisplays(bool D0 = false, bool D1 = false, bool D2 = false,
		bool D3 = false, bool D4 = false, bool D5 = false, bool D6 = false, bool D7 = false);
	/* Initialize the GPIO Pins as outputs. See Code for which pins to utilize.
	* Common Anode: Select if this chip is using a common Anode display or common kathode.
	* ChipAddress: This is the I2C address for the display chip.
	*/
	void SetupSegmentGPIO(bool CommonAnode, uint8_t ChipAddress);
	/**	Sets the delay for how long the outputs should be off before switching to the next digit (in milliseconds).
	*	How long each display should stay on when multiplexing (in milliseconds). */
	void UpdateTimerVariables(int TimeOffDelay, int StayOnTime);

  void DisplayChar(String CharacterToDisplay, int DisplayNo, bool DecimalPt);

  /*Send a chracter to the display, then read back the 16-bit value*/
  void Test(String Char_, int DisplayNo, bool DecimalPt);

private:
  //Routines to setup the chip outputs, and turn segments on and off.
  
	void SetupGPIO_Outputs(int GPIO_Output);
	void SetOutputHigh(int GPIO_Output);
	void SetOutputLow(int GPIO_Output);
	void ActivateDisplay(int DisplayNumber = 0);
	void TurnAllSegmentsoff();
	void TurnOffAllDisplays();
	void TurnOnDisplay0();
	void TurnOnDisplay1();
	void TurnOnDisplay2();
	void TurnOnDisplay3();
	void TurnOnDisplay4();
	void TurnOnDisplay5();
	void TurnOnDisplay6();
	void TurnOnDisplay7();
	void TurnOnSegment_A();
	void TurnOnSegment_B();
	void TurnOnSegment_C();
	void TurnOnSegment_D();
	void TurnOnSegment_E();
	void TurnOnSegment_F();
	void TurnOnSegment_G();
	void TurnOnSegment_H();

// routines to check which character to display

  void DisplayNumber(String NumberToDisplay);
  void DisplayLetter(String LetterToDisplay);
  void DisplaySpecialCase(String SpecialCase);
  void UpdateDisplay();
  
/// Numeric characters to display

	void SetDisplayTo_0();
	void SetDisplayTo_1();
	void SetDisplayTo_2();
	void SetDisplayTo_3();
	void SetDisplayTo_4();
	void SetDisplayTo_5();
	void SetDisplayTo_6();
	void SetDisplayTo_7();
	void SetDisplayTo_8();
	void SetDisplayTo_9();

///Alphabet letters to display

	void SetDisplayTo_A();
	void SetDisplayTo_B();
	void SetDisplayTo_C();
	void SetDisplayTo_D();
	void SetDisplayTo_E();
	void SetDisplayTo_F();
	void SetDisplayTo_G();
	void SetDisplayTo_H();
	void SetDisplayTo_J();
	void SetDisplayTo_L();
	void SetDisplayTo_O();
	void SetDisplayTo_P();
	void SetDisplayTo_R();
	void SetDisplayTo_S();
	void SetDisplayTo_U();
	void SetDisplayTo_V();
	void SetDisplayTo_Y();


/* GPIO Values are stored in a string, then concatenated + converted into a 16-digit number uint16_t and sent to the MCP23017 chip */
uint8_t GPIOA;	/*String for setting the values to GPIOA */
uint8_t GPIOB;  /*String for setting the values to GPIOB */
String StrGPIOBits;  /*String for setting the values to GPIOAB */
uint16_t GPIOBits; /*16-bit value to send to the chip to update display */

// Misc Registers
#define OUTPUT 0x1
#define HIGH 0x1
#define LOW 0x0
	

// ALL REGISTERS BELOW CORRESPOND TO COMMON-ANODE DISPLAYS. MUST BE INVERTED FOR COMMON-CATHODE!!!

#define KillDisplay 1111111100000000	// 16-bit register for use when turning the display off.

// 8-Bit registers for choosing which display to display on
#define TurnOnDisp0	0x1		//00000001
#define TurnOnDisp1	0x2		//00000010
#define TurnOnDisp2	0x4		//00000100
#define TurnOnDisp3	0x8		//00001000
#define TurnOnDisp4	0x16	//00010000
#define TurnOnDisp5	0x32	//00100000
#define TurnOnDisp6	0x64	//01000000
#define TurnOnDisp7	0x256	//10000000

// 8-Bit Registers for numeric digits
#define SetDispAs0	0xC0	//11000000
#define SetDispAs1	0xF9	//11111001
#define SetDispAs2	0xA4	//10100100
#define SetDispAs3	0xB0	//10110000
#define SetDispAs4	0x99	//10011001
#define SetDispAs5	0x92	//10010010
#define SetDispAs6	0x82	//10000010
#define SetDispAs7	0xF8	//11111000
#define SetDispAs8	0x80	//10000000
#define SetDispAs9	0x98	//10011000

// 8-Bit Registers for Alphabet chars
#define SetDispAsA	0x88	//10001000
#define SetDispAsB	0x80	//10000000
#define SetDispAsC	0xC6	//11000110
#define SetDispAsD	0xA1	//10100001
#define SetDispAsE	0x86	//10000110
#define SetDispAsF	0x8E	//10001110
#define SetDispAsG	0x90	//10010000
#define SetDispAsH	0x89	//10001001
#define SetDispAsJ	0xE1	//11100001
#define SetDispAsL	0xC7	//11000111
#define SetDispAsO	0xC0	//11000000
#define SetDispAsP	0x8C	//10001100
#define SetDispAsR	0x88	//10001000
#define SetDispAsU	0xC1	//11000001
#define SetDispAsY	0x99	//10011001
	
// 8-Bit Registers for Special Chars
#define SetDispAsExclamation	0x7D	//1111101
#define SetDispAsMiddleDash		0xBF	//10111111
#define SetDispAsThreeDashes	0xB6	//10110110
#define SetDispAsTopRightBotLeftCorner	0xE4	//11100100
#define SetDispAsTopLeftBotRightCorner	0xD2	//11010010
#define SetDispAsAD		0xF6	//11110110
#define SetDispAsBE		0xED	//11101101
#define SetDispAsFC		0xDB	//11011011


};

