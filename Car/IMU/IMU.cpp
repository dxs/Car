/*
The sensor outputs provided by the library are the raw 16-bit values
obtained by concatenating the 8-bit high and low accelerometer and
magnetometer data registers. They can be converted to units of g and
gauss using the conversion factors specified in the datasheet for your
particular device and full scale setting (gain).

Example: An LSM303D gives a magnetometer X axis reading of 1982 with
its default full scale setting of +/- 4 gauss. The M_GN specification
in the LSM303D datasheet (page 10) states a conversion factor of 0.160
mgauss/LSB (least significant bit) at this FS setting, so the raw
reading of -1982 corresponds to 1982 * 0.160 = 317.1 mgauss =
0.3171 gauss.

In the LSM303DLHC, LSM303DLM, and LSM303DLH, the acceleration data
registers actually contain a left-aligned 12-bit number, so the lowest
4 bits are always 0, and the values should be shifted right by 4 bits
(divided by 16) to be consistent with the conversion factors specified
in the datasheets.

Example: An LSM303DLH gives an accelerometer Z axis reading of -16144
with its default full scale setting of +/- 2 g. Dropping the lowest 4
bits gives a 12-bit raw value of -1009. The LA_So specification in the
LSM303DLH datasheet (page 11) states a conversion factor of 1 mg/digit
at this FS setting, so the value of -1009 corresponds to -1009 * 1 =
1009 mg = 1.009 g.
*/

#include <Wire.h>
#include <LSM303.h>
#include "IMU.h"

#define G 9.8

struct acceleration {
  uint16_t X;
  uint16_t Y;
  uint16_t Z;
};

struct compass {
  uint16_t X;
  uint16_t Y;
  uint16_t Z;
};

LSM303 compass;

Acceleration accel;
Compass com;
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
  accel.X = compas.a.x;
  accel.Y = compas.a.y;
  accel.Z = compas.a.z;
  com.X = compas.m.x;
  com.Y = compas.m.y;
  com.Z = compas.m.z;
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
  compass.writeReg(0x23, current_scale << 4); 
}

float IMU_get_accel_X()
{
  switch(current_scale)
  {
    case SCALE_2G:
      return G / 16535 * accel.X;
    case SCALE_4G:
      return 2 * G / 16535 * accel.X;
    case SCALE_8G:
      return 4 * G / 16535 * accel.X;
    case SCALE_16G:
      return 16 * G / 16535 * accel.X;
  }
}
float IMU_get_accel_Y()
{
  switch(current_scale)
  {
    case SCALE_2G:
      return G / 16535 * accel.Y;
    case SCALE_4G:
      return 2 * G / 16535 * accel.Y;
    case SCALE_8G:
      return 4 * G / 16535 * accel.Y;
    case SCALE_16G:
      return 16 * G / 16535 * accel.Y;
  }
}
float IMU_get_accel_Z()
{
  switch(current_scale)
  {
    case SCALE_2G:
      return G / 16535 * accel.Z;
    case SCALE_4G:
      return 2 * G / 16535 * accel.Z;
    case SCALE_8G:
      return 4 * G / 16535 * accel.Z;
    case SCALE_16G:
      return 16 * G / 16535 * accel.Z;
  }
}
