#include <Adafruit_NeoPixel.h>
#include "IRLibAll.h"
#include <SoftwareSerial.h>

// INFRARED START_______________________________________________________
IRrecvPCI myReceiver(2); //Create a receiver object to listen on pin 2
IRdecodeNEC  myDecoder; //Create a decoder object 
bool modeSelect = false;
unsigned long previousSignal = 0;
// INFRARED END_________________________________________________________

// NEOPIXELS START_______________________________________________________
#define PIN       11 // The Data Pin to the strip
#define NUMPIXELS 60 // Number of LEDs on strip
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// NEOPIXELS END_________________________________________________________
// BLUEOOTH START
const int txPin = 1; //pin for hc05 communication
const int rxPin = 0; //pin for hc05 communication
// SoftwareSerial BTSerial(rxPin, txPin); // RX, TX
// BLUEOOTH END
//TYPEDEFS______________________________________________________________
typedef enum mode {sColour = 0, fade = 1, multiFade = 2, endToEndFade = 3, mixFade = 4, mixFadeChaos = 5, selectorMode = 6, collision = 7, areaFade = 8, randomTotal = 9, surroundingCollision = 10} MODE;
typedef enum channel {rChannel = 0, gChannel = 1, bChannel = 2} CHANNEL; 
struct rgb {
  float colour[3] = {0,0,0};
  CHANNEL rgbchannel;
};
//TYPEDEFS END__________________________________________________________
//GENERAL SETTINGS______________________________________________________
MODE mode = fade;
float speedFactor = 10;
struct rgb singleColour;
//GENERAL SETTINGS END__________________________________________________
//FADE SETTINGS_________________________________________________________
//typedef unsigned char colour[3];
struct rgb areaFadeTargets[60];
int activeAreas = 3;
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
  singleColour.colour[0] = 255;
  singleColour.colour[1] = 127;
  singleColour.colour[2] = 36;
  singleColour.rgbchannel = rChannel;
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  doSingleColour();


  for(int i = 0; i < activeAreas; i++)
  {
    areaFadeTargets[i].colour[0] = random(255);
    areaFadeTargets[i].colour[1] = random(255);
    areaFadeTargets[i].colour[2] = random(255);
  }
  
  Serial.println("The bluetooth gates are open.\n");
  //INFRARED
  while (!Serial);
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
}

void doMultiFade(bool endToEnd, bool mixFade)
{
  if(currentFadePixel == fadeTargetPixel)
  {
    lastFadeTarget[0] = fadeTarget[0];
    lastFadeTarget[1] = fadeTarget[1];
    lastFadeTarget[2] = fadeTarget[2];
    fadeTarget[0] = random(255);
    fadeTarget[1] = random(255);
    fadeTarget[2] = random(255);
    if(!endToEnd)
      fadeTargetPixel = random(60);
    else{
      if(fadeTargetPixel > 0)
        fadeTargetPixel = -1;
      else
        fadeTargetPixel = 60;      
    }
    distance = abs(currentFadePixel - fadeTargetPixel);
  }
  else
  {
    if(currentFadePixel < fadeTargetPixel)
      currentFadePixel++;
    else if(currentFadePixel > fadeTargetPixel)
      currentFadePixel--;
    if(!mixFade)
      pixels.setPixelColor(currentFadePixel, pixels.Color(fadeTarget[0],fadeTarget[1],fadeTarget[2]));
    else
    {
//      unsigned char r = lastFadeTarget[0];
//      unsigned char g = lastFadeTarget[1];
//      unsigned char b = lastFadeTarget[2];
      //EXPERIMENTAL:
      unsigned char r = (pixels.getPixelColor(currentFadePixel) >> 16);
      unsigned char g = (pixels.getPixelColor(currentFadePixel) >> 8);
      unsigned char b = (pixels.getPixelColor(currentFadePixel));
      int currentDistance = abs(currentFadePixel - fadeTargetPixel);
      float factor = 1.0f - (float)currentDistance / (float)distance;
      pixels.setPixelColor(currentFadePixel, pixels.Color(r + (char)(factor * (float)(fadeTarget[0] - r)),g + (char)(factor * (float)(fadeTarget[1] - g)),b + (char)(factor * (float)(fadeTarget[2]-b))));
    }
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void doRandom(bool moving)
{
  if(!moving)
  {
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(random(255), random(255), random(255)));
    }
  }
  else
  {
    if(currentFadePixel == fadeTargetPixel)
    {
        if(fadeTargetPixel > 0)
          fadeTargetPixel = -1;
        else
          fadeTargetPixel = 60;      
    }
    else
    {
      if(currentFadePixel < fadeTargetPixel)
        currentFadePixel++;
      else if(currentFadePixel > fadeTargetPixel)
        currentFadePixel--;
      pixels.setPixelColor(currentFadePixel, pixels.Color(random(255), random(255), random(255)));
    }
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}
void loop()
{  
  listenBluetooth();
  listenInfrared();
  
  switch(mode){
    case sColour:
    {
      //manual mode
      //doSingleColour();
      delay(500.0);
      break;
    }
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
    case randomTotal:
    {
      doRandom(false);
      delay(50.0 / speedFactor);
      break;
    }
  }
}
