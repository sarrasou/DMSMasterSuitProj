void pixelUpdate() {
  
  for (int i = 0; i < NUM_LEDS; i++) {

    if (colorCode == 0) { //RED
      leds[i] = CRGB(255, 0, 0);
      analogWrite(lcdR, 0);       //FYI: for analogWrite, esp8266 uses numbers between 0-1023
      analogWrite(lcdG, 1023);    //but, it's backwards, for full value, print zero, for no value, print 1023
      analogWrite(lcdB, 1023);
    }

    if (colorCode == 1) { //ORANGE
      leds[i] = CRGB(255, 120, 0);
      analogWrite(lcdR, 0);
      analogWrite(lcdG, 500);
      analogWrite(lcdB, 1023);
    }

    if (colorCode == 2) { //YELLOW
      leds[i] = CRGB(255, 255, 0);
      analogWrite(lcdR, 0);
      analogWrite(lcdG, 0);
      analogWrite(lcdB, 1023);
    }

    if (colorCode == 3) { //GREEN
      leds[i] = CRGB(0, 255, 0);
      analogWrite(lcdR, 1023);
      analogWrite(lcdG, 0);
      analogWrite(lcdB, 1023);
    }

    if (colorCode == 4) { //BLUE
      leds[i] = CRGB(0, 0, 255);
      analogWrite(lcdR, 1023);
      analogWrite(lcdG, 1023);
      analogWrite(lcdB, 0);
    }

    if (colorCode == 5) { //PURPLE
      leds[i] = CRGB(128, 12, 128);
      analogWrite(lcdR, 526);
      analogWrite(lcdG, 1023);
      analogWrite(lcdB, 526);
    }
    
    if (colorCode == 6) { //White
      leds[i] = CRGB::White;
      analogWrite(lcdR, 0);
      analogWrite(lcdG, 0);
      analogWrite(lcdB, 0);
    }

    else if (colorCode == 99) {
      leds[i] = CRGB::Black;
      analogWrite(lcdR, 1023);
      analogWrite(lcdG, 1023);
      analogWrite(lcdB, 1023);
    }

  }

  FastLED.show();
}

