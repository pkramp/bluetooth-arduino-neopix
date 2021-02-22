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
     for(int channel = 0; channel<3; channel++)
        areaFadeTargets[i].colour[channel] = random(255);
   }
  }
  updateStrip();
}

void updateStrip()
{
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(stripPixels[i].colour[0], stripPixels[i].colour[1], stripPixels[i].colour[2]));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}
