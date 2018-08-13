/*
  Nextion_HMI_Test using NeoNextion library 
  (c) Claus Kuehnel 2018-04-16 info@ckuehnel.ch

  Arduino reads DHT11 data and creates messages for displaying these data on Nextion HMI.
  The corresponding firmware on Nextion HMI is commands.tft.
  See for repository at Github https://github.com/ckuehnel/NextionHMI

  Tested w/ Olimexino-328 as Duemilanove-328
  Should run unchanged on Arduino Uno.
*/
#include "DHT.h"
#include <Nextion.h>
#include <NextionDualStateButton.h>
#include <NextionPage.h>
#include <NextionSlidingText.h>
#include <NextionText.h>
#include <SoftwareSerial.h>

// Nextion TX to pin 10 and RX to pin 11 of Arduino
SoftwareSerial nextionSerial(10, 11); // RX, TX

#include <avr/io.h>
// ISR interrupt service routine
#include <avr/interrupt.h>

Nextion nex(nextionSerial);
NextionPage p0(nex, 0, 0, "page0");
NextionText t0(nex, 0, 2, "t0");
NextionText t1(nex, 0, 4, "t1");
NextionSlidingText lauf(nex, 0, 6, "lauf");
NextionDualStateButton button(nex, 0, 5, "button");

//#define DEBUG
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define DHTPIN 7

String message;
const int pRelais = 2;
const int pLED = LED_BUILTIN;
boolean Interrupt = false;

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

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
    t0.setText(msg);
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
    t1.setText(msg);
  }
  Serial.println(button.isActive());
}

void setup(void)
{    
  Serial.begin(115200);

  dht.begin(); 
  initTimer1();

  nextionSerial.begin(9600);
  nex.init();

  pinMode(pRelais, OUTPUT);          // set pin to output
  digitalWrite(pRelais, HIGH);       // turn off relais

  pinMode(pLED, OUTPUT);          // set pin to output
  digitalWrite(pLED, LOW);       // turn off LED
  
  while(!Serial);
  Serial.println("Nextion HMI Test");
  Serial.println("Setup finished.");
  Serial.println("Start Nextion HMI");

  p0.show();
  button.attachCallback(&callback);
  t0.setText("---");
  t1.setText("--"); 
  lauf.setText("Fan stopped.");
  Serial.println(button.isActive());
}

void loop(void)
{  
  if (Interrupt==true)
  {
    Interrupt = false;
    queryDHT11();
  }
  nex.poll(); 
}

void callback(NextionEventType type, INextionTouchable *widget) // angepasstes tft mit ID laden
{
  if (type == NEX_EVENT_PUSH)
  {
    if (button.isActive())
    {
      digitalWrite(pRelais, LOW);
      digitalWrite(pLED, HIGH);      // turn on LED
    
      Serial.println("Fan on");
      #ifdef DEBUG 
        Serial.println("lauf.txt=\"Fan running...\"");
      #endif
      lauf.setText("Fan running...");
    }
    else
    {
      digitalWrite(pRelais, HIGH);
      digitalWrite(pLED, LOW);       // turn off LED
    
      Serial.println("Fan off");
      #ifdef DEBUG 
        Serial.println("lauf.txt=\"Fan stopped.\"");
      #endif
      lauf.setText("Fan stopped.");
    }
  }
}










