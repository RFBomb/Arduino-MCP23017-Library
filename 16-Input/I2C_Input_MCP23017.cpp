#include "I2C_Input_MCP23017.h"


Adafruit_MCP23017 MCP_Input_Chip;
////-------------------------------------------------------------------
//		Define Variables
////-------------------------------------------------------------------

//	Pin Numbers: 0 - 15, where 0 - 7 is Port A and 8 - 15 is Port B
//	Port B = pins 1 - 8, Port A = pins 21 - 28
uint8_t InputPin_01 = 0;
uint8_t InputPin_02 = 1;
uint8_t InputPin_03 = 2;
uint8_t InputPin_04 = 3;
uint8_t InputPin_05 = 4;
uint8_t InputPin_06 = 5;
uint8_t InputPin_07 = 6;
uint8_t InputPin_08 = 7;
uint8_t InputPin_09 = 8;
uint8_t InputPin_10 = 9;
uint8_t InputPin_11 = 10;
uint8_t InputPin_12 = 11;
uint8_t InputPin_13 = 12;
uint8_t InputPin_14 = 13;
uint8_t InputPin_15 = 14;
uint8_t InputPin_16 = 15;

void I2C_Input_MCP23017::SetupChipIO(uint8_t ChipAddress_)
{
	///Initialize communication with MCP23017 chip
	Serial.print("Attempting 16-Input Chip Communication.... \n");
	ChipAddress = ChipAddress_;
	MCP_Input_Chip.begin(ChipAddress_);
	byte error = Wire.endTransmission();
	if (error == 0){Serial.print("Input Chip Communication Established!\n");	}	
	else	{		Serial.print("Input Chip Communication Failed :[ \n");	}

	///Setup IO on chip
	SetupInput(InputPin_01);
	SetupInput(InputPin_02);
	SetupInput(InputPin_03);
	SetupInput(InputPin_05);
	SetupInput(InputPin_06);
	SetupInput(InputPin_07);
	SetupInput(InputPin_08);
	SetupInput(InputPin_09);
	SetupInput(InputPin_10);
	SetupInput(InputPin_11);
	SetupInput(InputPin_12);
	SetupInput(InputPin_13);
	SetupInput(InputPin_14);
	SetupInput(InputPin_15);
	SetupInput(InputPin_16);

}


// Sets up an input on the chip.
void I2C_Input_MCP23017::SetupInput(uint8_t InputNumber)
{
	MCP_Input_Chip.pinMode(InputNumber, INPUT);
	MCP_Input_Chip.pullUp(InputNumber, 1);
}


// Reads all chip IO, reports back the first input number detected. 
int I2C_Input_MCP23017::GetFirstIO()
{
	for (int i = 0; i = i + 1; ) 
	{	
		bool V = ReadInputStatus(i);
		if (V == true) { return i; /*Returns +1 to correllate with the InputPin numbers*/ };
		Serial.println(i);
		if (i > 15) { return 0; }
	}
}

/*Reads all inputs on the chip and returns a 16-bit integer.*/
uint16_t I2C_Input_MCP23017::ReadAllInputs() { return(MCP_Input_Chip.readGPIOAB()); }

/*	Reads a single port on the chip and returns an 8-bitvalue. 
*	Parameter b should be 0 for GPIOA or 1 for GPIOB
*/
uint8_t I2C_Input_MCP23017::Read8Inputs(uint8_t b){	return(MCP_Input_Chip.readGPIO(b));}

bool I2C_Input_MCP23017::ReadInputStatus(uint8_t InputNumber)
{
	return (MCP_Input_Chip.digitalRead(InputNumber)==1) ?true:false ;
}
