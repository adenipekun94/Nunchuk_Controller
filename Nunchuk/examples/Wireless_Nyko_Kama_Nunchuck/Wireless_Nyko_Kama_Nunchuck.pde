/*
	This is an example using the Nunchuck library with
	a wireless Nyko Kama nunchuck to print out data.

*/

#include <Wire.h>
//#include <Servo.h>
#include <Nunchuck.h>

#define SERIAL_BAUD_RATE  115200

Nunchuck nunchuck;
//Servo esc;

void setup()
{
    pinMode(40, OUTPUT);
    Serial.begin(SERIAL_BAUD_RATE);
   // esc.attach(41); 
    nunchuck = Nunchuck(WIRELESS_KAMA_NUNCHUCK);
    nunchuck.initialize();
    delay(1000);
}

void loop()
{
    nunchuck.readData();
  

    digitalWrite(40, (int) nunchuck.getBtnZ());
 //   esc.write(map(nunchuck.getJoyY(),0,255,0,180));
}
