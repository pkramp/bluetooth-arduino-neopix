void doSingleColour()
{
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(singleColour.colour[0], singleColour.colour[1], singleColour.colour[2]));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}


void doSingleColour(char colour[])
{
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(colour[0], colour[1], colour[2]));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}

void doAreaFade()
{
  int areaSize = NUMPIXELS/activeAreas;
  for(int area = 0; area < activeAreas; area++)
  {
    unsigned int refreshCount = 0;
    for(int pixel = 0 + areaSize*area; pixel < (area+1)*areaSize; pixel++)
    {
     for(int channel = 0; channel<3; channel++)
       stripPixels[pixel].colour[channel] += ( areaFadeTargets[area].colour[channel] - stripPixels[pixel].colour[channel])/fadeStep;
    }
  }
  fadeStep-=1;
  if(fadeStep<=0)
  {
   fadeStep = 100;
   for(int i = 0; i < activeAreas; i++)
   {
     struct rgb newColour = getRandomColour();
     for(int channel = 0; channel<3; channel++)
     {
      areaFadeTargets[i].colour[channel] = newColour.colour[channel];
     }
   }
  }
  updateStrip();
}

void updateStrip()
{
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(stripPixels[i].colour[0]*dimFactor, stripPixels[i].colour[1]*dimFactor, stripPixels[i].colour[2]*dimFactor));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}

struct rgb getRandomColour()
{
  struct rgb randomColour;
  float sum = 0;
  for(int channel = 0; channel<3; channel++)
  {
   randomColour.colour[channel] = random(255);
   sum += randomColour.colour[channel];
  }
  // recursively get new random colour if colour too dark
  if(sum<100)
    return getRandomColour();

  float brightnessLevel = randomColour.colour[0] / 255.0;
  if(fabs(randomColour.colour[0] / 255.0 - randomColour.colour[1] / 255.0) < 0.1 &&
     fabs(randomColour.colour[0] / 255.0 - randomColour.colour[2] / 255.0) < 0.3 )
    return getRandomColour();
  return randomColour;
}
