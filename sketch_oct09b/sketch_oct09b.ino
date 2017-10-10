#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include "TM1637.h"

const int CLK = 11; //Set the CLK pin connection to the display
const int DIO = 12; //Set the DIO pin connection to the display
int NumStep = 0;  //Variable to interate
int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};

bool point = false;
TM1637 tm1637(CLK, DIO);  //set up the 4-Digit Display.

void setup() {
  
  tm1637.set();
  tm1637.init();
  
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
}

void loop() {
  tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
    
if(tm.Second % 2 == 0)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF); 
    //display.setSegments(tm.Second % 2 ? 1 : 0);

      TimeDisp[0] = tm.Hour / 10;
  TimeDisp[1] = tm.Hour % 10;
  TimeDisp[2] = tm.Minute / 10;
  TimeDisp[3] = tm.Minute % 10;
    
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    //delay(500);  //A half second delay between steps.

    delay(9000);
  }
  delay(500);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
