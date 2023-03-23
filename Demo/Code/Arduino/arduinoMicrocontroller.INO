#include "Firebase_Arduino_WiFiNINA.h"
#include <utility/wifi_drv.h>
#include <dht.h>

#define DATABASE_URL "homegrower-database-default-rtdb.europe-west1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "VQckmdJ3AdryXl4vlzcFQmqtGfY6Yukvow0jl8Se"
#define WIFI_SSID "eir69785572"
#define WIFI_PASSWORD "jXtk9ceP76"
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

  String path = "/test";
  String UID = "/000001";
  String jsonStr;  

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
  unsigned long long tempVal = DHT.temperature;
  unsigned long long humidVal = DHT.humidity; 
  unsigned long long moistureVal = moisture_percentage;

  if (Firebase.setInt(fbdo, path + "/sensorData/temperature", tempVal)) //support large number
  {
    Serial.println("ok");
    Serial.println("path: " + fbdo.dataPath());
    Serial.println("type: " + fbdo.dataType());
    Serial.print("value: ");
    if (fbdo.dataType() == "int")
      Serial.println(fbdo.intData());
    if (fbdo.dataType() == "int64")
      Serial.println(fbdo.int64Data());
    if (fbdo.dataType() == "uint64")
      Serial.println(fbdo.uint64Data());
    else if (fbdo.dataType() == "double")
      Serial.println(fbdo.doubleData());
    else if (fbdo.dataType() == "float")
      Serial.println(fbdo.floatData());
    else if (fbdo.dataType() == "boolean")
      Serial.println(fbdo.boolData() == 1 ? "true" : "false");
    else if (fbdo.dataType() == "string")
      Serial.println(fbdo.stringData());
    else if (fbdo.dataType() == "json")
      Serial.println(fbdo.jsonData());
    else if (fbdo.dataType() == "array")
      Serial.println(fbdo.arrayData());
  }
  else
  {
    Serial.println("error, " + fbdo.errorReason());
  }

  Serial.println();

  if (Firebase.setInt(fbdo, path + "/sensorData/humidity", humidVal)) //support large number
  {
    Serial.println("ok");
    Serial.println("path: " + fbdo.dataPath());
    Serial.println("type: " + fbdo.dataType());
    Serial.print("value: ");
    if (fbdo.dataType() == "int")
      Serial.println(fbdo.intData());
    if (fbdo.dataType() == "int64")
      Serial.println(fbdo.int64Data());
    if (fbdo.dataType() == "uint64")
      Serial.println(fbdo.uint64Data());
    else if (fbdo.dataType() == "double")
      Serial.println(fbdo.doubleData());
    else if (fbdo.dataType() == "float")
      Serial.println(fbdo.floatData());
    else if (fbdo.dataType() == "boolean")
      Serial.println(fbdo.boolData() == 1 ? "true" : "false");
    else if (fbdo.dataType() == "string")
      Serial.println(fbdo.stringData());
    else if (fbdo.dataType() == "json")
      Serial.println(fbdo.jsonData());
    else if (fbdo.dataType() == "array")
      Serial.println(fbdo.arrayData());
  }
  else
  {
    Serial.println("error, " + fbdo.errorReason());
  } 

  Serial.println();

  if (Firebase.setInt(fbdo, path + "/sensorData/moisture", moistureVal)) //support large number
  {
    Serial.println("ok");
    Serial.println("path: " + fbdo.dataPath());
    Serial.println("type: " + fbdo.dataType());
    Serial.print("value: ");
    if (fbdo.dataType() == "int")
      Serial.println(fbdo.intData());
    if (fbdo.dataType() == "int64")
      Serial.println(fbdo.int64Data());
    if (fbdo.dataType() == "uint64")
      Serial.println(fbdo.uint64Data());
    else if (fbdo.dataType() == "double")
      Serial.println(fbdo.doubleData());
    else if (fbdo.dataType() == "float")
      Serial.println(fbdo.floatData());
    else if (fbdo.dataType() == "boolean")
      Serial.println(fbdo.boolData() == 1 ? "true" : "false");
    else if (fbdo.dataType() == "string")
      Serial.println(fbdo.stringData());
    else if (fbdo.dataType() == "json")
      Serial.println(fbdo.jsonData());
    else if (fbdo.dataType() == "array")
      Serial.println(fbdo.arrayData());
  }
  else
  {
    Serial.println("error, " + fbdo.errorReason());
  }  

  Serial.println();
  
  delay(10000);//Wait 5 seconds before accessing sensor again.
}
