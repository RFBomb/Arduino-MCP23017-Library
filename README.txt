This is a library for the MCP23017 I2c Port Expander
 
These chips use I2C to communicate, 2 pins required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above must be included in any redistribution

-------------------------------------------------------------------------------

Additions to the library were produced by Robert Brenckman.
 - Additions and code produced in Visual Studio 2017.
 - Description additions to Adafruit_MCP23017.h were made to make the library easier to use.
	- Descriptions were previously only in the .cpp file, forcing you to dig deeper into code, rather than having descriptions right when you call the function.
	
 - 7-Segment Display contains functions that allow a single MCP23017 chip to multiplex up to 8 seven-segment displays. 
-------------------------------------------------------------------------------

To download. click the DOWNLOADS button in the top right corner, rename the uncompressed folder Adafruit_MCP23017. Check that the Adafruit_MCP23017 folder contains Adafruit_MCP23017.cpp and Adafruit_MCP23017.h

Place the Adafruit_MCP23017 library folder your <arduinosketchfolder>/libraries/ folder. You may need to create the libraries subfolder if its your first library. Restart the IDE.