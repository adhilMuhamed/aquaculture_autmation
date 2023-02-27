
#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#define SensorPin A2
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
char auth[] = "2UdWgOOgxA79Co1TK8ZOyhnjPHjAnfL-";

char ssid[] = "iball";
char pass[] = "0987654321";

unsigned long int avgValue;
float b;
int buf[10], temp;
float phValue;
float Caliberation = 3;
float TEMP;

SoftwareSerial EspSerial(2, 3); // RX, TX

#define ESP8266_BAUD 9600
ESP8266 wifi(&EspSerial);
BlynkTimer timer;

void myTimerEvent()
{
  Blynk.virtualWrite(V0, phValue);
  Blynk.virtualWrite(V1, TEMP);
  
}

void setup()
{
  Serial.begin(9600);
  sensors.begin();
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  
  Blynk.run();
  timer.run();
  PH();
  Temp();
  
}
void Temp(){
  sensors.requestTemperatures();
  delay(10);
 TEMP =sensors.getTempCByIndex(0);
}
void PH() {
  for (int i = 0; i < 10; i++)
  {
    buf[i] = analogRead(SensorPin);
    delay(10);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++)
  avgValue += buf[i];
  phValue = (float)avgValue * 5.0 / 1024 / 6;
  phValue = Caliberation * phValue;
  
}
