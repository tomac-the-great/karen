#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Define pins for each relay.
//#define P_INTAKE 14  // Intake Fan Pin
//#define P_HEAT 8  // Heater Relay Pin
//#define P_LIGHT 53 // Light Relay Pin
//#define P_CIRC 15 // Circulation Fan Relay

// Define pins for each relay (Test Arduino Uno)
#define P_INTAKE 6  // Intake Fan Pin
#define P_HEAT 11  // Heater Relay Pin
#define P_LIGHT 8 // Light Relay Pin
#define P_CIRC 7 // Circulation Fan Relay

// #define P_UNUSED3 3
// #define P_UNUSED4 4
// #define P_UNUSED5 5
// #define P_UNUSED6 6
// #define P_UNUSED7 7

// Define inside/outside temp/humidity pins
//#define P_TEMPIN 20
//#define P_TEMPOUT 5

// Define inside/outside temp/humidity pins (Test Arduino Uno)
#define P_TEMPIN 20
#define P_TEMPOUT 5

#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(P_TEMPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(P_TEMPOUT);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

// Variables

float hum;  // Stores humidity value
float temp; // Stores inside temperature value
float tempf; // Stores inside temperature value in Fahrenheit

void setup() {
	// Start up the DHT22 library
  sensors.begin();

	// Start serial connection.
	Serial.begin(9600);

	// Set mode of pins used to toggle relays.
	pinMode(P_INTAKE, OUTPUT);
	pinMode(P_HEAT, OUTPUT);
	pinMode(P_LIGHT, OUTPUT);
  pinMode(P_CIRC, OUTPUT);

	// Default state is all devices off.
	digitalWrite(P_INTAKE, HIGH);
	digitalWrite(P_HEAT,HIGH);
	digitalWrite(P_LIGHT, HIGH);
  digitalWrite(P_CIRC, LOW);

  // DHT
  dht.begin();
}

void loop() {
  //  int relay0 = digitalRead(P_INTAKE);
  
  // Send the command to get temperatures
  sensors.requestTemperatures(); 
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  tempf = ((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  
  //  Print sensor/relay data to serial
  Serial.print("{\"TempOut\":");
  Serial.print((sensors.getTempCByIndex(0) * 9.0) / 5.0 + 32.0);
  Serial.print(", \"TempIn\":");
  Serial.print((temp) * 9.0 / 5.0 + 32.0);
  Serial.print(", \"Humidity\":");
  Serial.print(hum);
  Serial.println("}");
  //  Serial.println(relay0);
    
  delay(500);
  
  // Get Serial Input
  while(Serial.available()) 
   {
     char state = Serial.read();
      if (state == 'A') {
        digitalWrite(P_INTAKE, HIGH);
      }
      if (state == 'a') {
        digitalWrite(P_INTAKE, LOW);
      }
      if (state == 'B') {
        digitalWrite(P_HEAT, HIGH);
      }
      if (state == 'b') {
        digitalWrite(P_HEAT, LOW);
      }
      if (state == 'C') {
        digitalWrite(P_LIGHT, HIGH);
      }
      if (state == 'c') {
        digitalWrite(P_LIGHT, LOW);
      }
     delay(50);
  }
  
  // Test Control based on set point
  if(tempf > 80.0) {
    digitalWrite(P_INTAKE,LOW);  //Turn on vent FAN
    }
    else {
    digitalWrite(P_INTAKE,HIGH);  //Turn off vent fan
    }

  if(tempf < 74.0) {
    digitalWrite(P_HEAT,LOW);    //Turn on heater
    delay(90000);
    }
    else {
    digitalWrite(P_HEAT,HIGH);    //Turn off heater
    }
  delay(1000); //Update at every 100mSeconds
}
