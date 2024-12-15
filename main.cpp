#define BLYNK_TEMPLATE_ID "TMPL6LEvE7DeE"
#define BLYNK_TEMPLATE_NAME "smartgreenhouse"
#define BLYNK_AUTH_TOKEN "SbDSyKbMA791fCOoYtT3xNdla3SL3okF"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Konfigurasi WiFi
char ssid[] = "SSID";     
char pass[] = "Password"; 

// Pin sensor dan aktuator
#define SOIL_SENSOR_PIN 34
#define LIGHT_SENSOR_PIN 35
#define TEMP_SENSOR_PIN 32

#define PUMP_PIN 25
#define LIGHT_CONTROL_PIN 26
#define FAN_PIN 27

int soilMoisture = 0;
int lightIntensity = 0;
float temperature = 0.0;

#define VPIN_SOIL V1
#define VPIN_LIGHT V2
#define VPIN_TEMP V3
#define VPIN_PUMP_CONTROL V4
#define VPIN_LIGHT_CONTROL V5
#define VPIN_FAN_CONTROL V6
#define VPIN_PUMP_STATUS V7

BlynkTimer timer;

void readSensors() {
  //tanah
  soilMoisture = analogRead(SOIL_SENSOR_PIN);
  soilMoisture = map(soilMoisture, 0, 4095, 0, 100);  // Konversi ke persen (0 = kering, 100 = basah)
  soilMoisture = constrain(soilMoisture, 0, 100);     // Membatasi nilai antara 0-100

  // cahaya
  lightIntensity = analogRead(LIGHT_SENSOR_PIN);
  lightIntensity = map(lightIntensity, 0, 4095, 0, 100);  // Konversi ke persen
  lightIntensity = constrain(lightIntensity, 0, 100);

  // Membaca suhu
  int tempValue = analogRead(TEMP_SENSOR_PIN);
  float voltage = (tempValue / 4095.0) * 3.3;  // Konversi ke tegangan dalam Volt
  temperature = voltage * 100.0;               // Konversi ke Celsius

  // Kirim data ke Blynk
  Blynk.virtualWrite(VPIN_SOIL, soilMoisture);
  Blynk.virtualWrite(VPIN_LIGHT, lightIntensity);
  Blynk.virtualWrite(VPIN_TEMP, temperature);

  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.println("%");
  Serial.print("Light Intensity: ");
  Serial.print(lightIntensity);
  Serial.println("%");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
}

BLYNK_WRITE(VPIN_PUMP_CONTROL) {
  int pumpState = param.asInt();
  digitalWrite(PUMP_PIN, pumpState);

  Blynk.virtualWrite(VPIN_PUMP_STATUS, pumpState);
  Serial.print("Pompa Air: ");
  Serial.println(pumpState == HIGH ? "Hidup" : "Mati");
}

BLYNK_WRITE(VPIN_LIGHT_CONTROL) {
  int lightState = param.asInt();
  digitalWrite(LIGHT_CONTROL_PIN, lightState);
}

BLYNK_WRITE(VPIN_FAN_CONTROL) {
  int fanState = param.asInt();
  digitalWrite(FAN_PIN, fanState);
}

void setup() {
  Serial.begin(115200);

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LIGHT_CONTROL_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(LIGHT_CONTROL_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, readSensors);
}

void loop() {
  Blynk.run();
  timer.run();
}
