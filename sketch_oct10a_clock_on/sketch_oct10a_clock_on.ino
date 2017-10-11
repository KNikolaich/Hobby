// часы
#include <DS1302.h>
#include <stdio.h>

// дисплей
#include "TM1637.h"

// часы
DS1302 rtc(10, 7, 5);


// дисплей

#define interval 1000           // интервал между переключениями дисплея

#define CLK 11 //Set the CLK pin connection to the display
#define DIO 12 //Set the DIO pin connection to the display
int8_t TimeDisp[] = {0x0e,0x0f,0x0f,0x0f};
unsigned long previousMillis = 0;        // храним время последнего переключения дисплея
bool points = false;

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
    RefreshDateTime();    
    delay(100);
}


void RefreshDateTime(){
  unsigned long currentMillis = millis();
 //проверяем не прошел ли нужный интервал, если прошел то
  if(currentMillis - previousMillis > interval) {
    // сохраняем время последнего переключения
    previousMillis = currentMillis; 

    Time tm = printTime();

    tm1637.point(points ? POINT_ON:POINT_OFF);
    points = !points;
      
    TimeDisp[0] = tm.hr / 10;
    TimeDisp[1] = tm.hr % 10;
    TimeDisp[2] = tm.min / 10;
    TimeDisp[3] = tm.min % 10;
    tm1637.display(TimeDisp);  
  
    
  }
}

Time printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d",
           day.c_str(),
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time.
  Serial.println(buf);
  return t;
}


String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Sunday";
    case Time::kMonday: return "Monday";
    case Time::kTuesday: return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday: return "Thursday";
    case Time::kFriday: return "Friday";
    case Time::kSaturday: return "Saturday";
  }
  return "(unknown day)";
}
