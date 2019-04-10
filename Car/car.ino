
#define G 9.8
#define SCALE_2G    0b00
#define SCALE_4G    0b01
#define SCALE_8G    0b10
#define SCALE_16G   0b11
#define MAX_ABS_VAL 16535.0

void IMU_init();
void IMU_read();
void IMU_set_accel_scale(uint8_t scale);

double IMU_get_accel_X();
double IMU_get_accel_Y();
double IMU_get_accel_Z();

#include <SPI.h>
#define TFT_DC    9
#define TFT_RST   10 
#define TFT_MOSI  11   // for hardware SPI data pin (all of available pins)
#define TFT_SCLK  13   // for hardware SPI sclk pin (all of available pins)

#include <Adafruit_GFX.h>    // Core graphics library by Adafruit
#include <Arduino_ST7789.h> // Hardware-specific library for ST7789 (with or without CS pin)
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST); //for display without CS pin

void setup(void)
{
    Serial.begin(9600);
    IMU_init();
    //IMU_set_accel_scale(SCALE_2G);
    tft.init(240, 240);   // initialize a ST7789 chip, 240x240 pixels
    //IMU_set_filter(); // donne une sorte de dérivée des mouvements

    delay(500);
    tft.setTextWrap(false);
    tft.fillScreen(BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.println("Start CAR");

    delay(100);
    draw_Figure();
}



/***************************DISPLAY***************************/

void draw_Figure()
{
  tft.fillScreen(BLACK);
  tft.fillCircle(120,100,5,WHITE);
  tft.drawCircle(120,100,50,WHITE);
  tft.drawCircle(120,100,100,WHITE);
}

void draw_accel(double X, double Y)//in G
{
  static double oldX = 0; static double oldY = 0;
  static bool first = true;
  if(!first)
    tft.fillCircle(oldX+120, oldY*100+100, 5, BLACK);
  if(first)
    first = false;
  tft.fillCircle(X+120, Y+100, 5, RED);

  tft.fillRect(0, 210, 240, 18, BLACK);//StartX, StartY, WidthX, HeightY
  tft.setCursor(0,210);
  tft.print(abs(X)); tft.println("G");
  tft.setCursor(180,210);
  tft.print(abs(Y)); tft.println("G");

  oldX = X; oldY = Y;
}



/***************************END DISPLAY*******************/

/***************************IMU***************************/
#include <Wire.h>
#include <LSM303.h>

typedef struct acceleration * Acceleration;
typedef struct compass * Compass;
struct acceleration {
  double X;
  double Y;
  double Z;
};

struct compass {
  int X;
  int Y;
  int Z;
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

double IMU_get_accel_X()
{
  switch(current_scale)
  {
    case SCALE_2G:
      return accel->X / MAX_ABS_VAL;
    case SCALE_4G:
      return accel->X / MAX_ABS_VAL * 2;
    case SCALE_8G:
      return accel->X / MAX_ABS_VAL * 4;
    case SCALE_16G:
      return accel->X / MAX_ABS_VAL * 8;
    default:
      return accel->X / MAX_ABS_VAL;
  }
}
double IMU_get_accel_Y()
{
  switch(current_scale)
  {
    case SCALE_2G:
      return accel->Y / MAX_ABS_VAL;
    case SCALE_4G:
      return accel->Y / MAX_ABS_VAL * 2;
    case SCALE_8G:
      return accel->Y / MAX_ABS_VAL * 4;
    case SCALE_16G:
      return accel->Y / MAX_ABS_VAL * 8;
    default:
      return accel->Y / MAX_ABS_VAL;
  }
}
double IMU_get_accel_Z()
{
  switch(current_scale)
  {
    case SCALE_2G:
      return accel->Z / MAX_ABS_VAL;
    case SCALE_4G:
      return accel->Z / MAX_ABS_VAL * 2;
    case SCALE_8G:
      return accel->Z / MAX_ABS_VAL * 4;
    case SCALE_16G:
      return accel->Z / MAX_ABS_VAL * 8;
    default:
      return accel->Z / MAX_ABS_VAL;
  }
}



void loop(void)
{
  delay(100);
  IMU_read();
  draw_accel(IMU_get_accel_X(),IMU_get_accel_Y());
  Serial.print("X "); Serial.print(compass.a.x); Serial.print("\tY "); Serial.print(compass.a.y); Serial.print("\tZ "); Serial.print(compass.a.z); 
  Serial.print("\tX "); Serial.print(IMU_get_accel_X()); Serial.print("\tY "); Serial.print(IMU_get_accel_Y()); Serial.print("\tZ "); Serial.println(IMU_get_accel_Z()); 
}
