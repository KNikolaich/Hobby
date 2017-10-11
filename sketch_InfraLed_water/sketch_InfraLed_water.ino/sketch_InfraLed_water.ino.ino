#define sens1 A7 // LRD 7 pin
#define led1 2  // LED1 pin D2
#define pinWater 7  // Watermotor pin D7
#define led2 3 // LED2 pin D3


#define ledG 9 // LED green pin D9
#define ledR 6 // LED red pin D6
#define ledY 8 // LED yellow pin D8

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

int btnInput = 4; // Input managment channel pin 4 (4)
volatile long CH1PulseStartTicks;
bool ledState = false;
byte triadeState = 0; 
bool button_old = false;

void setup() {

  // дисплей
  tm1637.set();
  tm1637.init();
  
  // put your setup code here, to run once:
  pinMode(sens1, INPUT);
  pinMode(btnInput, INPUT_PULLUP);
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  pinMode(pinWater, OUTPUT);

  
  pinMode(ledY, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);

  
  LedTriade(100);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  RefreshDateTime();
  int val1 = analogRead(sens1);//ApproximateSensor();
  bool valBtn = digitalRead(btnInput);
  
 if ((digitalRead(btnInput)==LOW)&&(button_old==1))
  { 
    
    ChangeTriadeState(val1);
    //count = count + 1;
    //Serial.println(count);
  }
  button_old = digitalRead(btnInput); 
  
  delay(10);
if((micros() - CH1PulseStartTicks) > 1000000 && triadeState == 1){
  
  Darken(val1);
  
}
}

void ChangeTriadeState(int volume){
      triadeState++;
  if(triadeState > 2){
    triadeState =0;
  }
  
  LedTriade(volume);
  int waterOn = 0;
  if (volume==2){
    waterOn = 1;
  }
}


void LedTriade(int volume){
  switch(triadeState){
    case 0:
    
  Serial.println("off");
  digitalWrite(ledG, 1);  
  digitalWrite(ledR, 0);
  digitalWrite(ledY, 0);
  
  digitalWrite(pinWater,0);
  Darken(100);
      break;
    case 1:
    Serial.println("Auto");
  digitalWrite(ledG, 0);  
  digitalWrite(ledR, 0);
  digitalWrite(ledY, 1);
  
  digitalWrite(pinWater,1);
  Darken(volume);
      break;
    case 2:
    Serial.println("on");
  digitalWrite(ledG, 0);  
  digitalWrite(ledR, 1);
  digitalWrite(ledY, 0);
  
  digitalWrite(pinWater,0);
  Darken(1000);
      break;
    
  }
  
}

// апроксимируем N значений с сенсора, чтобы не обращать внимание на ложные выпады
int ApproximateSensor(){
  int result=0;
  for(int i = 0;i<100;i++ ){
    result += analogRead(sens1);
  
  delay(5);
  }
  
  result/=100;
  //int volume = analogRead(sens1); // read the value of sensor 1
  return result;
}

// метод "стемнело" определяет, что делать при нступлении темноты или расвете
void Darken(int volume){

if (ledState){
  ledState = volume > 200;
}else{
  ledState = volume > 280;
}
  //bool ledOn = volume > 250;
  
  digitalWrite(led1, ledState);
  
  digitalWrite(led2, ledState);
  
  CH1PulseStartTicks = micros();
}


// Работы с дисплеем
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

