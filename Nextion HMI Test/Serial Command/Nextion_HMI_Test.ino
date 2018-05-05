/*
  Nextion_HMI_Test
  (c) Claus Kuehnel 2018-04-16 info@ckuehnel.ch

  Arduino reads DHT11 data and creates messages for displaying these data on Nextion HMI.
  The corresponding firmware on Nextion HMI is commands.tft.
  See for repository at Github https://github.com/ckuehnel/NextionHMI

  Tested w/ Olimexino-328 as Duemilanove-328
  Should run unchanged on Arduino Uno.
*/

#include "DHT.h"
#include <SoftwareSerial.h>

#include <avr/io.h>
// ISR interrupt service routine
#include <avr/interrupt.h>

#define DEBUG  // comment if no debugging

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define DHTPIN 7

String message;
const int pRelais = 2;
const int pLED = LED_BUILTIN;
boolean Interrupt = false;

// Nextion TX to pin 10 and RX to pin 11 of Arduino
SoftwareSerial HMISerial(10, 11);

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void HMIprint(String str)
{
  HMISerial.print(str);
  HMISerial.write(0xFF);       // Each Nextion HMI command is closed by three bytes 0xFF
  HMISerial.write(0xFF);
  HMISerial.write(0xFF);
}

// Install the interrupt routine for Timer1 CompareA
ISR(TIMER1_COMPA_vect)       
{
  Interrupt = true;
}

void queryDHT11() 
{
  char msg[20];
  
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  //float temperature=33.3;       // for test only
  Serial.print("Temperature: ");
  Serial.println(temperature,1);
  // Check if any reads failed and exit early (to try again).
  if (isnan(temperature)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else 
  {
    dtostrf(temperature, 3, 1, msg);  //3 is mininum width, 1 is precision
    String toHMI = "t0.txt=\""; 
    toHMI += msg; 
    toHMI += "\"";
    #ifdef DEBUG 
      Serial.println(toHMI);
    #endif
    HMIprint(toHMI);
  }

  float humidity = dht.readHumidity();
  //float humidity=66.6;          // for test only
  Serial.print("Humidity: ");
  Serial.println(humidity,0);
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else 
  {
    dtostrf(humidity, 2, 0, msg);  //2 is mininum width, 0 is precision
    String toHMI = "t1.txt=\""; 
    toHMI += msg; 
    toHMI += "\"";
    #ifdef DEBUG 
      Serial.println(toHMI); 
    #endif
    HMIprint(toHMI);
  }
}

void setup()
{
  Serial.begin(115200);
  dht.begin(); 
  initTimer1();
    
  // Set the baudrate which is for debug and communicate with Nextion HMI
  HMISerial.begin(9600);

  pinMode(pRelais, OUTPUT);       // set pin to output
  digitalWrite(pRelais, HIGH);    // turn off relais

  pinMode(pLED, OUTPUT);          // set pin to output
  digitalWrite(pLED, LOW);        // turn off LED
  
  while(!Serial);
  Serial.println("Nextion HMI Test");
  Serial.println("Setup finished.");
  Serial.println("Start Nextion HMI");
  
  HMIprint("page 0");
  HMIprint("t0.txt=\"---\"");
  HMIprint("t1.txt=\"--\"");
}

void loop() 
{
  if (Interrupt==true)
  {
    Interrupt = false;
    queryDHT11();
  }
  while (HMISerial.available()) //Is data coming through the serial from the Nextion?
  { 
    //Serial.print(HMISerial.read(),HEX);
    message.concat(HMISerial.read()); //If so, construct the message 
  }
  if (message == "1000")
  {
    digitalWrite(pRelais, LOW);
    digitalWrite(pLED, HIGH);      // turn on LED
    
    Serial.println("Fan on");
    #ifdef DEBUG 
      Serial.println("lauf.txt=\"Fan running...\"");
    #endif
    HMIprint("lauf.txt=\"Fan running...\"");
    message = "";
  }
  if (message == "0000")
  {
    digitalWrite(pRelais, HIGH);
    digitalWrite(pLED, LOW);       // turn off LED
    
    Serial.println("Fan off");
    #ifdef DEBUG 
      Serial.println("lauf.txt=\"Fan stopped.\"");
    #endif
    HMIprint("lauf.txt=\"Fan stopped.\"");
    message = "";
  }
}

