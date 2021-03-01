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
