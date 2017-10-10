#define sens1 A7 // LRD 7 pin
#define led1 2  // LED1 pin D2
#define pinWater 7  // Watermotor pin D7
#define led2 3 // LED2 pin D3


#define ledG 9 // LED green pin D9
#define ledR 6 // LED red pin D6
#define ledY 8 // LED yellow pin D8

int btnInput = 4; // Input managment channel pin 4 (4)
volatile long CH1PulseStartTicks;
bool ledState = false;
byte triadeState = 0; 
bool button_old = false;

void setup() {
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


