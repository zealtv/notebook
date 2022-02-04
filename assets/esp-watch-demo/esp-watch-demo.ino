#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <Adafruit_NeoPixel.h>
#include "SH1106.h"
#include "FS.h"
#include "SD_MMC.h"

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
char currentFilename[9] = "/000.log";
int numFiles = 0;
File currentFile;
char msgCharArray[999];

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

  delay(500);
  
  initSD();
  countFiles();

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
    recordingStartTime = millis();
    openFile();
    state = RECORDING;
  }
  else if(state == RECORDING){
     closeFile();
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
    if(getRecordingDuration() < 500){
      strip.setPixelColor(0, 100, 100, 100); //todo make this blink/pulse
//      buzz();
    }

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
        display.drawString( 0, 13, "< start");
        display.drawString( 0, 36, "ip:    " + networkAddress);
        display.drawString( 0, 49, "port: 9000");
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
        //this line displays incoming OSC
         //display.drawStringMaxWidth( 0, 26, 128, lastMessageAddress + " " + lastMessageValues );

        display.drawString( 0, 13, "< save");        
        display.setFont(ArialMT_Plain_24);
        display.drawString(0, 39, String(currentFilename));
        display.display();
      break;            
    }
}



////SOUND-----------------------------------------------------
void buzz() {
  if(millis() % 2 == 0)
    digitalWrite(BUZZER, LOW);
  else
    digitalWrite(BUZZER, HIGH);
}



//FILE-HANDLING----------------------------

//COUNT FILES--------------------
int countFiles(){
  int count = 0;

  Serial.printf("Counting files...\n");
  File root = SD_MMC.open("/");
  File file = root.openNextFile();
  while(file){
      if(!file.isDirectory()){
        Serial.print("  FILE: ");
        Serial.print(file.name());
        Serial.print("  SIZE: ");
        Serial.println(file.size());
        count++;
      }
      file = root.openNextFile();
  }
  Serial.printf("Found %d files\n", count);
  numFiles = count;
  sprintf(currentFilename, "/%03d.LOG", numFiles);
  Serial.println("next filename: " + String(currentFilename));  
}


//INITIALIZE---------------
int initSD(){
  Serial.println("Initialisting SD...");
  
    if(!SD_MMC.begin()){
        Serial.println("Card Mount Failed");
        return -1;
    }

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
}


//OPEN FILE-------------------
void openFile(){
  //delete current file if it already exists (overwrite)
  // //delete a file:
  if (SD_MMC.exists(currentFilename)){
    Serial.println("Deleting existing " + String(currentFilename));
    SD_MMC.remove(currentFilename);
    Serial.println("Done");
  } 

  Serial.println("Opening " + String(currentFilename));
  currentFile = SD_MMC.open(currentFilename, FILE_WRITE);

  setSyncFlag();
//  currentFile.println("hi");
}

//CLOSE FILE-----------------------
void closeFile(){
  Serial.println("closing " + String(currentFilename));
  setCloseFlag();
  currentFile.close();
  countFiles();
}

//WRITE OSC MESSAGE TO FILE-----------------------
void writeMessageToFile(){
    //write to
    //fileName

    String thisMessage = String(getRecordingDuration()) + " " + lastMessageAddress + " " + lastMessageValues + "\n";
//    sprintf(msgCharArray, "%d %s %s\n", getRecordingDuration(), lastMessageAddress, lastMessageValues);
    
//    currentFile.print(msgCharArray);
    currentFile.print(thisMessage);
}


//SET SYNC FLAG---------------
void setSyncFlag(){
      currentFile.print(String(getRecordingDuration()) + " /packetpunk/sync 1\n");
}


//SET CLOSE FLAG---------------
void setCloseFlag(){
      currentFile.print(String(getRecordingDuration()) + " /packetpunk/close 1\n");
}
