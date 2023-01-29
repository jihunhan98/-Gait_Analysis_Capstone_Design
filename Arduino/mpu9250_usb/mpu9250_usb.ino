#include <Wire.h>

//MPU9250 => SCL: A5, SCA: A4
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU9250.h"

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

void setup()
{
    Wire.begin();
    Serial.begin(115200); //PC-아두이노간
    accelgyro.initialize();

    delay(50);
}

void loop()
{

    getAccel_Data();
    getGyro_Data();
    getCompassDate_calibrated();  

    float Ax = Axyz[0];
    float Ay = Axyz[1];
    float Az = Axyz[2];
    float Gx = Gxyz[0] / 57.3;
    float Gy = Gxyz[1] / 57.3;
    float Gz = Gxyz[2] / 57.3;
    float Mx = Mxyz[0];
    float My = Mxyz[1];
    float Mz = Mxyz[2];

    float pitch = atan2 (Ay ,( sqrt ((Ax * Ax) + (Az * Az))));
    float roll = atan2(-Ax ,( sqrt((Ay * Ay) + (Az * Az))));

   // yaw from mag

    float Yh = (My * cos(roll)) - (Mz * sin(roll));
    float Xh = (Mx * cos(pitch))+(My * sin(roll)*sin(pitch)) + (Mz * cos(roll) * sin(pitch));
    float yaw =  atan2(Yh, Xh);

    // Serial.print("Roll: ");
    Serial.print(roll*57.3);
    Serial.print(',');
    // Serial.print(".    Pitch: ");
    Serial.print(pitch*57.3);
    Serial.print(',');
    // Serial.print(".    yaw: ");
    Serial.print(yaw*57.3);
    Serial.println();
    

    delay(10);

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
