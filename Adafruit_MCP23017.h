/*************************************************** 
  This is a library for the MCP23017 i2c port expander

  These displays use I2C to communicate, 2 pins are required to  
  interface.
  Adafruit invests time and resources providing this open (awkward-to-use) source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
/**
Made much more user friendly by Robert Brenckman. 8/16/2017
Most function descriptions were pulled from the .CPP file, but additional detail was added to describe the input variables in each routine. 
(Should have been defined from start, or atleast used meaningful variable names.)
All licenses stated above apply.
**/

#pragma once

#ifndef _Adafruit_MCP23017_H_
#define _Adafruit_MCP23017_H_

// Don't forget the Wire library!!
#ifdef __AVR_ATtiny85__
#include <TinyWireM.h>
#else
#include <Wire.h>
#endif

class Adafruit_MCP23017 {
public:
	/*Starts communication with chip at specified HEX address. */
  void begin(uint8_t addr);

	/* Initializes the default MCP23017, with 000 for the configurable part of the address. */
  void begin(void);

	/*	Set a pin to an Input / Output, 
	*	p = which pin to use (0-15, where 0-7 is Port A and 8-15 is Port B)
	* Port B = pins 1-8, Port A = pins 21-28
	* d = 'INPUT' / 'OUTPUT'
	*/
  void pinMode(uint8_t p, uint8_t d);

	/*Write a value to a pin. 
	*	p = which pin to use (0-15, where 0-7 is Port A and 8-15 is Port B)
	* Port B = pins 1-8, Port A = pins 21-28
	* d = 'HIGH' / 'LOW'
	*/
  void digitalWrite(uint8_t p, uint8_t d);

	/*Activate/Deactivate 100k Pull-Up resistor on specified pin. 
	*	p = which pin to use (0-15, where 0-7 is Port A and 8-15 is Port B)
	* Port B = pins 1-8, Port A = pins 21-28
	*	d = 1 (on) / 0 (off)
	*/
  void pullUp(uint8_t p, uint8_t d);

	/*Returns the value of a specified pin number. (1 or 0)
	*	p = which pin to use (0-15, where 0-7 is Port A and 8-15 is Port B)
	* Port B = pins 1-8, Port A = pins 21-28
	*/
  uint8_t digitalRead(uint8_t p);

	/*	Write to all 16 pins simeoultaneously. Useful for multiplexing.
	*	Still not sure wtf do do here. Not sure how to generate a 16 bit number to get the desired effects.
	*/
  void writeGPIOAB(uint16_t);

  /*Reads all pins and returns a 16-bit number with their states.*/
  uint16_t readGPIOAB();

	/*	Read a single port, A or B, and return its current 8 bit value.
	*	Parameter b should be 0 for GPIOA, and 1 for GPIOB.*/
  uint8_t readGPIO(uint8_t b);

	/**
	* Configures the interrupt system. both port A and B are assigned the same configuration.
	* Mirroring will OR both INTA and INTB pins.
	* Opendrain will set the INT pin to value or open drain.
	* polarity will set LOW or HIGH on interrupt.
	* Default values after Power On Reset are: (false,flase, LOW)
	* If you are connecting the INTA/B pin to arduino 2/3, you should configure the interupt handling as FALLING with
	* the default configuration.
	*/
  void setupInterrupts(uint8_t mirroring, uint8_t open, uint8_t polarity);

	/* Set's up a pin for interrupt. uses arduino MODEs: CHANGE, FALLING, RISING.
	* Note that the interrupt condition finishes when you read the information about the port / value
	* that caused the interrupt or you read the port itself. Check the datasheet, can be confusing.  
	*	p = which pin to use (0-15, where 0-7 is Port A and 8-15 is Port B)
	*/
  void setupInterruptPin(uint8_t p, uint8_t mode);

	/**/
  uint8_t getLastInterruptPin();
  
	/**/
  uint8_t getLastInterruptPinValue();



 private:
  uint8_t i2caddr;  // User Set I2C address for chip.
  
  /*Returns the bit for a specified pin number*/
  uint8_t bitForPin(uint8_t pin);

  /* Returns the register location for a pin.*/
  uint8_t regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr); 
  
  /* reads a value from a register.*/
  uint8_t readRegister(uint8_t addr); 
  
  /* writes a value to a register.*/
  void writeRegister(uint8_t addr, uint8_t value);	

  /**
   * Utility private method to update a register associated with a pin (whether port A/B)
   * reads its value, updates the particular bit, and writes its value.
   */
  void updateRegisterBit(uint8_t p, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr);

};

///---------------------------------------------------------------
///Default I2C address (all three address pins to ground)
#define MCP23017_ADDRESS 0x20

///---------------------------------------------------------------
/// Misc Registers
#define MCP23017_INT_ERR 255

///---------------------------------------------------------------
/// Registers for IOCON.Bank = 0
///---------------------------------------------------------------

///	Registers for A (8-Bit-Mode)
#define MCP23017_IODIRA 0x00
#define MCP23017_IPOLA 0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 0x06
#define MCP23017_INTCONA 0x08
#define MCP23017_IOCONA 0x0A
#define MCP23017_GPPUA 0x0C
#define MCP23017_INTFA 0x0E
#define MCP23017_INTCAPA 0x10
#define MCP23017_GPIOA 0x12
#define MCP23017_OLATA 0x14

///	Registers for B (8-Bit-Mode)
#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATB 0x15

///---------------------------------------------------------------
/// Registers for IOCON.Bank = 1 (Currently not used)
///---------------------------------------------------------------

///	Registers for A (16-Bit-Mode)
#define MCP23017_IODIRA_16 0x00
#define MCP23017_IPOLA_16 0x01
#define MCP23017_GPINTENA_16 0x02
#define MCP23017_DEFVALA_16 0x03
#define MCP23017_INTCONA_16 0x04
#define MCP23017_IOCONA_16 0x05
#define MCP23017_GPPUA_16 0x06
#define MCP23017_INTFA_16 0x07
#define MCP23017_INTCAPA_16 0810
#define MCP23017_GPIOA_16 0x19
#define MCP23017_OLATA_16 0x0A

///	Registers for B (16-Bit-Mode)
#define MCP23017_IODIRB_16 0x10
#define MCP23017_IPOLB_16 0x11
#define MCP23017_GPINTENB_16 0x12
#define MCP23017_DEFVALB_16 0x13
#define MCP23017_INTCONB_16 0x14
#define MCP23017_IOCONB_16 0x15
#define MCP23017_GPPUB_16 0x16
#define MCP23017_INTFB_16 0x17
#define MCP23017_INTCAPB_16 0x18
#define MCP23017_GPIOB_16 0x19
#define MCP23017_OLATB_16 0x1A


#endif
