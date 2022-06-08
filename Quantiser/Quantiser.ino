/*
  

*/
#include <SPI.h>

const int CVpin = A0;
const int diffAmt = 10;
const int gateIn = 3;
unsigned int note[128];

void setup() {
  pinMode(gateIn, INPUT_PULLUP);

  for (int i=0.0; i<128; i++){
    note[i] = static_cast<unsigned int>round(((i/12.0)*4096.0)/(127.0/12.0)); //  create LUT
  }

}

void DAC(unsigned int x, bool channel){
  if (channel){x = x|0x9000;} // if channel is 1, use bitmask to select DAC-b and turn on dac (shutdown bit high)
  else {x = x|0x1000;}        // otherwise, just turn on dac
  SPI.begin();
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  SPI.transfer16(x);
  SPI.endTransaction();
}


void loop() {
  int lastCV;
  if (!digitalRead(gateIn)){
    int CV = analogRead(CVpin);
    if (CV-lastCV>diffAmt){
      DAC(note[CV/8],0);
    }
    lastCV = CV;
  }
  
  

}
