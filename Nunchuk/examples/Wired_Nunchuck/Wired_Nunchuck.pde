/*
	This is an example using the Nunchuck library with
	a wired nunchuck to print out data.
*/


#include <Wire.h>
#include <Nunchuck.h>

#define SERIAL_BAUD_RATE  115200

Nunchuck nunchuck;

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);
    
    nunchuck = Nunchuck(WIRED_NUNCHUCK);
    nunchuck.initialize();
}

void loop()
{
    nunchuck.readData();
    nunchuck.printData();
}