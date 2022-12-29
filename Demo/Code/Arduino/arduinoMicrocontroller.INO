#include <dht.h>

// CODE FOR DHT11 LINK: https://create.arduino.cc/projecthub/techno_z/dht11-temperature-humidity-sensor-98b03b
//CODE FOR MOISTURE SENSOR LINK: https://www.instructables.com/Soil-Moisture-Sensor-With-Arduino/

#define dht_apin A0 // Analog Pin sensor is connected to
 
//library for DHT11 sensor 
dht DHT;
const int sensor_pin = A1; /* Soil moisture sensor O/P pin */
//variables for soil mositur


void setup(){
 
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  //Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor
 
}//end "setup()"
 
void loop(){
  //Start of Program 
 
    //code for DHT sensor
    DHT.read11(dht_apin);
    
    //Serial.print("Current humidity = ");
    
    //Serial.print("temperature = ");
    

    //code for Mositure Sensor
    float moisture_percentage;

    int sensor_analog;

    sensor_analog = analogRead(sensor_pin);

    moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );

    //Serial.print("Moisture Percentage = ");
    Serial.print(DHT.temperature); 
    Serial.print(",");
    Serial.print(DHT.humidity);
    Serial.print(",");
    Serial.print(moisture_percentage);

    Serial.print("\n");
    
    delay(5000);//Wait 5 seconds before accessing sensor again.
 
  //Fastest should be once every two seconds.
 
}
