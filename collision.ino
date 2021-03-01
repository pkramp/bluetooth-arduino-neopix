int progressCounter = 0;
int direction = -1;
void doCollision(bool showSurroundingLights)
{
  bool particle = true;
  if(particle)
  {
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      for(int channel = 0; channel < 3; channel++)
        stripPixels[i].colour[channel] = 0;
    }
  }
  if(progressCounter == 0)
  {
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      for(int channel = 0; channel < 3; channel++)
        stripPixels[i].colour[channel] = 0;
    }
  }
  if(progressCounter == 0 || progressCounter == 30)
  {  
    fadeTarget[0] = random(255);
    fadeTarget[1] = random(255);
    fadeTarget[2] = random(255);
    lastFadeTarget[0] = random(255);
    lastFadeTarget[1] = random(255);
    lastFadeTarget[2] = random(255);
    direction *= -1;
  }

  if(showSurroundingLights)
    surroundingLights();
  pixels.setPixelColor(progressCounter, pixels.Color(fadeTarget[0],fadeTarget[1],fadeTarget[2]));
  pixels.setPixelColor(NUMPIXELS - progressCounter, pixels.Color(lastFadeTarget[0],lastFadeTarget[1],lastFadeTarget[2]));
  
  progressCounter += direction;
  pixels.show();
}

void surroundingLights()
{
  int range = 15;
    for(unsigned int i = 1; i <= range; i++)
    {
      float brightness = (range - i) * 0.06;
      if(progressCounter - i >= 0)
      {
        pixels.setPixelColor(progressCounter-i, pixels.Color((char)((float)fadeTarget[0]*brightness), (char)((float)fadeTarget[1]*brightness), (char)((float)fadeTarget[2]*brightness)));
        pixels.setPixelColor(NUMPIXELS - progressCounter-i, pixels.Color((char)((float)lastFadeTarget[0]*brightness), (char)((float)lastFadeTarget[1]*brightness), (char)((float)lastFadeTarget[2]*brightness)));
      }
      if(progressCounter + i < 60)
      {
        pixels.setPixelColor(progressCounter+i, pixels.Color((char)((float)fadeTarget[0]*brightness), (char)((float)fadeTarget[1]*brightness), (char)((float)fadeTarget[2]*brightness)));
        pixels.setPixelColor(NUMPIXELS - progressCounter+i, pixels.Color((char)((float)lastFadeTarget[0]*brightness), (char)((float)lastFadeTarget[1]*brightness), (char)((float)lastFadeTarget[2]*brightness)));
      }
      //if(progressCounter + i<
    }
}
