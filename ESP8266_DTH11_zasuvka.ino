#include <SimpleTimer.h>


#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "8ecd574ffc1343ffb2a6c57a7279e3cf";

#define DHTPIN 2          // What digital pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

#define RELEPIN 5

/******Blobal Variables**************/
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Yak-Yak";
char pass[] = "beruskak";

int iSlider = 0;
float h = 0; //vlhkost
// Keep this flag not to re-sync on every reconnection
bool isFirstConnect = true;
bool isTimerOn = false;
bool isManualOn = false;
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

  //Serial.println(isTimerOn);



  if (((   (int)h < iSlider) & isTimerOn)     |    isManualOn )
  {
    digitalWrite(RELEPIN, HIGH);
    led1.on();
//    isManualOn = true;

  Blynk.virtualWrite(V3, 1);

  } else
  {
    digitalWrite(RELEPIN, LOW);
    led1.off();
  //  isManualOn = false;
  
  Blynk.virtualWrite(V3, 0);
  }


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
    isTimerOn = true;
  }
  else
  {
    isTimerOn = false;
  }
  /* */

}

//****slider
BLYNK_WRITE(V1)
{
  iSlider = param.asInt();
  Serial.print("slider: ");
  Serial.println(iSlider);
}

//*** tlacitko
BLYNK_WRITE(V3)
{
  if (param.asInt() == 1)
  {
    isManualOn = true;
  }
  else
  {
    isManualOn = false;
  }
}


// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  if (isFirstConnect) {
    // Request Blynk server to re-send latest values for all pins
    Blynk.syncAll();

    // You can also update an individual Virtual pin like this:
    //Blynk.syncVirtual(V0);

    isFirstConnect = false;
  }
}

void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
