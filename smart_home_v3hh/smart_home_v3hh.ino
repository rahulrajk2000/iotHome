#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "A4988.h"

//wifico
#include <ESP8266WebServer.h>
#include <EEPROM.h>
int i = 0;
int statusCode;
const char *ssid = "text";
const char *passphrase = "text";
String st;
String content;
//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);
ESP8266WebServer server(80);


#define Relay0 LED_BUILTIN



#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883                         // use 8883 for SSL
#define AIO_USERNAME "rahulrajk2000"                // Replace it with your username
#define AIO_KEY "aio_RzkM756T9P269XBnvv5yeA3IL3a7"  // Replace with your Project Auth Key



WiFiClient client;


Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/




Adafruit_MQTT_Subscribe Light0 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/curtain");
Adafruit_MQTT_Subscribe adafruitA = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/curtainA");


A4988 curtain(280, D3, D0);
A4988 curtainA(280, D6,D5);




void MQTT_connect();

void setup() {
  Serial.begin(115200);
  //wifico
  Serial.println();
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
  EEPROM.begin(512);  //Initialasing EEPROM
  delay(10);
  //---------------------------------------- Read eeprom for ssid and pass
  Serial.println("maked by yatin");
  Serial.println("Reading EEPROM ssid");
  String esid;
  for (int i = 0; i < 32; ++i) {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");

  String epass = "";
  for (int i = 32; i < 96; ++i) {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);


  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(esid);

  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi()) {
    Serial.println("Succesfully Connected!!!");

  } else {
    Serial.println("Turning the HotSpot On");
    launchWeb();
    setupAP();  // Setup HotSpot
  }
  while ((WiFi.status() != WL_CONNECTED)) {
    Serial.print(".");
    delay(100);
    server.handleClient();
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  pinMode(Relay0, OUTPUT);

  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());



  mqtt.subscribe(&Light0);
  mqtt.subscribe(&adafruitA);

  
  MQTT_connect();
  curtain.setRPM(300);
  curtain.setSpeedProfile(BasicStepperDriver::LINEAR_SPEED,500,1000);

  curtainA.setRPM(300);
   curtainA.setSpeedProfile(BasicStepperDriver::LINEAR_SPEED,500,1000);


}

void loop() {

  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {

    if (subscription == &Light0) {
      Serial.print(F("Relay0: "));
      Serial.println((char *)Light0.lastread);
      int Light0_State = atoi((char *)Light0.lastread);
      digitalWrite(Relay0, not Light0_State);
      if (Light0_State == 0) {
      curtain.move  (10000);
      } else {
        curtain.move(-10000);
      }
    }

    if (subscription == &adafruitA) {
      Serial.print(F("Relay0: "));
      Serial.println((char *)adafruitA.lastread);
      int Light0_State = atoi((char *)adafruitA.lastread);
      digitalWrite(Relay0, not Light0_State);
      if (Light0_State == 0) {
      curtainA.move  (10000);
      } else {
        curtainA.move(-10000);
      }
    }

    
  }
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");


  uint8_t retries = 10000;

  while ((ret = mqtt.connect()) != 0) {  // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }
  Serial.println("MQTT Connected!");
}
bool testWifi(void) {
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while (c < 25) {
    if (WiFi.status() == WL_CONNECTED) {
      return true;
    }
    delay(1000);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}