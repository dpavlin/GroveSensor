#include <Arduino.h>
#include <U8x8lib.h>

#include "DHT.h"

#include <Adafruit_BMP280.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
 

#define DHTPIN 3     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);


Adafruit_BMP280 bmp; // I2C
 
 
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

}
 
void loop(void) {

  float temp, humi;
  temp = dht.readTemperature();
  humi = dht.readHumidity();

  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  //u8x8.print("Temp:");
  u8x8.print(temp);
  u8x8.print("C ");

  temp = bmp.readTemperature();
  u8x8.print( temp );

  u8x8.print(" ");

  u8x8.setCursor(0,9);
  u8x8.print("Humidity:");
  u8x8.print(humi);
  u8x8.print("%");

  float pressure;
  pressure = bmp.readPressure();

  u8x8.setCursor(0,18);
  u8x8.print( pressure );

  u8x8.refreshDisplay();
  delay(200);

}

