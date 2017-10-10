// часы
#include <DS1302.h>
DS1302 rtc(10, 7, 5);

// дисплей
#include "TM1637.h"

const int CLK = 11; //Set the CLK pin connection to the display
const int DIO = 12; //Set the DIO pin connection to the display
int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};

TM1637 tm1637(CLK, DIO);  //set up the 4-Digit Display.


void setup()
{
  
  tm1637.set();
  tm1637.init();
    Serial.begin(9600);
    //display.setBrightness(0x0a);  //set the diplay to maximum brightness
}


void loop()
{
    Time t = rtc.time();
    Serial.println(t.yr);
    Serial.println(t.hr);
    Serial.println(t.min);
    Serial.println(t.sec);
    Serial.println(t.mon);
    Serial.println(t.date);
    Serial.println(t.day);

if(t.sec % 2 == 0)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF); 
    //display.setSegments(tm.Second % 2 ? 1 : 0);

      TimeDisp[0] = t.hr / 10;
  TimeDisp[1] = t.hr % 10;
  TimeDisp[2] = t.min / 10;
  TimeDisp[3] = t.min % 10;
  tm1637.display(TimeDisp);
    delay(500);
}
