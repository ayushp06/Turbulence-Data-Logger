#include <Adafruit_MPL3115A2.h>
#include <Adafruit_INA219.h>
#include <Adafruit_LSM6DSOX.h>
#include <SD.h>
#include <SPI.h>

Adafruit_INA219 ina219;
Adafruit_MPL3115A2 baro;
Adafruit_LSM6DSOX sox;
const int chipSelect = BUILTIN_SDCARD;
const int buzzer = 30;

void setup() {

  pinMode(buzzer, 30);

  if (!baro.begin()) {
    tone(buzzer, 1000);
    while(1);
  }

  if (!ina219.begin()) {
    tone(buzzer, 1000);
    while (1);
  }

  if (!sox.begin_I2C()) {
    tone(buzzer, 1000);
    while (1);
  }

  if(!SD.begin(chipSelect)) {
    tone(buzzer, 1000);
    while (1);
  }

  tone(buzzer, 1000);
  delay(1000);
  noTone(buzzer);
  delay(1000);

  baro.setSeaPressure(1028.48);

  File dataFile = SD.open("FlightData.txt", FILE_WRITE);

  if (SD.exists("FlightData.txt")) {
    SD.remove("FlightData.txt");
  }
}

void dataRecord() {

  File dataFile = SD.open("FlightData.txt", FILE_WRITE);
  
  float pressure = baro.getPressure();
  float altitude = baro.getAltitude();
  float temperature = baro.getTemperature();
  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();
  float power_mW = ina219.getPower_mW();
  float loadvoltage = busvoltage + (shuntvoltage / 1000);

  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);

  float accelX = accel.acceleration.x;
  float accelY = accel.acceleration.y;
  float accelZ = accel.acceleration.z;
  float gyroX = gyro.gyro.x;
  float gyroY = gyro.gyro.y;
  float gyroZ = gyro.gyro.z;

  dataFile.print("Pressure: "); dataFile.print(pressure); dataFile.println(" hPa");
  dataFile.print("Altitute: "); dataFile.print(altitude); dataFile.println(" m");
  dataFile.print("Temperature: "); dataFile.print(temperature); dataFile.println(" C");
  dataFile.print("Bus Voltage: "); dataFile.print(busvoltage); dataFile.println(" V");
  dataFile.print("Shunt Voltage: "); dataFile.print(shuntvoltage); dataFile.println(" mV");
  dataFile.print("Load Voltage: "); dataFile.print(loadvoltage); dataFile.println(" V");
  dataFile.print("Current: "); dataFile.print(current_mA); dataFile.println(" mA");
  dataFile.print("Power: "); dataFile.print(power_mW); dataFile.println(" mW");
  dataFile.print("Acceleration (X): "); dataFile.print(accelX); dataFile.println(" m/s^2");
  dataFile.print("Acceleration (Y): "); dataFile.print(accelY); dataFile.println(" m/s^2");
  dataFile.print("Acceleration (Z): "); dataFile.print(accelZ); dataFile.println(" m/s^2");
  dataFile.print("Gyro (X): "); dataFile.print(gyroX); dataFile.println(" rad/s");
  dataFile.print("Gyro (Y): "); dataFile.print(gyroY); dataFile.println(" rad/s");
  dataFile.print("Gyro (Z): "); dataFile.print(gyroZ); dataFile.println(" rad/s");
  dataFile.println("-----------------------------------");
}

void loop() {

  File dataFile = SD.open("FlightData.txt", FILE_WRITE);

  if (dataFile) {
    dataRecord();
    dataFile.close();
  }

  else {
    tone(buzzer, 1000);
  }

  tone(buzzer, 500);
  delay(500);
  noTone(buzzer);
  delay(500);
}