/*DIY Electric skateboard receiver for wireless nunchuck controller

This example code reads out the Y joystick value and prepares it for smooth acceleration on your electric scateboard.
The output singal is standard esc PPM signal. The input values are shaped with a timeinvariant pt1 filter which is fed
by two different time constats (acceleration and break). I reccomand a small break time constant for immediate break
result and no breakdelay. Acceleration on the contrary shoult be big for smooth acceleration.

T_accel = 1;
T_break = 0.01;

For more informatino follow this link:
www.massimoferrantino.wordpress.com
*/

#include <Wire.h>
#include <SoftwareServo.h>
#include <Nunchuck.h>
#include <TimerOne.h>


#define SERIAL_BAUD_RATE  115200

int microseconds = 2000;                                  // Timer interrupt value for timeinvariant filter 
volatile float y_neu = 90, y = 90, u = 125, Ta = 0 ;      // values for recursively calculation
volatile float T1 = 1, V =1, T_accel = 1, T_break = 0.01; // Time values and amplification
volatile float n = 0;                                     // X-joystick value just for error detection
volatile int esc_val = 0; //                              // esc value between 0 - 180
volatile bool firstStart = true, transmissionError = true;// booleans for improving behaviour

Nunchuck nunchuck;
SoftwareServo esc;

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE);
  pinMode(2, OUTPUT);                         // conncecte pull up resistors
  digitalWrite(2, HIGH);                      // 5V for pullups 
 
  Ta = (float)microseconds / 1000000;         // sampletime Ta in seconds
  
  esc.attach(5); 
  nunchuck = Nunchuck(WIRELESS_KAMA_NUNCHUCK);
  nunchuck.initialize();
  while(!nunchuck.NunchuckConnected){         // hand on here when no nunchuck was detected  
  }
    delay(1000);
    Timer1.initialize(microseconds);          // set the timer
    Timer1.attachInterrupt( timerIsr );       // attach the service routine
}

void loop()
{
    nunchuck.readData();
    
    if(u >= 125){                           // changing time constants here for acceleration and break 
      T1 = T_accel;
      }
     else if(u < 125 && transmissionError == false){
      T1 = T_break;
      }
      
     if (firstStart == true){               // improving behaviour on poweron of the uC
       T1 = T_break;
      }
    
    esc_val = map((int)y_neu, 0 ,255, 0,180);//mapping values and prepare for esc signal 
    esc.write(esc_val);
    
}

void timerIsr()
{
  n = nunchuck.getJoyX();
  u = nunchuck.getJoyY();
  y_neu = (float)(1- Ta/T1)*y + Ta/T1*V*u;
  y = y_neu;

  if(y < 100){
    firstStart = false;
    }
  if(u == 0 && n == 0){               // on receiving errors bytes with value 0 are read. To detect this and fade out errors.
      transmissionError = true;
    }
  else{
      transmissionError = false;
    }
    
  SoftwareServo::refresh();
}
