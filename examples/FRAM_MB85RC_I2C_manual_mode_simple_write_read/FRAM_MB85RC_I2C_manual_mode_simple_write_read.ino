/**************************************************************************/
/*!
    @file     FRAM_MB85RC_I2C_manual_mode_simple_write_read.ino
    @author   SOSAndroid (E. Ha.)
    @license  BSD (see license.txt)

    Example sketch that write & read on a 16K chip using "manual setting".

    @section  HISTORY

    v1.0.0 - First release
	V1.1.0 - Reworked code to test wider range of the memory
	v1.1.1 - More detailed feedback to Serial + adaptation to any density + Explanation of what is going on for 16K chip.

	
*/
/**************************************************************************/

#include <Wire.h>
#include <FRAM_MB85RC_I2C.h>

uint8_t writevalue = 0xBE;
uint16_t writevalue2 = 0xBEEF;
uint8_t readvalue = 0xEF; // at the end, readvalue should be equal to writevalue
uint16_t readvalue2 = 0xDEAD;


/*
A bit of mathematics in the example of the 16K chip
write address 0x25 =>  0b 000 00100101
write address 0x750 => 0b 111 01010000

According to 16K device's datasheet, the I2C protocol is used the following way :
7 bits device address + R/W bit. Device address is 1010xxx where xxx are the 3 MSB of the memory slot (out of 11 bits)
After that device address, the 8 LSB of the memory slot are send to the bus.

To read or write at 0x025 memory slot, the IC2 device address is 0b 1010 000 x. Which means 0x050
To read or write at 0x750 memory slot, the IC2 device address is 0b 1010 111 x. Which means 0x057

The second bit send is
	To read or write at 0x025 memory slot, (0b 000 00100101 & 0x000 11111111) => 0b 000 00100101 => 0x025
	To read or write at 0x750 memory slot, (0b 111 01010000 & 0x000 11111111) => 0b 000 01010000 => 0x010
	
The process (0x025 memory slot)
	wire.beginTranmission(0x50);
	Wire.write(0x25);
	...
	
The process (0x750 memory slot)
	wire.beginTranmission(0x57);
	Wire.write(0x10);
	...
*/



//Example for mamual mode and a 16K chip --- 16K devices does not have A2, A1 nor A0 support. Device address `i2c_addr = 0b1010000` where 3LSB stands for 3 MSB of the memory address
uint8_t chipaddress = 0x50;
boolean wp = false; //Allows memory write
int pin = 13;
uint16_t chipDensity = 16; //Just change that value to the density you want : 4, 16, 64, 128, 256, 512 or 1024

//random addresses to write from
uint16_t writeaddress = 0x025; // Beginning of the memory map
uint16_t writeaddress2 = (chipdensity * 128) - 80; // End of the memory map = (chipdensity * 0x80) - 0x50


//Creating object for FRAM chip
FRAM_MB85RC_I2C mymemory(chipaddress, wp, pin, chipDensity);
byte resultw, resultr;


void setup() {

	Serial.begin(115200);
	while (!Serial) ; //wait until Serial ready
	Wire.begin();
	
    Serial.println("Starting...");

	
	mymemory.begin();

//--------------------------- First run, beginning of memory map ---------------------
	Serial.println("1st test");
	Serial.print("Writing at location 0x");
	Serial.println(writeaddress, HEX);
	
    Serialln.print("Writing...");
	resultw = mymemory.writeByte(writeaddress, writevalue);
	Serialln.print("Reading...");
	resultr = mymemory.readByte(writeaddress, &readvalue);
	
	Serial.print("Written value 0x");
	Serial.println(writevalue, HEX);
	
	Serial.print("Read value 0x");
	Serial.println(readvalue, HEX);
	
	if (writevalue == readvalue) {
		Serial.println("Write Byte test : OK");
	}
	else {
		Serial.println("Write Byte test : NOT OK");
	}
	Serial.println(".... ....");
	
//-------------------------- Second run, end of the memory map -----------------------
 	Serial.println("2nd test");   
	Serial.print("Writing at location 0x");
	Serial.println(writeaddress2, HEX);
	
    Serialln.print("Writing...");
	resultw = mymemory.writeWord(writeaddress2, writevalue2);
	Serialln.print("Reading...");
	resultr = mymemory.readWord(writeaddress2, &readvalue2);
	
	Serial.print("Written value 0x");
	Serial.println(writevalue2, HEX);
	
	Serial.print("Read value 0x");
	Serial.println(readvalue2, HEX);
	
	if (writevalue2 == readvalue2) {
		Serial.println("Write word test : OK");
	}
	else {
		Serial.println("Write word test : NOT OK");
	}
	Serial.println(".... ....");
	//------------------------- Device IDs -----------------------------------------------

	uint16_t id;
	resultr = mymemory.getOneDeviceID(1, &id);
	Serial.print("Manufacturer 0x");
	Serial.println(id, HEX);
	
	resultr = mymemory.getOneDeviceID(3, &id);
	Serial.print("DensityCode 0x");
	Serial.println(id, HEX);
	
	resultr = mymemory.getOneDeviceID(4, &id);
	Serial.print("Density ");
	Serial.println(id, DEC);

	
}

void loop() {
	// nothing to do
}