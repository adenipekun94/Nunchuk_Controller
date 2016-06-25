/*
  Nunchuck.cpp - Bjorn's Nunchuck Library
  
  Based on code from http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1259091426/135
*/

#include "Nunchuck.h"

// Initialize Class Variables --------------------------------------------------

unsigned int joy_x = 0;
unsigned int joy_y = 0;
unsigned int acc_x = 0;
unsigned int acc_y = 0;
unsigned int acc_z = 0;
unsigned int btn_c = 0;
unsigned int btn_z = 0;


unsigned long previous_read_time = 0;


// Constructors ----------------------------------------------------------------

Nunchuck::Nunchuck()
{
	// Default type is wired nunchuck
    
	nunchuckType = WIRED_NUNCHUCK;
}

Nunchuck::Nunchuck(int type)
{
	// Pass in WIRED_NUNCHUCK or WIRELESS_KAMA_NUNCHUCK
   
	nunchuckType = type;
}

// Public Functions -------------------------------------------------------------


void Nunchuck::initialize()
{
    
  Wire.begin();
  Wire.setClock(400000L);
  if(nunchuckType == WIRELESS_KAMA_NUNCHUCK)
  {
	Wire.beginTransmission(0x52);
	Wire.write (0xF0);
	Wire.write (0x55);
	Wire.endTransmission();
	delay(30);
  
	Wire.beginTransmission (0x52);
	Wire.write (0xFB);
	Wire.write (0x00);
	Wire.endTransmission();
	delay(30);
	
	Wire.beginTransmission(0x52);
	Wire.write (0xFA);
	Wire.endTransmission();
	delay(30);
  
	Wire.requestFrom(0x52, 6);
	Serial.print("Wireless Nunchuck ID is: ");
	while(Wire.available())  
	{
        NunchuckConnected = true;
		byte c = Wire.read();
		NunchuckAddress += c-48;
        
	}
    Serial.println(NunchuckAddress);
	Serial.println();
	delay(30);
  }
  else if(nunchuckType == WIRED_NUNCHUCK)
  {
	Wire.beginTransmission(0x52);
	Wire.write (0x40);
	Wire.write (0x00);
	Wire.endTransmission();
	delay(30);
  }
}

void Nunchuck::readData()
{
	if (millis() - previous_read_time > READ_DELAY)
	 {
		unsigned int buffer[6];
		byte buffer_index = 0;
	  
		Wire.beginTransmission(0x52);
		Wire.write(0x00);
		Wire.endTransmission();
		
		if(nunchuckType == WIRED_NUNCHUCK)
		{
			delay(1); /* This delay is required for a wired nunchuck otherwise the data will appear maxed out */
		}
	  
		Wire.requestFrom(0x52, 6);
		while(Wire.available())    
		{
			buffer[buffer_index] = Wire.read();
			buffer_index++;
		}
		
		joy_x = buffer[0];
		joy_y = buffer[1];
		acc_x = ((buffer[2] << 2) | ((buffer[5] & 0x0C) >> 2) & 0x03FF);
		acc_y = ((buffer[3] << 2) | ((buffer[5] & 0x30) >> 4) & 0x03FF);
		acc_z = ((buffer[4] << 2) | ((buffer[5] & 0xC0) >> 6) & 0x03FF);
		btn_c = !((buffer[5] & 0x02) >> 1);
		btn_z = !(buffer[5] & 0x01);
		
		previous_read_time = millis();
	  }
}

long int  Nunchuck::getJoyX() { return (long int)joy_x; }
long int  Nunchuck::getJoyY() { return (long int)joy_y; }
long int  Nunchuck::getAccX() { return (long int)acc_x; }
long int  Nunchuck::getAccY() { return (long int)acc_y; }
long int  Nunchuck::getAccZ() { return (long int)acc_z; }
long int  Nunchuck::getBtnC() { return (long int)btn_c; }
long int  Nunchuck::getBtnZ() { return (long int)btn_z; }

boolean Nunchuck::cButtonPressed()
{
	boolean isPressed = false;
	
	if(btn_c == 1)
	{
		isPressed = true;
	}
	
	return isPressed;
}

boolean Nunchuck::zButtonPressed()
{
	boolean isPressed = false;
	
	if(btn_z == 1)
	{
		isPressed = true;
	}
	
	return isPressed;
}

// This only works if a serial connection is already established
// (put Serial.begin(######) in the Setup() function
void Nunchuck::printData()
{
	
	Serial.print("    "); Serial.print(joy_x);
	Serial.print("      "); Serial.print(joy_y);
	Serial.print("      "); Serial.print(acc_x);
	Serial.print("       "); Serial.print(acc_y);
	Serial.print("       "); Serial.print(acc_z);
	Serial.print("        "); Serial.print(btn_c);
	Serial.print("          "); Serial.print(btn_z);
	Serial.println("");
}
