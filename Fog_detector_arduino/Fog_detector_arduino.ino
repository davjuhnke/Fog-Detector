//CODE FOR ARDUINO

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
unsigned long delayTime;

void setup() {
    Serial.begin(9600);
    while(!Serial);
    Serial.println(F("BME280 starting..."));

    unsigned status;
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }

    delayTime = 1000;
    Serial.println(F("BME280 ready!"));
    Serial.println();
}

void loop() {
    sendValues();
    delay(delayTime);
}

void sendValues() {
    float temperature = bme.readTemperature();
    float pressure    = bme.readPressure() / 100.0F;
    float altitude    = bme.readAltitude(SEALEVELPRESSURE_HPA);
    float humidity    = bme.readHumidity();

    
    Serial.print("Temp: ");      Serial.print(temperature); Serial.println(" °C");
    Serial.print("Pressure: ");  Serial.print(pressure);    Serial.println(" hPa");
    Serial.print("Altitude: ");  Serial.print(altitude);    Serial.println(" m");
    Serial.print("Humidity: ");  Serial.print(humidity);    Serial.println(" %");
    Serial.println();
}
