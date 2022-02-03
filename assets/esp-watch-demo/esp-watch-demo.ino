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

#define RECORDING     2
#define READY       1
#define DISCONNECTED  0
int state = DISCONNECTED;

unsigned long recordingStartTime = -1000;
unsigned long lastMessageTime = -1000;
String lastMessageAddress = "";
String lastMessageValues = "";
String networkAddress = "";
String fileName = "";

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

long startTime = -1000;

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
  buttonConfig->setLongPressDelay(2000);
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);
  
  display.init();
  display.flipScreenVertically();
  display.clear();

  strip.begin();
  updatePixel();

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
  updateDisplay();
  updatePixel();
  
  Server.on("/", rootPage);
  //this will hang until true:
  if (Portal.begin()) {
    state = READY;
    networkAddress = WiFi.localIP().toString();
    Udp.begin(localPort);
  }
}


//-----------------------------------------------------------------------
//----------MAIN LOOP----------------------------------
//-------------------------
void loop() {
  getOSC();
  Portal.handleClient();
  checkButtons();
  updatePixel();
  updateDisplay();
}


void checkButtons(){
  wheelLeft.check();
  wheelRight.check();
  wheelPress.check();
  buttonTop.check();
  buttonBottom.check();
}



//OSC----------------------------------------------
void getOSC() {
  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    
    while (size--)
      msg.fill(Udp.read());
      
    if (!msg.hasError())
      receiveOSC(msg);
      
  }
}

void receiveOSC(OSCMessage &msg) {
  int msgSize = msg.size();
  String oscAddress;
  char addressBuffer[99];
  msg.getAddress(addressBuffer);
  
  lastMessageAddress = String(addressBuffer);
  lastMessageValues = makeOSCString(msg);
  lastMessageTime = millis();

  if(state == RECORDING)
    writeMessageToFile();
  
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


//FILE-HANDLING----------------------------
void writeMessageToFile(){
    //write to
    //fileName
    
    //getRecordingDuration()
    //lastMessageAddress
    //lastMessageValues
}




//BUTTONS----------------------------------------------
// The event handler for both buttons.
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {

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
      if (button->getPin() == BUTTON_BOTTOM){
        reinitConnection();
      }
      break;
    
    //PRESSES
    case AceButton::kEventPressed:
      if (button->getPin() == BUTTON_TOP)
        toggleRecording();
      break;
  }
}

//Toggle recording
void toggleRecording(){
  if(state == READY){
    //generate and store uniqe filename
    //open file on SD
    //start timer
    recordingStartTime = millis();
    state = RECORDING;
  }
  else if(state == RECORDING){
     //close and save file
     state = READY;
  }
  else if(state == DISCONNECTED){
    //close file if it is being written
  }
}

unsigned long getRecordingDuration(){
  return millis() - recordingStartTime;
}

//WIFI------------------------------------------------------------
//Drop off current network and start up adhoc network for reconfiguration
void reinitConnection(){
    state = DISCONNECTED;
    if(state == RECORDING){
      //save and close file
    }
    updatePixel();
    updateDisplay();
    Config.immediateStart = true;
    Portal.config(Config);
    initConnection();
}



//PIXEL----------------------------------------------
void updatePixel(){
    switch(state){
      case DISCONNECTED:
      strip.setPixelColor(0, 30, 15, 0); // yellow orange
      break;
      case READY:
        strip.setPixelColor(0, 0, 10, 0);  // green
      break;
      case RECORDING:
        strip.setPixelColor(0, 20, 0, 0);  // red
      break;            
    }

    //if recent data
    if(millis() - lastMessageTime < 10)
      strip.setPixelColor(0, 0, 0, 30); //todo make this blink/pulse

    //if recent start
    //WHITE (flash)
    if(getRecordingDuration() < 500)
      strip.setPixelColor(0, 100, 100, 100); //todo make this blink/pulse

    strip.show();
}


//DISPLAY-----------------------------------------------------
void updateDisplay(){
    switch(state){
      
      case DISCONNECTED:
        display.clear();
        display.setFont(ArialMT_Plain_10);
        display.drawString( 0, 0, "DISCONNECTED");
        display.drawString( 0, 26, "Join packetPunk");
        display.drawString( 0, 36, "goto 172.217.28.1");
        display.drawString( 0, 46, "to configure");
        display.display();
      break;

      
      case READY:
        display.clear();
        display.setFont(ArialMT_Plain_10);
        display.drawString( 0, 0, "READY");
        display.drawString( 0, 26, "ip: " + networkAddress);
        display.display();
      break;

   
      case RECORDING:
        //calculate time to display
        unsigned long totalSeconds = getRecordingDuration() / 1000;
        unsigned long totalMinutes = totalSeconds / 60;
        unsigned long secs = totalSeconds % 60;
        unsigned long mins = totalMinutes % 60;
        unsigned long hrs = totalMinutes / 60;
        //enough room for 9999 hours       
        char timeBuffer[22];
        sprintf(timeBuffer, "RECORDING   %d:%02d:%02d", hrs, mins, secs);

        display.clear();
        display.setFont(ArialMT_Plain_10);
        display.drawString( 0, 0, timeBuffer );
        display.drawStringMaxWidth( 0, 26, 128, lastMessageAddress + " " + lastMessageValues );   
        display.display();
      break;            
    }
}



////SOUND-----------------------------------------------------
//void soundone() {
////  if(millis()/2.5 % 2 > 0)
////    digitalWrite(BUZZER, LOW);
////  else
////    digitalWrite(BUZZER, HIGH);
//}
