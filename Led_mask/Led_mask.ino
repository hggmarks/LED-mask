#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define MIC_PIN A0
#define NUM_OF_LEDS 64

uint32_t color;
int vol = 0;
const uint16_t samples = 128;
Adafruit_NeoPixel matrix(NUM_OF_LEDS, LED_PIN);

void Cartesian_To_Pixel(int row, int col, uint32_t color) {
  int pxl_even =  row * 8 + col;
  int pxl_odd = row * 8 + (7 - col);
  (row % 2) == 1 ? matrix.setPixelColor(pxl_odd, color) : matrix.setPixelColor(pxl_even, color);
}

const byte mask[] = {
                     B00000001,
                     B00000010,
                     B00000100,
                     B00001000,
                     B00010000,
                     B00100000,
                     B01000000,
                     B10000000
                    };

byte mouth [][8] = {
  {
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B11111111,
    B00000000,
    B00000000,
    B00000000
  }, {
    B00000000,
    B00000000,
    B00011000,
    B11100111,
    B11100111,
    B00011000,
    B00000000,
    B00000000
  }, {
    B00000000,
    B00000000,
    B01111110,
    B10000001,
    B10000001,
    B01111110,
    B00000000,
    B00000000
  }, {
    B00000000,
    B00111100,
    B01000010,
    B11000011,
    B11000011,
    B01000010,
    B00111100,
    B00000000
  }, {
    B00111100,
    B01000010,
    B10000001,
    B10000001,
    B10000001,
    B10000001,
    B01000010,
    B00111100
  }, {
    B00000000,
    B00000000,
    B10000001,
    B11000011,
    B01111110,
    B00111100,
    B00000000,
    B00000000
  }, {
    B00000000,
    B00000000,
    B00111100,
    B01111110,
    B11000011,
    B10000001,
    B00000000,
    B00000000
  }, {
    B00000000,
    B00000000,
    B00111100,
    B01111110,
    B11000011,
    B10000001,
    B00000000,
    B00000000
  }, {
    B00000000,
    B01100110,
    B11111111,
    B11111111,
    B01111110,
    B00111100,
    B00011000,
    B00000000
  }, {
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B00111100,
    B00111100,
    B00011000,
    B00000000
  }
};

void draw_mouth(byte mouth_shape[], uint32_t color) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++)
    {
      mouth_shape[i] & mask[j] ? Cartesian_To_Pixel(i, j, color) : Cartesian_To_Pixel(i, j, 0);
    }
  }
  matrix.show();
}

void setup() {
  randomSeed(analogRead(A5));
  Serial.begin(9600);
  matrix.begin();
  matrix.setBrightness(50);
  matrix.show();
  color = matrix.Color(random(256), random(256), random(256));
}

void loop() {
  float newvol = 0;
  float vol_medium = 0;

  for (int i = 0; i < samples; i++) {
    int micdata = analogRead(MIC_PIN);
    newvol += micdata;
  }

  vol_medium = newvol / samples;
  vol = (vol_medium + vol) / 2.0;

    switch (vol) {
      case 0 ... 45:
        draw_mouth(mouth[0], color);
        break;
      case 46 ... 60:
        draw_mouth(mouth[1], color);
        break;
      case 61 ... 80:
        draw_mouth(mouth[2], color);
        break;
      case 81 ... 120:
        draw_mouth(mouth[3], color);
        break;
      case 121 ... 200:
        draw_mouth(mouth[4], color);
        delay(100);
        break;
    }
  }
