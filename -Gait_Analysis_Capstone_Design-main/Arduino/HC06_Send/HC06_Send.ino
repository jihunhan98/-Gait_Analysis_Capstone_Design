#include <Wire.h>
#include <MPU9250_asukiaaa.h>
#include <SoftwareSerial.h>

MPU9250_asukiaaa mpu9250;

float aX, aY, aZ, aSqrt;  //가속도 (Accel)
float gX, gY, gZ;   //각속도 (GyroScope)
uint16_t mX, mY, mZ;    //지자기 (magnetic)
SoftwareSerial HC06(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  HC06.begin(9600);
  Wire.begin();

  mpu9250.setWire(&Wire);
  mpu9250.beginAccel();
  mpu9250.beginGyro();
  mpu9250.beginMag();
  
}

void loop() {

  //Accel
  mpu9250.accelUpdate();
  aX = mpu9250.accelX();
  aY = mpu9250.accelY();
  aZ = mpu9250.accelZ();

  //gyroScope
  mpu9250.gyroUpdate();
  gX = mpu9250.gyroX();
  gY = mpu9250.gyroY();
  gZ = mpu9250.gyroZ();

  //magnetic 
  mpu9250.magUpdate();
  mX = mpu9250.magX();
  mY = mpu9250.magY();
  mZ = mpu9250.magZ();

  float roll = atan2(aY, aZ);
  float pitch = atan2(-aX, sqrt(aY * aY + aZ * aZ));
  float yaw = atan2(mY, mX) + PI;

  float declinationAngle = 0.22; // replace with your local declination angle
  yaw += declinationAngle;

  // Wrap yaw angle between -180 and 180 degrees
  if (yaw > PI) yaw -= 2 * PI;
  else if (yaw < -PI) yaw += 2 * PI;


  //RAD_TO_DEG는 라디안에서 각도로 변경할 수 있는 변환 계수 (Radius to Degree)
  HC06.println(roll * RAD_TO_DEG);

  //delay를 줄이면 줄일 수록 굴곡이 많아짐 (아마 정확한 값 측정도 불가능할 뿐더러 0.04초마다 점을 찍으니깐 선의 굴곡이 많ㅇ ㅣ만들어짐)
  //delay(50)이 가장 적당한듯
  delay(50);
}#include <Wire.h>
#include <MPU9250_asukiaaa.h>
#include <SoftwareSerial.h>

MPU9250_asukiaaa mpu9250;

float aX, aY, aZ, aSqrt;  //가속도 (Accel)
float gX, gY, gZ;   //각속도 (GyroScope)
uint16_t mX, mY, mZ;    //지자기 (magnetic)
SoftwareSerial HC06(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  HC06.begin(9600);
  Wire.begin();

  mpu9250.setWire(&Wire);
  mpu9250.beginAccel();
  mpu9250.beginGyro();
  mpu9250.beginMag();
  
}

void loop() {

  //Accel
  mpu9250.accelUpdate();
  aX = mpu9250.accelX();
  aY = mpu9250.accelY();
  aZ = mpu9250.accelZ();

  //gyroScope
  mpu9250.gyroUpdate();
  gX = mpu9250.gyroX();
  gY = mpu9250.gyroY();
  gZ = mpu9250.gyroZ();

  //magnetic 
  mpu9250.magUpdate();
  mX = mpu9250.magX();
  mY = mpu9250.magY();
  mZ = mpu9250.magZ();

  float roll = atan2(aY, aZ);
  float pitch = atan2(-aX, sqrt(aY * aY + aZ * aZ));
  float yaw = atan2(mY, mX) + PI;

  float declinationAngle = 0.22; // replace with your local declination angle
  yaw += declinationAngle;

  // Wrap yaw angle between -180 and 180 degrees
  if (yaw > PI) yaw -= 2 * PI;
  else if (yaw < -PI) yaw += 2 * PI;


  //RAD_TO_DEG는 라디안에서 각도로 변경할 수 있는 변환 계수 (Radius to Degree)
  HC06.println(roll * RAD_TO_DEG);

  //delay를 줄이면 줄일 수록 굴곡이 많아짐 (아마 정확한 값 측정도 불가능할 뿐더러 0.04초마다 점을 찍으니깐 선의 굴곡이 많ㅇ ㅣ만들어짐)
  //delay(50)이 가장 적당한듯
  delay(50);
}
