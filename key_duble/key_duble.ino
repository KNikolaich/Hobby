//﻿Программное обеспечение. Для компиляции необходима библиотека OneWire. Скачать можно тут http://go.mysku.ru/?r=http%3A%2F%2Fwww.pjrc.com%2Fteensy%2Farduino_libraries%2FOneWire.zip&key=ms
//Для редактирования кода мастер ключа меняем строчку 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x2F в скетче на свою.
#include <OneWire.h>

#define pin 11
OneWire ibutton (pin); // Пин D11 для подлючения iButton (Data)
byte addr[8];
/// 01:FD:14:3D:D:0:0:A3 - Наташин ключ
/// 1:E6:E0:D6:C:0:0:CF - б.Оли ключ
byte ReadID[8] = { 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x2F }; // "Универсальный" ключ. Прошивается последовательность 01:FF:FF:FF:FF:FF:FF:2F

const int buttonPin = 6;
const int ledPin = 13;
int writeflag = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {

  if (digitalRead(buttonPin) == HIGH) {
    writeflag = 1;
    digitalWrite(ledPin, HIGH);
  }
  if (!ibutton.search (addr)) {
      ibutton.reset_search();
      delay(50);
      return;
    }
    digitalWrite(ledPin, HIGH);
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
    digitalWrite(ledPin, LOW);

  

  if (writeflag == 1) {
    writeIntoKey();
    //send logical 1
    digitalWrite(pin, LOW); pinMode(pin, OUTPUT); delayMicroseconds(10);
    pinMode(pin, INPUT); digitalWrite(pin, HIGH); delay(10);
    writeflag = 0;
    //readflag = 0;
    digitalWrite(ledPin, LOW);
  }
}

void writeIntoKey() {
  Serial.print("Start programming..."); // начало процесса записи данных в ключ

  for (int i = 0; i < 8; i++) {
    ibutton.reset(); // сброс ключа и формирование 4-х байт для записи в ключ

    byte data[4]; // массив для хранения данных ключа
    data[0] = 0x3C; // отправляем команду "запись"
    data[1] = i; // указываем байт для записи
    data[2] = 0;
    data[3] = ReadID[i];
    ibutton.write_bytes(data, 4); // записываем i-ый байт в ключ

    Serial.println("write_bytes: "); // сообщение о записи байт

    Serial.print(ReadID[i]); // сообщение о записи байт
    Serial.print(' ');

    uint8_t b = ibutton.read(); // считываем байт из ключа

    if (OneWire::crc8(data, 4) != b) { // при ошибке контрольной суммы
      Serial.println("Error while programming!"); // сообщаем об этом
      return; // и отменяем запись ключа
    }
    send_programming_impulse(); // если всё хорошо, посылаем импульс для записи i-го байта в ключ
  }
  Serial.println("Success!"); // сообщение об успешной записи данных в ключ
}

// Инициализация записи данных в ключ-таблетку iButton:
void send_programming_impulse() {
  digitalWrite(pin, HIGH);
  delay(60);
  digitalWrite(pin, LOW);
  delay(5);
  digitalWrite(pin, HIGH);
  delay(50);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void writeIntoKeyOld() {
  ibutton.skip(); ibutton.reset(); ibutton.write(0x33);
  Serial.print("  ID before write:");
  for (byte x = 0; x < 8; x++) {
    Serial.print(' ');
    Serial.print(ibutton.read(), HEX);
  }
  // send reset
  ibutton.skip();
  ibutton.reset();
  // send 0xD1
  ibutton.write(0xD1);
  // send logical 0
  digitalWrite(pin, LOW); pinMode(pin, OUTPUT); delayMicroseconds(60);
  pinMode(pin, INPUT); digitalWrite(pin, HIGH); delay(10);

  Serial.print('\n');
  Serial.print("  Writing iButton ID:\n    ");
  byte newID[8] = { (ReadID[0]), (ReadID[1]), (ReadID[2]), (ReadID[3]), (ReadID[4]), (ReadID[5]), (ReadID[6]), (ReadID[7]) };
  ibutton.skip();
  ibutton.reset();
  ibutton.write(0xD5);
  for (byte x = 0; x < 8; x++) {
    writeByte(newID[x]);
    Serial.print('*');
  }
  Serial.print('\n');
  ibutton.reset();
  // send 0xD1
  ibutton.write(0xD1);
}
int writeByte(byte data) {
  int data_bit;
  for (data_bit = 0; data_bit < 8; data_bit++) {
    if (data & 1) {
      digitalWrite(pin, LOW); pinMode(pin, OUTPUT);
      delayMicroseconds(60);
      pinMode(pin, INPUT); digitalWrite(pin, HIGH);
      delay(10);
    } else {
      digitalWrite(pin, LOW); pinMode(pin, OUTPUT);
      pinMode(pin, INPUT); digitalWrite(pin, HIGH);
      delay(10);
    }
    data = data >> 1;
  }
  return 0;
}
