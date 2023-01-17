/*
  238-2-2 HC06(슬레이브코드)
 */
#include <SoftwareSerial.h>

SoftwareSerial HC06(2, 3); // RX, TX

void setup() {
  Serial.begin(9600); //PC-아두이노간
  HC06.begin(9600); //아두이노-블루투스모듈
}

void loop() { // run over and over
  if(HC06.available()){
    String text = HC06.readStringUntil(0x0D);

    Serial.println(text);
  }
}
