#include <Arduino.h>
#include <U8x8lib.h>

#include "RunningAverage.h"

RunningAverage temp_avg(10);
RunningAverage hum_avg(10);

#include "DHT.h"

#include <Adafruit_BMP280.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
 

#define DHTPIN 3     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);


Adafruit_BMP280 bmp; // I2C


#include "LIS3DHTR.h"
#include <Wire.h>
LIS3DHTR<TwoWire> LIS; //IIC
#define WIRE Wire


#define BUTTON_PIN 6 

#define SOUND_PIN A2
#define LIGHT_PIN A6
 
void setup(void) {
  //u8x8.setBusClock(100000);  // If you breakout other modules, please enable this line
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFlipMode(1);

  Serial.begin(115200);

  dht.begin();

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor!"));
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  pinMode(BUTTON_PIN, OUTPUT);
  pinMode(SOUND_PIN, INPUT);
  pinMode(LIGHT_PIN, INPUT);


  LIS.begin(WIRE, 0x19);
  LIS.openTemp();
  delay(100);
  LIS.setFullScaleRange(LIS3DHTR_RANGE_2G);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_5KHZ);
  LIS.setHighSolution(true); //High solution enable

}

unsigned long t;
 
void loop(void) {

  t = millis();

  int oled_active = ! digitalRead(BUTTON_PIN);
  u8x8.setPowerSave( oled_active );

  temp_avg.addValue( dht.readTemperature() );
  hum_avg.addValue( dht.readHumidity() );
  float temp = temp_avg.getAverage();
  float humi = hum_avg.getAverage();

  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  //u8x8.print("Temp:");
  u8x8.print(temp);
  u8x8.print("C ");
  Serial.print("dht_temp=");
  Serial.print(temp);

  temp = bmp.readTemperature();
  u8x8.print( temp );
  Serial.print(",bmp_temp=");
  Serial.print(temp);

  u8x8.print(" ");

  u8x8.setCursor(0,1 * 9);
  u8x8.print("Humidity:");
  u8x8.print(humi);
  u8x8.print("%");

  Serial.print(",dht_humidity=");
  Serial.print(humi);

  float pressure;
  pressure = bmp.readPressure();

  u8x8.setCursor(0,2 * 9);
  u8x8.print( pressure );
  Serial.print(",bmp_pressure=");
  Serial.print(pressure);

  unsigned long sumSquare;
  for(int i=0; i++; i<1000) {
    int sound = analogRead(SOUND_PIN);
    sumSquare += sound * sound;
  }
  int rms_sound = sqrt(sumSquare / 1000);
  Serial.print(",rms_sound=");
  Serial.print(rms_sound);

  int sound = analogRead(SOUND_PIN);
  u8x8.setCursor(0,3 * 9);
  u8x8.print( sound );
  Serial.print(",sound=");
  Serial.print(sound);

  int light = analogRead(LIGHT_PIN);
  u8x8.setCursor(0,3 * 9);
  u8x8.print( light );
  Serial.print(",light=");
  Serial.print(light);

  u8x8.refreshDisplay();

  float x = 0, y = 0, z = 0;
  for(int i=0; i<=300; i++) {
	x += LIS.getAccelerationX();
	y += LIS.getAccelerationY();
	z += LIS.getAccelerationZ();
  }

  Serial.print(",x="); Serial.print(x);
  Serial.print(",y="); Serial.print(y);
  Serial.print(",z="); Serial.print(z);

  Serial.println();

  t = t + 1000 - millis();
  delay(t);

}

