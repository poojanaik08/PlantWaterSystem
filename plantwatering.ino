#define BLYNK_PRINT Serial
#include <OneWire.h>
#include <SPI.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

char auth[] = "CxY6hfhjH0vozkUXn0ViifjCMeeekBap";
char ssid[] = "Terrace home";
char pass[] = "manorama21";

int LED = D3;

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V5, h); //V5 is for Humidity
  Blynk.virtualWrite(V6, t); //V6 is for Temperature
}
void setup()
{
  Serial.begin(9600);
  dht.begin();
  
  timer.setInterval(1000L, sendSensor);
  pinMode(LED, OUTPUT);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  sensors.begin();  
}

int sensor=0;
int output=0;
void sendTemps()
{
  sensor=analogRead(A0);
  output=(145-map(sensor,0,1023,0,100));
  delay(1000);
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  Serial.println(temp);
  Serial.print("moisture = ");
  Serial.print(output);
  Serial.println("%");
  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2,output);
  delay(1000);
}
void loop()
{
  Blynk.run();
  timer.run();
  sendTemps();
}
BLYNK_WRITE(V7) {
  int pinValue = param.asInt(); // Assigning incoming value from pin V7 to a variable
  Serial.print("Pin number: ");
  Serial.println(LED);
  Serial.println(pinValue);
  if (pinValue == 1) {
    digitalWrite(LED, HIGH); // Turn LED on.
  }
  else {
    digitalWrite(LED, LOW); // Turn LED off.
 }
}
