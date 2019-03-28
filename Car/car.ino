
#define G 9.8
#define SCALE_2G    0b00
#define SCALE_4G    0b01
#define SCALE_8G    0b10
#define SCALE_16G   0b11

void IMU_init();
void IMU_read();
void IMU_set_accel_scale(uint8_t scale);

float IMU_get_accel_X();
float IMU_get_accel_Y();
float IMU_get_accel_Z();


void setup(void)
{
    Serial.begin(9600);
    IMU_init();
    IMU_set_accel_scale(SCALE_2G);
    //IMU_set_filter(); // donne une sorte de dérivée des mouvements
}

void loop(void)
{
  IMU_read();
  Serial.println(IMU_get_accel_X());
  delay(50);
}



/***************************IMM***************************/
#include <Wire.h>
#include <LSM303.h>

typedef struct acceleration * Acceleration;
typedef struct compass * Compass;
struct acceleration {
  int16_t X;
  int16_t Y;
  int16_t Z;
};

struct compass {
  int16_t X;
  int16_t Y;
  int16_t Z;
};

LSM303 compass;

Acceleration accel;
uint8_t current_scale = SCALE_2G;

void IMU_init()
{
  Wire.begin();
  compass.init();
  compass.enableDefault();
}



void IMU_read()
{
  compass.read();
  accel->X = compass.a.x;
  accel->Y = compass.a.y;
  accel->Z = compass.a.z;
}

void IMU_set_filter()
{
  int reg = compass.readReg(0x21);
  reg |= 0x08;
  compass.writeReg(0x21, reg);
}

void IMU_set_accel_scale(uint8_t scale)
{
  switch (scale)
  {
    case SCALE_2G:
    case SCALE_4G:
    case SCALE_8G:
    case SCALE_16G:
      current_scale = scale;
      break;
    default:
      current_scale = SCALE_2G;
      break;
  }
  compass.writeReg(0x23, current_scale + 8);
  Serial.println(current_scale << 4);
}

float IMU_get_accel_X()
{
  switch(current_scale)
  {
    case SCALE_2G:
      return G / 16535 * accel->X;
    case SCALE_4G:
      return 4*G / 16535 * accel->X;
    case SCALE_8G:
      return 8*G / 16535 * accel->X;
    case SCALE_16G:
      return 16*G / 16535 * accel->X;
  }
}
float IMU_get_accel_Y()
{
  switch(current_scale)
  {
    case SCALE_2G:
      return G / 16535 * accel->Y;
    case SCALE_4G:
      return 2 * G / 16535 * accel->Y;
    case SCALE_8G:
      return 4 * G / 16535 * accel->Y;
    case SCALE_16G:
      return 16 * G / 16535 * accel->Y;
  }
}
float IMU_get_accel_Z()
{
  switch(current_scale)
  {
    case SCALE_2G:
      return G / 16535 * accel->Z;
    case SCALE_4G:
      return 2 * G / 16535 * accel->Z;
    case SCALE_8G:
      return 4 * G / 16535 * accel->Z;
    case SCALE_16G:
      return 16 * G / 16535 * accel->Z;
  }
}
