#include <ESP8266WiFi.h>
 
const char* ssid     = "jihun";     // 공유기 SSID
const char* password = "11111111"; // 비밀번호
 
const char* host = "www.google.com";    // 서버 호스트 주소
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void loop() {
}
