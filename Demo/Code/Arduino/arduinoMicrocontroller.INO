#include "Firebase_Arduino_WiFiNINA.h"
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>
#include <dht.h>
#define DATABASE_URL "homegrower-database-default-rtdb.europe-west1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "VQckmdJ3AdryXl4vlzcFQmqtGfY6Yukvow0jl8Se"
//#define WIFI_SSID "IoT2"
//#define WIFI_PASSWORD "ilikecake"
#define dht_apin A0
#define sensor_pin A1

dht DHT;
FirebaseData fbdo;
boolean noWifiCredentials = true;
boolean connectWifi = false;
boolean connected = false;

char ssid[] = "GROW_NODE";
char pass[] = "GROW_NODE";
int status = WL_IDLE_STATUS;
WiFiServer server(80);

String network;
String password;

void setup() {

  Serial.begin(9600);
  delay(100);
  
  Serial.println();


  Serial.println("Access Point Web Server");

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  printStatus();


}

void loop() {
  
  int sensorValue = analogRead(sensor_pin);  // Read the analog value from sensor
  int moisture_percentage = map(sensorValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data

  if (noWifiCredentials) {
    getWiFiCredentials();
  }
  if (connectWifi) {
    connectToWifi();
  }

  if (connected){
    Firebase.begin(DATABASE_URL, DATABASE_SECRET, network, password);
    Firebase.reconnectWiFi(true);   
     
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

}



void getWiFiCredentials() {
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<html>");
            client.println("<style>");
            client.println("</style>");
            client.println("<body>");
            client.println("<form method=get>");
            client.print("WiFi Name: ");
            client.print("<input type=\"text\" name=\"network\"/><br>");
            client.print("PASSWORD: ");
            client.print("<input type = \"text\" name=\"password\"/><br>");
            client.print("<input type=submit value=Submit></form>");
            client.println("</body>");
            client.println("</html>");
            client.println();
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        int netIndex = -1;
        int passIndex = -1;
        int httpIndex = -1;

        if (currentLine.indexOf(" HTTP/1.1") != -1 && currentLine.indexOf("&password=") != -1 && currentLine.indexOf("&password=")) {
          netIndex = currentLine.indexOf("?network=");
          passIndex = currentLine.indexOf("&password=");
          httpIndex = currentLine.indexOf("&password=");
          network = currentLine.substring(netIndex + 9, passIndex);
          password = currentLine.substring(passIndex + 10, currentLine.indexOf(" HTTP/1.1"));
          network.trim();
          password.trim();
          WiFi.end();
          noWifiCredentials = false;
          connectWifi = true;
          
        }
      }
    }
    client.stop();
  }
}




void connectToWifi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(network);
    Serial.println(password);
    WiFi.begin(network.c_str(), password.c_str());
    delay(5000);
  }
  Serial.println("WiFi connection successful");
  printStatus();
  connectWifi = false;
  connected = true;
}  //Establish a WiFi connection



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


void printStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  Serial.print("To connect, open a browser to http://");
  Serial.println(ip);
}
