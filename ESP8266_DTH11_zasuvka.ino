/**************************************************************
   Blynk is a platform with iOS and Android apps to control
   Arduino, Raspberry Pi and the likes over the Internet.
   You can easily build graphic interfaces for all your
   projects by simply dragging and dropping widgets.

     Downloads, docs, tutorials: http://www.blynk.cc
     Blynk community:            http://community.blynk.cc
     Social networks:            http://www.fb.com/blynkapp
                                 http://twitter.com/blynk_app

   Blynk library is licensed under MIT license
   This example code is in public domain.

 **************************************************************
   This example runs directly on ESP8266 chip.

   You need to install this for ESP8266 development:
     https://github.com/esp8266/Arduino

   Please be sure to select the right ESP8266 module
   in the Tools -> Board menu!

   Change WiFi ssid, pass, and Blynk auth token to run :)

 **************************************************************/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

#define DHTPIN 2          // What digital pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

#define RELEPIN 3

/******Blobal Variables**************/
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

int iSlider = 0;
float h = 0; //vlhkost
/***************************************/
//------Created classes-----------
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
WidgetLED led1(V2);
//--------------------------------
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}


void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(RELEPIN, OUTPUT);
  digitalWrite(RELEPIN, LOW);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);

}

//****casovac
BLYNK_WRITE(V0)
{
  // You'll get HIGH/1 at startTime and LOW/0 at stopTime.
  // this method will be triggered every day
  // until you remove widget or stop project or
  // clean stop/start fields of widget
  Serial.print("Stav casovace: ");
  Serial.println(param.asStr());

  if (param.asInt() == 1)
  {
    h = dht.readHumidity();
    if (!isnan(h)) // ...kdyz nenastala chyba cteni dat...
    {
      if ((int)h < iSlider)
      {
        digitalWrite(RELEPIN, HIGH);
      } else
      {
        digitalWrite(RELEPIN, LOW);
      }
    }
  }
}

//****slider
BLYNK_WRITE(V1)
{
  iSlider = param.asInt();
}

//*** tlacitko
BLYNK_WRITE(V3)
{
  if (param.asInt() == 1)
  {
    digitalWrite(RELEPIN, HIGH);
    led1.on();
  }
  else
  {
    digitalWrite(RELEPIN, LOW);
    led1.off();
  }
}

void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}

