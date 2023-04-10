#include "Firebase_Arduino_WiFiNINA.h"
#include <utility/wifi_drv.h>
#include <dht.h>

#define DATABASE_URL "YOUR_DATABASE_URL" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "YOUR_SECRET"
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define dht_apin A0
#define sensor_pin A1

dht DHT;
FirebaseData fbdo;

void setup() {

  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(100);

  //RGB Definition  
  WiFiDrv::pinMode(25, OUTPUT); //G
  WiFiDrv::pinMode(26, OUTPUT); //R
  WiFiDrv::pinMode(27, OUTPUT); //B

  WiFiDrv::analogWrite(25, 255);
  WiFiDrv::analogWrite(26, 0);
  WiFiDrv::analogWrite(27, 0);
  
  Serial.println();

  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  WiFiDrv::analogWrite(25, 0);
  WiFiDrv::analogWrite(26, 255);
  WiFiDrv::analogWrite(27, 0);

  Firebase.begin(DATABASE_URL, DATABASE_SECRET, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);
}

void loop() {
    
  int sensorValue = analogRead(sensor_pin);  // Read the analog value from sensor
  int moisture_percentage = map(sensorValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data

  //code for DHT sensor
  DHT.read11(dht_apin);
  
  Serial.print(DHT.temperature); 
  Serial.print(",");
  Serial.print(DHT.humidity);
  Serial.print(",");
  Serial.println(moisture_percentage);

  Serial.print("\n");

  delay(5000);
  Serial.print("sending data to firebase");
  
  firebasePush(moisture_percentage);

  Serial.println();
  
  delay(10000);//Wait 5 seconds before accessing sensor again.
}






void firebasePush(int moisture_percentage){

  //String path = "/test";
  String UID = "/000001";
  String jsonStr;

  unsigned long long tempVal = DHT.temperature;
  unsigned long long humidVal = DHT.humidity; 
  unsigned long long moistureVal = moisture_percentage;

  char buffer[200];

  sprintf(buffer, "{\"moisture\":%d,\"temperature\":%d,\"humidity\":%d}", moistureVal, tempVal, humidVal);
  Serial.println(buffer);

  if (Firebase.pushJSON(fbdo, UID, buffer))
  {
    Serial.println("ok");
    Serial.println("path: " + fbdo.dataPath());
    Serial.print("push name: ");
    Serial.println(fbdo.pushName());
  }
  else
  {
    Serial.println("error, " + fbdo.errorReason());
  }
}
