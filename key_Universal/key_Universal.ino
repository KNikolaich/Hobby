//﻿Программное обеспечение. Для компиляции необходима библиотека OneWire. Скачать можно тут http://go.mysku.ru/?r=http%3A%2F%2Fwww.pjrc.com%2Fteensy%2Farduino_libraries%2FOneWire.zip&key=ms
//Для редактирования кода мастер ключа меняем строчку 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x2F в скетче на свою.
#include <OneWire.h>

#define iButtonPin 11
OneWire ibutton (iButtonPin); // Пин D11 для подлючения iButton (Data)
byte addr[8];
/// 01:FD:14:3D:D:0:0:A3 - Наташин ключ
/// 1:E6:E0:D6:C:0:0:CF - б.Оли ключ
byte keyID[8] = { 0x01, 0x19, 0x90, 0x00, 0x00, 0x00, 0x00, 0xFF };
//{ 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x2F }; // "Универсальный" ключ. Прошивается последовательность 01:FF:FF:FF:FF:FF:FF:2F

const int buttonPin = 6;
const int R_Led = 13;
bool writeflag = false;

void setup() {
  pinMode(R_Led, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
}

void loop() {

  if (digitalRead(buttonPin) == HIGH) {
    writeflag = !writeflag;
    digitalWrite(R_Led, writeflag);
    delay(150);
  }
  if (!ibutton.search (addr)) {
      ibutton.reset_search();
      delay(50);
      return;
    }
    digitalWrite(R_Led, HIGH);
    delay(50);

    for (byte x = 0; x < 8; x++) {
      Serial.print(addr[x], HEX);
      //if (readflag == 0) {
      // ReadID[x] = (addr[x]);
      //}
      Serial.print(":");
    }

    
    byte crc; // Проверка контрольной суммы
    crc = ibutton.crc8(addr, 7);
    Serial.print("CRC: ");
    Serial.println(crc, HEX);
    digitalWrite(R_Led, LOW);

  

  if (writeflag) {
    write2iBtnRW1990_1_2_TM01();
    //send logical 1
    digitalWrite(iButtonPin, LOW); pinMode(iButtonPin, OUTPUT); delayMicroseconds(10);
    pinMode(iButtonPin, INPUT); digitalWrite(iButtonPin, HIGH); delay(10);
    writeflag = false;
    //readflag = 0;
    digitalWrite(R_Led, LOW);
  }
}

bool write2iBtnRW1990_1_2_TM01(){              // функция записи на RW1990.1, RW1990.2, TM-01C(F)
  byte rwCmd, rwFlag = 1;
  rwCmd = 0xD1; rwFlag = 0; // RW1990.1  флаг записи инвертирован
   
  ibutton.reset(); ibutton.write(rwCmd);       // send 0xD1 - флаг записи
  ibutton.write_bit(rwFlag);                   // записываем значение флага записи = 1 - разрешить запись
  delay(10); pinMode(iButtonPin, INPUT);
  ibutton.reset(); ibutton.write(0xD5);        // команда на запись
  for (byte i = 0; i<8; i++){
    digitalWrite(R_Led, !digitalRead(R_Led));
    BurnByte(~keyID[i]);      // запись происходит инверсно для RW1990.1
     
    Serial.print('*');
  } 
  ibutton.write(rwCmd);                     // send 0xD1 - флаг записи
  ibutton.write_bit(!rwFlag);               // записываем значение флага записи = 1 - отключаем запись
  delay(10); pinMode(iButtonPin, INPUT);
  digitalWrite(R_Led, LOW);       
  if (!dataIsBurningOK()){          // проверяем корректность записи
    Serial.println(" The key copy faild");
   
    digitalWrite(R_Led, HIGH);
    return false;
  }
  Serial.println(" The key has copied successesfully");
  
  delay(500);
  digitalWrite(R_Led, HIGH);
  return true;
}


void BurnByte(byte data){
  for(byte n_bit=0; n_bit<8; n_bit++){ 
    ibutton.write_bit(data & 1);  
    delay(5);                        // даем время на прошивку каждого бита до 10 мс
    data = data >> 1;                // переходим к следующему bit
  }
  pinMode(iButtonPin, INPUT);
}

bool dataIsBurningOK(){
  byte buff[8];
  if (!ibutton.reset()) return false;
  ibutton.write(0x33);
  ibutton.read_bytes(buff, 8);
  byte Check = 0;
  for (byte i = 0; i < 8; i++) 
    if (keyID[i] == buff[i]) Check++;      // сравниваем код для записи с тем, что уже записано в ключе.
  if (Check != 8) return false;             // если коды совпадают, ключ успешно скопирован
  return true;
}
