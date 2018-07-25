//SUIT 3

/*
   ARDUINO Variable         JSON Name           SIZE
   lcdMessage1                lcd1               20
   lcdMessage2                lcd2               20
   colorCode                   cc               int
   tickle                      tk             boolean
   suit                       suit              int
   lis1.x, lis2.y, lis2.z    x1,y1,z1           int
   lis2.x, lis2.y, lis1.z    x2,y2,z2           int
*/

//////////////////////////////////////////////////////////////////////
/////////////////DEFINITIONS//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//You will need to change the name of your suit per the defines below:

//This device publishes on a unique sub-channel, suit5, on the channel dmsSuits
#define pub_topic "dmsuits/"  //change suit number to 'suit' your device
#define this_suit 3

//This device is listening to all sub-channels of the dmsSuites channel
//'#' is wildcard for all sub-channels below the main channel
#define sub_topic "dmsuits/suit3/#"

//This information is needed for adressing our mqtt server at mediatedspaces.net
#define mqtt_server           "mediatedspaces.net"
#define mqtt_user             "" //put user name here for mqtt server
#define mqtt_password         "" //put password here Hotel Clara, at Stickgatan 17
#define unique_client_passwd  "dmsuit3" //whatever you wish to use, just make sure no one else is using this name

//These definitions set information related to the FastLED library
#define NUM_LEDS 4
#define BRIGHTNESS 32
#define LED_TYPE NEOPIXEL
#define COLOR_ORDER GRB
#define dataPin 15

#define lcdR 0
#define lcdG 2
#define lcdB 13

#define hall 14
#define vibPin 12

//These definitions hold key variables for connecting to the UB wifi network
#define wifi_ssid "UB_Connect"
#define wifi_password ""
//
//#define wifi_ssid "aquinnmartinproduction"
//#define wifi_password "18fa6d2d75"


///////////////////////////////////////////////////////////////////////
/////////////////LIBRARY INCLUDES//////////////////////////////////////
///////////////////////////////////////////////////////////////////////

//i2c and Accelerometer
#include <Wire.h> //for i2c communication
#include <Adafruit_LIS3DH.h> //for chatting with the accelerometers
#include <Adafruit_Sensor.h> //utility library for adafruit sensor boards

//esp8266 (Arduino) connectivity to network and the MQTT server
#include <ESP8266WiFi.h> //for Wifi connectivity between esp8266 and the network
#include <PubSubClient.h> //for MQTT communication with mqtt server

//Utility for parsing JSON strings
#include <ArduinoJson.h> //for parsing/reading incoming JSON messages

//neoPixel management and display
#include <FastLED.h> //for the neoPixels

//Commuication to LCD via i2c backpack
#include <LiquidTWI.h> //for the LCD


///////////////////////////////////////////////////////////////////////
/////////////////CONSTRUCTORS/INSTANTIATION////////////////////////////
///////////////////////////////////////////////////////////////////////

//Declare the lcd object directing the i2c backpack and addres the unit
//at i2c bus address 0x25. ther are 8 possibl addressed 0-7 which correspond
//to addressed 0x20 - Ox27,
LiquidTWI lcd(5); //buss address 0x25 on teh hardware pins CLK 5, SDA 4

//Declare the objects to direct activity of the 2 LIS3DH accelerometers, called lis1 and lis2, respectively
Adafruit_LIS3DH lis1 = Adafruit_LIS3DH();  //We don't declare CLK and SDA pins, using the hardware pins,
Adafruit_LIS3DH lis2 = Adafruit_LIS3DH();  //CLK 5, SDA 4

//WiFi
WiFiClient espClient; //create a WiFi client to connect to the WiFi
PubSubClient client(espClient); //create an MQTT client to speak over the WiFi client


///////////////////////////////////////////////////////////////////////
/////////////////VARIABLES/////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

//Messaging
char message[300]; //this holds the content of our outgoing messages to the webocket server
char incomingMessage[300]; //this holds the content of messages coming in from the MQTT server (from twitter)

//neoPixels
CRGB leds[NUM_LEDS]; //this is an array to hold our 4 LEDs

//LCD

const char* lcdMessage1; //to hold first line of lcd message, to be printed to the display
const char* lcdMessage2; //to hold second line of lcd message, to be printed to the display


//TIMERS

unsigned long currentMillis;
unsigned long accelTimer = 0;
int accelInterval = 10;

unsigned long vibTimer = 0;
int vibInterval = 500;
int vibrateCount = 5;
boolean vibState = false;

boolean debug = true;
boolean debugAccel = false;

//GLOBAL SUIT VARIABLES
int colorCode = 0; //this holds our colorCode, default BLACK (99)
boolean button = 0;

#define numReadings 2

int x1reads[numReadings];      // the readings from the analog input
int totX1 = 0;                  // the running total
int avgX1 = 0;                // the average

int y1reads[numReadings];      // the readings from the analog input
int totY1 = 0;                  // the running total
int avgY1 = 0;                // the average

int z1reads[numReadings];      // the readings from the analog input
int totZ1 = 0;                  // the running total
int avgZ1 = 0;                // the average


int x2reads[numReadings];      // the readings from the analog input
int totX2 = 0;                  // the running total
int avgX2 = 0;                // the average

int y2reads[numReadings];      // the readings from the analog input
int totY2 = 0;                  // the running total
int avgY2 = 0;                // the average

int z2reads[numReadings];      // the readings from the analog input
int totZ2 = 0;                  // the running total
int avgZ2 = 0;                // the average

int readIndex = 0;              // the index of the current reading

int hallState;


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
  //    x2reads[thisReading] = 0;
  //  }
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(unique_client_passwd, mqtt_user, mqtt_password)) { //you need a unique name (universal) for the first arguement
      Serial.println("connected");
      client.subscribe(sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup() {
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif

  if (debug) {
    Serial.begin(9600); //We will use the Serial port to debug/read our sensor output and the like
  }

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  if (! lis1.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start/find accelerometer1 . . .");
    while (1);
  }

  if (! lis2.begin(0x19)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Could not start/find accelerometer2 . . .");
    while (1);
  }

  //Accelerometer constructor: make me an accelrometer and set its 'sensitivity'
  lis1.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G! here we chose 2, low sensitivity (?)
  lis2.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G! here we chose 2, low sensitivity (?)


  FastLED.addLeds<LED_TYPE, dataPin>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  lcd.begin(16, 2); //start up the LCD

  pinMode(vibPin, OUTPUT);
  digitalWrite(vibPin, LOW);

  pinMode(lcdR, OUTPUT);
  pinMode(lcdG, OUTPUT);
  pinMode(lcdB, OUTPUT);

  pinMode(hall, INPUT_PULLUP);

  analogWrite(lcdR, 1023);
  analogWrite(lcdR, 1023);
  analogWrite(lcdR, 1023);
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////MAIN LOOP///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  currentMillis = millis(); //get current time, in past millisecond, since loop started

  if (!client.connected()) {
    reconnect();
  }
  client.loop(); //this keeps the client alive (MQTT)

  readAccs();
  vibrate();
  readHall();



  ///////////////////////////////////////////////////
  //Accelerometer code, done once per accelInterval
  if (currentMillis - accelTimer > accelInterval) {
    client.publish("dmsuits/", message);
    accelTimer = currentMillis;
  }

  pixelUpdate();
  //  Serial.println(vibrateCount);

  //  //Message is sent at the fastest sampling rate, the acceleromters
  //  sprintf(message, "{\"suit\": %d, \"x1\": %d, \"y1\": %d, \"z1\": %d, \"x2\": %d, \"y2\": %d, \"z2\": %d, \"cc\": %d, \"lcd1\": \"%s\", \"lcd2\": \"%s\"}", this_suit, lis1.x, lis1.y, lis1.z, avgX2, avgY2, lis2.z, colorCode, "test", "message");
  //  sprintf(message, "{\"suit\": %d, \"x1\": %d, \"y1\": %d, \"z1\": %d, \"x2\": %d, \"y2\": %d, \"z2\": %d, \"cc\": %d, \"vb\": %d}", this_suit, avgX1, avgY1, avgZ1, avgX2, avgY2, avgZ2, colorCode, vibrateCount);
  sprintf(message, "{\"suit\": %d,\"button\": %d, \"x1\": %d, \"y1\": %d, \"z1\": %d, \"x2\": %d, \"y2\": %d, \"z2\": %d, \"cc\": %d, \"vb\": %d, \"lcd1\":\"%s\", \"lcd2\":\"%s\"}", this_suit, hallState, avgX1, avgY1, avgZ1, avgX2, avgY2, avgZ2, colorCode, vibrateCount, lcdMessage1, lcdMessage2);

  //  Serial.println(message);
}


