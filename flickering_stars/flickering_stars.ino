// Based on the WS2803-TestCode from Thomas Olson
//
// FLICKERING STARS
//
// This Code provides a way to interact with the WS2803 to display flickering stars.
// It tries to simulate the typical behavior which you can experience when looking to the sky at night.
// At first, you see only a few, bright shining stars. And after a while, you'll see more and more stars.
// They're flickering at different speeds. This is something the code tries to emit.
//
// WS2803 pin4 CKI - > ws2803_clockPin
// WS2803 pin5 SDI - > ws2803_dataPin
// WS2803 pin2 IREF = Rext to GND.
// WS2803 pin6-23 = OUT0-17

// The byte order is 0-17. The bit order is MSB-LSB. The chip order is Last to First.

const int ws2803_clockPin = 4;
const int ws2803_dataPin = 3;

#define nLEDs 36

uint8_t ledBar[nLEDs][3]; // Array representing LED PWM levels (byte size)
int nRandomNumber;

void calculateTwinkling(uint8_t nLED)
{
  if (ledBar[nLED][1] == 1)
  {
    if(ledBar[nLED][0] == 16)
    {
      ledBar[nLED][1] = 0;
    }
    else
    {
      if(random(1,2) == 1){
         ledBar[nLED][0] = ledBar[nLED][0] - 1;
      }
    }
  }
  else
  {
    if(ledBar[nLED][0] == 255)
    {
      ledBar[nLED][1] = 1;
    }
    else
    {
      if(random(1,2) == 1){
         ledBar[nLED][0] = ledBar[nLED][0] + 1;
      }
    }    
  }
}

void setup() {
 
  pinMode(ws2803_clockPin, OUTPUT);
  pinMode(ws2803_dataPin, OUTPUT);

  randomSeed(analogRead(0));

// Initialize WS2803 - Clock needs to be low at least 600us to prepare itself.
  digitalWrite(ws2803_clockPin, LOW);
  delayMicroseconds(600);

// Initialize the ledBar array - all LEDs OFF.
  for(int wsOut = 0; wsOut < nLEDs; wsOut++){
    ledBar[wsOut][0] = 16;
    ledBar[wsOut][1] = 0;
    ledBar[wsOut][2] = 0;
  }
  
  loadWS2803();
}

void loop() {
// Then turn them ALL off
/*
  for(int wsOut = 0; wsOut < nLEDs; wsOut++){
    ledBar[wsOut][0] = 0x00; // Full OFF
    ledBar[wsOut][1] = 0; // Full OFF
  }
  loadWS2803();
  */
// Then try twinkling

  nRandomNumber = 16000;

  if(millis() > 240000)
  {
    nRandomNumber = 126;
  }
  else if (millis() > 120000)
  {
    nRandomNumber = 8125;
  }

    for(int wsOut = 0; wsOut < nLEDs; wsOut++){
      if(ledBar[wsOut][2] == 1)
      {
        calculateTwinkling(wsOut);
      }
      else
      {
        if(random(1,nRandomNumber) == 59)
        {
          ledBar[wsOut][2] = 1;
        }
        else
        {
          ledBar[wsOut][2] = 0;
        }
      }
    }
    loadWS2803();
  

// Simple test - LED Chaser
/*
  for(int wsOut = 0; wsOut < nLEDs; wsOut++){
    ledBar[wsOut] = 0x3C; // 23% brightness
    loadWS2803();

    delay(100);  // ON time for each LED during chase.

    ledBar[wsOut][0] = 0x00; // Full OFF
    loadWS2803();
  }*/
// Simple test - increase brightness to max and then dim to min each LED
  /*for(int wsOut = 0; wsOut < nLEDs; wsOut++){
    for(int iOut = 0; iOut < 256; iOut++){ // brighten one LED
      ledBar[wsOut] = iOut;
      loadWS2803();
    }
    for(int iOut = 0; iOut < 256; iOut++){ // now dim that one LED
      ledBar[wsOut] = (uint8_t)0xFF & 255-iOut;
      loadWS2803();
    }
  }*/
} //loop

void loadWS2803(){
    for (int wsOut = 0; wsOut < nLEDs; wsOut++){
    shiftOut(ws2803_dataPin, ws2803_clockPin, MSBFIRST, ledBar[wsOut][0]);
    }
    delayMicroseconds(600); // 600us needed to reset WS2803s
}


