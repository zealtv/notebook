#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <Adafruit_NeoPixel.h>
#include "SH1106.h"

//OSC
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

//BUTTONS
#include <AceButton.h>
using namespace ace_button;

#define WHEEL_PRESS   18
#define WHEEL_LEFT    5
#define WHEEL_RIGHT   19
#define BUZZER 32
#define BUTTON_TOP 22
#define BUTTON_BOTTOM 23
#define FLASHLIGHT 27
#define PIXEL_PIN    33    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 1

AceButton wheelPress(WHEEL_PRESS);
AceButton wheelLeft(WHEEL_LEFT);
AceButton wheelRight(WHEEL_RIGHT);
AceButton buttonTop(BUTTON_TOP);
AceButton buttonBottom(BUTTON_BOTTOM);

// Forward reference to prevent Arduino compiler becoming confused.
void handleEvent(AceButton*, uint8_t, uint8_t);

//Set up Neopixel and OLED display
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
SH1106  display(0x3c, 17, 16);

WebServer Server;
AutoConnect Portal(Server);
AutoConnectConfig Config;

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(255, 255, 255, 255);  // remote IP of your computer
const unsigned int outPort = 8000;          // remote port to receive OSC
const unsigned int localPort = 9000;        // local port to listen for OSC packets
String macString = "";
String oscAddress;
OSCErrorCode error;

//TODO can prob get rid of this...
int showType = 0;

long startTime = 0;

void rootPage() {
  char content[] = "Hello, world";
  Server.send(200, "text/plain", content);
}

void setup() {
  Serial.begin(115200);

  pinMode(WHEEL_PRESS, INPUT_PULLUP);
  pinMode(WHEEL_LEFT, INPUT_PULLUP);
  pinMode(WHEEL_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_TOP, INPUT_PULLUP);
  pinMode(BUTTON_BOTTOM, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(FLASHLIGHT, OUTPUT);
  
  digitalWrite(FLASHLIGHT, HIGH);

  // Configure the ButtonConfig with the event handler, and enable all higher
  // level events.
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);
  
  display.init();
  display.flipScreenVertically();
  display.clear();

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //config autoconnect.  see here: https://hieromon.github.io/AutoConnect/adconnection.html
  Config.beginTimeout = 15000; // Timeout sets to 15[s]
  Config.immediateStart = false;

  Config.apid = "packetPunk";
  Config.psk  = "packetPunk";
  Portal.config(Config);
  initConnection();
}


// initiate connection
void initConnection() {
  Server.on("/", rootPage);
  strip.setPixelColor(0, 70, 50, 0); // Yellow indicates not yet connected
  strip.show();

  // HOLD BOTTOM LEFT BUTTON ON START TO REINIT WIFI CONNECTION
  //TODO change this to buttonBottom.isPressedRaw()
//  if (digitalRead(BUTTON_BOTTOM) == LOW) {   
//    Config.immediateStart = true;
//    Portal.config(Config);
//    initConnection();
//  }

  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString( 0, 0, "Ready to connect");
  display.drawString( 0, 26, "Join packetPunk");
  display.drawString( 0, 36, "goto 172.217.28.1");
  display.drawString( 0, 46, "to configure");
  display.display();

  //this will hang until true:
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
    Udp.begin(localPort);
    //set led to green
    strip.setPixelColor(0, 0, 70, 0);  // Green
    strip.show();

    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString( 0, 0, "CONNECTED");
    display.drawString( 0, 26, "ready...");
    display.display();
  }
}


void loop() {
  getOSC();
  Portal.handleClient();
  checkButtons();

//  //wheel up
//  if (digitalRead(WHEEL_LEFT) == LOW){}
//
//  //wheel down
//  if (digitalRead(WHEEL_RIGHT) == LOW){}
//   
//  //wheel press
//  if(digitalRead(WHEEL_PRESS) == LOW){}
//
//  //top left button
//  if (digitalRead(BUTTON_TOP) == LOW){}
//
//  //bottom left button
//  if (digitalRead(BUTTON_BOTTOM) == LOW){}
}

void checkButtons(){
  wheelLeft.check();
  wheelRight.check();
  wheelPress.check();
  buttonTop.check();
  buttonBottom.check();
}

void getOSC() {
  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    strip.setPixelColor(0, 0, 0, 0);  // Black
    strip.show();
    while (size--) {
      msg.fill(Udp.read());
    }
    
    if (!msg.hasError()) {
      //send all OSC messages
      receiveOSC(msg);

    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}

void receiveOSC(OSCMessage &msg) {
  int msgSize = msg.size();
  String oscAddress;
  char addressBuffer[99];
  msg.getAddress(addressBuffer);
  
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString( 0, 0, "[R]");

  //draw addres to screen
  display.drawString( 26, 0, addressBuffer);

  //Create string from incoming message
  String msgString = makeOSCString(msg);
//  Serial.println(String(addressBuffer) + " " +  msgString);
  
  display.drawStringMaxWidth( 0, 13, 128, msgString );

  display.display();
  strip.setPixelColor(0, 0, 0, 10);  // Black
  strip.show();
}


String makeOSCString(OSCMessage &msg){
  int msgSize = msg.size();
  String OSCString = "";
  
  //Build a string of the OSC arguments, casting each type as a string
  for(int i = 0; i < msgSize; i++){
    float thisVal;

    if(msg.isInt(i)){
      OSCString = OSCString + String(msg.getInt(i));
    }
    else if(msg.isFloat(i)){
      OSCString = OSCString + String(msg.getFloat(i));
    }
    else if(msg.isDouble(i)){
      OSCString = OSCString + String(msg.getDouble(i));
    }
    //TODO Bools not working?
    else if(msg.isBoolean(i)){
      OSCString = OSCString + String(msg.getBoolean(i));
    }
    //TODO Strings

    //Add a space if not the last element
    if(i + 1 < msgSize){
      OSCString = OSCString + " ";
    }
    
    
  }

  return OSCString;
}


// The event handler for both buttons.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {

//  Print out a message for all events, for both buttons.
//  Serial.print(F("handleEvent(): pin: "));
//  Serial.print(button->getPin());
//  Serial.print(F("; eventType: "));
//  Serial.print(eventType);
//  Serial.print(F("; buttonState: "));
//  Serial.println(buttonState);

//Button States:
//    kEventPressed
//    kEventReleased
//    kEventClicked
//    kEventDoubleClicked
//    kEventLongPressed
//    kEventRepeatPressed
//    kEventLongReleased
//    kButtonStateUnknown

  switch (eventType) {

    //LONG PRESSES
    case AceButton::kEventLongPressed:
      
      //BOTTOM BUTTON
      if (button->getPin() == BUTTON_BOTTOM)
        reinitConnection();
      break;
    
//    case AceButton::kEventReleased:
//      if (button->getPin() == BUTTON1_PIN) {
//        digitalWrite(LED_PIN, LED_OFF);
//      }
//      break;
//    
//    case AceButton::kEventClicked:
//      if (button->getPin() == BUTTON2_PIN) {
//        Serial.println(F("Button 2 clicked!"));
//      } else if (button->getPin() == BUTTON3_PIN) {
//        Serial.println(F("Button 3 clicked!"));
//      }
//      break;
  }
}

//Drop off current network and start up adhoc network for reconfiguration
void reinitConnection(){
    Config.immediateStart = true;
    Portal.config(Config);
    initConnection();
}

//-----------------------------------------------------
void soundone() {
  unsigned char i, j;
  for (i = 0; i < 5; i++) {
    for (i = 0; i < 200; i++) {
      digitalWrite(BUZZER, HIGH);
      delay(1);//Change this could adjust voice
      digitalWrite(BUZZER, LOW);
      delay(1);
    }

    for (i = 0; i < 100; i++) {
      digitalWrite(BUZZER, HIGH);
      delay(2);
      digitalWrite(BUZZER, LOW);
      delay(2);
    }
  }
}

void soundtwo() {
  unsigned char i, j;

  for (i = 0; i < 5; i++) {
    for (i = 0; i < 100; i++) {
      digitalWrite(BUZZER, HIGH);
      delay(2);//Change this could adjust voice
      digitalWrite(BUZZER, LOW);
      delay(2);
    }

    for (i = 0; i < 200; i++) {
      digitalWrite(BUZZER, HIGH);
      delay(1);
      digitalWrite(BUZZER, LOW);
      delay(1);
    }
  }
}
