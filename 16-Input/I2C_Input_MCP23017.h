#pragma once
#include <Adafruit_MCP23017.h>
#include <Arduino.h> // Need for serial output and 'delay' function


class I2C_Input_MCP23017{
public:
	/*Starts communication with chip at specified HEX address. Sets up the IO on the chip.*/
	void SetupChipIO(uint8_t ChipAddress);
	/*Reads the state of a single input*/
	bool ReadInputStatus(uint8_t InputNumber);
	/* Reads all chip IO, reports back the first input number detected. */
	int GetFirstIO();
	/*Reads all inputs on the chip and returns a 16-bit integer.*/
	uint16_t ReadAllInputs();
	/*	Reads a single port on the chip and returns an 8-bitvalue.
	*	Parameter b should be 0 for GPIOA or 1 for GPIOB
	*/
	uint8_t Read8Inputs(uint8_t b);


private:
	/* Sets up an input on the chip.*/
	void SetupInput(uint8_t InputNumber);

///------------------------------------------------------------
/// All Variables are listed below:

public:	// Public Variables

/*	
	/// Input Status
	bool Input_01_Status;
	bool Input_02_Status;
	bool Input_03_Status;
	bool Input_04_Status;
	bool Input_05_Status;
	bool Input_06_Status;
	bool Input_07_Status;
	bool Input_08_Status;
	bool Input_09_Status;
	bool Input_10_Status;
	bool Input_11_Status;
	bool Input_12_Status;
	bool Input_13_Status;
	bool Input_14_Status;
	bool Input_15_Status;
	bool Input_16_Status; 
*/
	/// Input Pins
	uint8_t InputPin_01;
	uint8_t InputPin_02;
	uint8_t InputPin_03;
	uint8_t InputPin_04;
	uint8_t InputPin_05;
	uint8_t InputPin_06;
	uint8_t InputPin_07;
	uint8_t InputPin_08;
	uint8_t InputPin_09;
	uint8_t InputPin_10;
	uint8_t InputPin_11;
	uint8_t InputPin_12;
	uint8_t InputPin_13;
	uint8_t InputPin_14;
	uint8_t InputPin_15;
	uint8_t InputPin_16;

private:	// Private Variables
	uint8_t ChipAddress;
};



