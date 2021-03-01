#include <Adafruit_NeoPixel.h>
#include "IRLibAll.h"
#include <SoftwareSerial.h>

// NEOPIXELS START_______________________________________________________
#define PIN       11 // The Data Pin to the strip
#define NUMPIXELS 60 // Number of LEDs on strip
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// NEOPIXELS END_________________________________________________________
// INFRARED START_______________________________________________________
IRrecvPCI myReceiver(2); //Create a receiver object to listen on pin 2
IRdecodeNEC  myDecoder; //Create a decoder object 
bool modeSelect = false;
unsigned long previousSignal = 0;
// INFRARED END_________________________________________________________
// BLUEOOTH START
const int txPin = 1; //pin for hc05 communication
const int rxPin = 0; //pin for hc05 communication
// SoftwareSerial BTSerial(rxPin, txPin); // RX, TX
// BLUEOOTH END

//TYPEDEFS______________________________________________________________
typedef enum mode {staticColour = 0, fade = 1, multiFade = 2, endToEndFade = 3, mixFade = 4, mixFadeChaos = 5, selectorMode = 6, collision = 7, areaFade = 8, surroundingCollision = 10} MODE;
typedef enum channel {rChannel = 0, gChannel = 1, bChannel = 2} CHANNEL; 
struct rgb {
  float colour[3] = {0,0,0};
  CHANNEL rgbchannel;
};
//TYPEDEFS END__________________________________________________________
//GENERAL SETTINGS______________________________________________________
MODE mode = staticColour;
float speedFactor = 10;
struct rgb singleColour;
float dimFactor = 0.1;
bool activateMixing = true;
//GENERAL SETTINGS END__________________________________________________
//FADE SETTINGS_________________________________________________________
struct rgb areaFadeTargets[60];
int activeAreas = 2;
float fadeStep = 100;
unsigned char fadeTarget[3] = {255,255,255};
unsigned char lastFadeTarget[3] = {0,0,0};
int fadeTargetPixel = 60;
int currentFadePixel = 0;
int distance = 60;
struct rgb stripPixels[NUMPIXELS];
//FADE SETTINGS END_____________________________________________________

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600); 
  delay(100);  
  // NEOPIXELS
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  areaFadeTargets[0].colour[0] = 255;
  areaFadeTargets[0].colour[1] = 127;
  areaFadeTargets[0].colour[2] = 36;
  areaFadeTargets[0].rgbchannel = rChannel;

  for(int i = 1; i < activeAreas; i++)
  {
    areaFadeTargets[i].colour[0] = random(255);
    areaFadeTargets[i].colour[1] = random(255);
    areaFadeTargets[i].colour[2] = random(255);
  }
  if(mode == staticColour && activeAreas == 1)
    doStaticColour(areaFadeTargets[0]);
  else
    doStaticColour();
  
  Serial.println("The bluetooth gates are open.\n");
  //INFRARED
  while (!Serial);
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
}

void loop()
{  
  listenBluetooth();
  listenInfrared();
  switch(mode){
    case fade:
    {
      activeAreas = 1;
      //fade to autogenerated colours
      doAreaFade();
      delay(50.0 / speedFactor);
      break;
    }
    case multiFade:
    {
      //fade to autogenerated colours
      doMultiFade(false, false);
      delay(50.0 / speedFactor);
      break;
    }
    case endToEndFade:
    {
      //fade to autogenerated colours
      doMultiFade(true, false);
      delay(50.0 / speedFactor);
      break;
    }    
    case mixFade:
    {
      //fade to autogenerated colours
      doMultiFade(true, true);
      delay(50.0 / speedFactor);
      break;
    }    
    case mixFadeChaos:
    {
      //fade to autogenerated colours
      doMultiFade(false, true);
      delay(50.0 / speedFactor);
      break;
    }
    case selectorMode:
    {
      delay(1000);
      break;
    }
    case collision:
    {
      doCollision(false);
      delay(100.0 / speedFactor);
      break;
    }
    case surroundingCollision:
    {
      doCollision(true);
      delay(100.0 / speedFactor);
      break;
    }
    case areaFade:
    {
      doAreaFade();
      delay(50.0 / speedFactor);
      break;
    }
  }
  if(activateMixing)
    mixAreas();
  updateStrip();
}