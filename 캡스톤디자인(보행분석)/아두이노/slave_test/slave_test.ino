#include <SoftwareSerial.h>



SoftwareSerial BTSerial(2, 3);



void setup() {

  Serial.begin(9600);

  pinMode(13, OUTPUT);

  BTSerial.begin(9600);// set the data rate for the BT port

}



 



void loop() {



    byte data;

    data = BTSerial.read();

    if(data=='a'){

      digitalWrite(13, HIGH);   // set the LED on

      Serial.println("a");

   }



    if(data=='b'){

      digitalWrite(13, LOW);    // set the LED off

      Serial.println("b");

    }

}
