//MPU9250 Library
#include <Wire.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU9250.h"

#include <ESP8266WiFi.h>
#define DHTPIN 5     // what digital pin we're connected to

#define WIFI_SSID "jihun" //Wifi ID
#define WIFI_PASSWORD "12345678" // Wifi Password

//MPIU9250 Parameter
MPU9250 accelgyro;
I2Cdev   I2C_M;
uint8_t buffer_m[6];
  
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

float Axyz[3];
float Gxyz[3];
float Mxyz[3];
#define sample_num_mdate  5000
volatile float mx_sample[3];
volatile float my_sample[3];
volatile float mz_sample[3];

static float mx_centre = 0;
static float my_centre = 0;
static float mz_centre = 0;

volatile int mx_max = 0;
volatile int my_max = 0;
volatile int mz_max = 0;

volatile int mx_min = 0;
volatile int my_min = 0;
volatile int mz_min = 0;

float temperature;
float pressure;
float atm;
float altitude;


void setup() {

  //MPU9250 Setup
  Wire.begin();
  Serial.begin(115200);
  accelgyro.initialize();


  //Wifi Setup
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

//n은 데이터 갯수
//cnt는 데이터 200개 기준으로 Firebase에 보내는 횟수

void loop() {
  delay(10);

  getAccel_Data();
  getGyro_Data();
  getCompassDate_calibrated();  

  //반올림 후 roll, pitch, yaw를 문자열에 저장 후 200개를 채우면 그 때 firebase로 push하는 알고리즘의 시간은 대략 60초에 800~1000개. => delay(50~80) 정도 됨
  float Ax = Axyz[0];
  float Ay = Axyz[1];
  float Az = Axyz[2];
  float Gx = Gxyz[0] / 57.3;
  float Gy = Gxyz[1] / 57.3;
  float Gz = Gxyz[2] / 57.3;
  float Mx = Mxyz[0];
  float My = Mxyz[1];
  float Mz = Mxyz[2];

  //Roll, Pitch, Yaw는 소수점 둘째짜리까지 짜름.
  float pitch = (round(atan2 (Ay ,( sqrt ((Ax * Ax) + (Az * Az)))) * 57.3) * 1000) / 1000;
  float roll = (round(atan2(-Ax ,( sqrt((Ay * Ay) + (Az * Az)))) * 57.3) * 1000) / 1000;
  // yaw from mag
  float Yh = (My * cos(roll)) - (Mz * sin(roll));
  float Xh = (Mx * cos(pitch))+(My * sin(roll)*sin(pitch)) + (Mz * cos(roll) * sin(pitch));
  float yaw =  (round(atan2(Yh, Xh) * 57.3) * 1000) / 1000;
  
  //하나의 roll, pitch, yaw를 하나의 string값으로 저장.

  Serial.print(roll);
  Serial.print(", ");
  Serial.print(pitch);
  Serial.print(", ");
  Serial.print(yaw);
  Serial.println();

}

void getAccel_Data(void)
{
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    Axyz[0] = (double) ax / 16384;
    Axyz[1] = (double) ay / 16384;
    Axyz[2] = (double) az / 16384;
}

void getGyro_Data(void)
{
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    Gxyz[0] = (double) gx * 250 / 32768;
    Gxyz[1] = (double) gy * 250 / 32768;
    Gxyz[2] = (double) gz * 250 / 32768;
}

void getCompass_Data(void)
{
    I2C_M.writeByte(MPU9150_RA_MAG_ADDRESS, 0x0A, 0x01); //enable the magnetometer
    delay(10);
    I2C_M.readBytes(MPU9150_RA_MAG_ADDRESS, MPU9150_RA_MAG_XOUT_L, 6, buffer_m);

    mx = ((int16_t)(buffer_m[1]) << 8) | buffer_m[0] ;
    my = ((int16_t)(buffer_m[3]) << 8) | buffer_m[2] ;
    mz = ((int16_t)(buffer_m[5]) << 8) | buffer_m[4] ;

    Mxyz[0] = (double) mx * 1200 / 4096;
    Mxyz[1] = (double) my * 1200 / 4096;
    Mxyz[2] = (double) mz * 1200 / 4096;
}

void getCompassDate_calibrated ()
{
    getCompass_Data();
    Mxyz[0] = Mxyz[0] - mx_centre;
    Mxyz[1] = Mxyz[1] - my_centre;
    Mxyz[2] = Mxyz[2] - mz_centre;
}