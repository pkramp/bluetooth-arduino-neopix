void listenInfrared()
{
  const unsigned long power = 16753245;
  const unsigned long volPlus = 16736925;
  const unsigned long funcStop = 16769565;
  const unsigned long left = 16720605;
  const unsigned long playPause = 16712445;
  const unsigned long right = 16761405;
  const unsigned long down = 16769055;
  const unsigned long volMinus = 16754775;
  const unsigned long up = 16748655;
  const unsigned long zero = 16738455;
  const unsigned long eq = 16750695;
  const unsigned long stRept = 16756815;
  const unsigned long one = 16724175;
  const unsigned long two = 16718055;
  const unsigned long three = 16743045;
  const unsigned long four = 16716015;
  const unsigned long five = 16726215;
  const unsigned long six = 16734885;
  const unsigned long seven = 16728765;
  const unsigned long eight = 16730805;
  const unsigned long nine = 16732845;

  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
    myDecoder.dumpResults(false);  //Now print results. Use false for less detail
    Serial.println(myDecoder.value);
    myReceiver.enableIRIn();      //Restart receiver
  }
  else{
    return;
  }
  unsigned long value = myDecoder.value;
  if(!value)
  {
    delay(3000);
    return;
  }
  if(value != 4294967295){
    previousSignal = myDecoder.value;
  }
  if (value != funcStop)
  {

    value = previousSignal;
    Serial.println(value);
  }
  myDecoder.value = 0;
  
  if(value == funcStop)
  {
    modeSelect = true;;
    Serial.println(modeSelect);
    mode = selectorMode;
    myDecoder.value = 0;
    return;
  }
  if(value != 0 && value != 4294967295)
  {
     switch (value)
      {
       case left:
        speedFactor *= 0.8;
        Serial.println("Decrease Speed" );
        break;
       case right:
        speedFactor *= 1.2;
        Serial.println("Increase Speed" );
        break;
      }
    if(modeSelect)
    {
      switch (value)
      {
       case zero:
        mode = sColour;
        break;
       case one:
        mode = fade;
        break;
       case two:
        mode = multiFade;
        break;
       case three:
        mode = endToEndFade;
        break;
       case four:
        mode = mixFade;
        break;
       case five:
        mode = mixFadeChaos;
        break;
      }
      char snum[5];
      itoa(mode, snum, 10);
      Serial.println("Selected Mode" );
      Serial.println(snum);
      modeSelect = false;
    }
    else if (mode == sColour){
      switch (value)
      {
       case one:
        singleColour.rgbchannel = rChannel;
        break;
       case two:
        singleColour.rgbchannel = gChannel;
        break;
       case three:
        singleColour.rgbchannel = bChannel;
        break;
      case volPlus:
       if(singleColour.colour[singleColour.rgbchannel]!= 255)
       {
        singleColour.colour[singleColour.rgbchannel]++;
       }
       break;
      case volMinus:
       if(singleColour.colour[singleColour.rgbchannel] != 0)
       {
        singleColour.colour[singleColour.rgbchannel]--;
       }
       break;
      }     
    }
  }
}

void listenBluetooth(){
  
 char *strings[60];
 char *ptr = NULL;
 String str = "";
 
 if(Serial.available() > 0){ // Checks whether data is comming from the serial port
    str = Serial.readString();
    Serial.println(str);
 }
 if(str.length()==1) //must be mode
 {
  const unsigned char value = (unsigned char)str[0];
    switch (value)
      {
       case '0':
        Serial.println("SColour");
        mode = sColour;
        break;
       case '1':
        Serial.println("Fade");
        mode = fade;
        break;
       case '2':
        Serial.println("multiFade");
        mode = multiFade;
        break;
       case '3':
        Serial.println("endToEndFade");
        mode = endToEndFade;
        break;
       case '4':
        Serial.println("mixFade");
        mode = mixFade;
        break;
       case '5':
        Serial.println("mixFadeChaos");
        mode = mixFadeChaos;
        break;
       case '6':
        Serial.println("collision");
        mode = mixFadeChaos;
        break;
       case '7':
        Serial.println("areaFade");
        mode = areaFade;
        break;
       case '8':
        Serial.println("randomTotal");
        mode = randomTotal;
        break;
       case '9':
        Serial.println("surroundingCollision");
        mode = surroundingCollision;
        break;
         case '-':
          speedFactor *= 0.9;
          break;
         case '+':
          speedFactor *= 1.1;
      }
 }
 byte index = 0;
 ptr = strtok(str.c_str(), ",");  // takes a list of delimiters
 while(ptr != NULL)
 {
     strings[index] = ptr;
     index++;
     ptr = strtok(NULL, ",");  // takes a list of delimiters
 }
 if(index == 2) // assume it is speed factor
 {
  speedFactor = (float)atoi(strings[0]) + 0.1 * (float)atoi(strings[1]);
 }
 else if(index != 0 && index %3 == 0)
 {
  Serial.println(index);
  int divisor = index / 3;
  for(int i = 0; i < divisor; i++)
  {
    Serial.println(i);
    int pixRange = NUMPIXELS/divisor;
    Serial.println(pixRange);
    for(int pix=pixRange*i; pix<pixRange*(i+1); pix++) { // For each pixel...
      pixels.setPixelColor(pix, pixels.Color((unsigned char)atoi(strings[i*3]), (unsigned char)atoi(strings[i*3+1]), (unsigned char)atoi(strings[i*3+2])));
    }
    pixels.show();   // Send the updated pixel colors to the hardware.
  }
 }
}
