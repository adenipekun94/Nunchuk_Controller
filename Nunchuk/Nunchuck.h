/*
  Nunchuck.h - Bjorn's Nunchuck Library
  
  Based on code from http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1259091426/135
*/

#ifndef Nunchuck_h
#define Nunchuck_h

// Includ standard types and constants of the Arduino language

#include <Arduino.h>
#include <Wire.h>

// Define supported Nunchuck types
#define WIRED_NUNCHUCK         0
#define WIRELESS_KAMA_NUNCHUCK  1

#define READ_DELAY        5      /* (milliseconds) - Increase this number to not read the nunchuck data so fast */

class Nunchuck
{
  private:
	unsigned int joy_x;
	unsigned int joy_y;
	unsigned int acc_x;
	unsigned int acc_y;
	unsigned int acc_z;
	unsigned int btn_c;
	unsigned int btn_z;
	
	int nunchuckType;

	unsigned long previous_read_time;
	
  public:
	Nunchuck();
	Nunchuck(int nunchuckType);
	void initialize();
	void readData();
	void printData();
	
	long int getJoyX();
	long int getJoyY();
	long int getAccX();
	long int getAccY();
	long int getAccZ();
	long int getBtnC();
	long int getBtnZ();
	
    bool NunchuckConnected = false;
    String NunchuckAddress = "";
    
	boolean cButtonPressed();
	boolean zButtonPressed();
};


#endif
