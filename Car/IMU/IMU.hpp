#ifndef IMU_H
#define IMU_H

typedef struct acceleration * Acceleration;

#define SCALE_2G    0
#define SCALE_4G    1
#define SCALE_8G    2
#define SCALE_16G   3

void IMU_init();
void IMU_read();
void IMU_set_accel_scale(uint8_t scale);

float IMU_get_accel_X();
float IMU_get_accel_Y();
float IMU_get_accel_Z();


#endif