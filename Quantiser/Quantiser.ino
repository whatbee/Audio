/*
  

*/
#include <SPI.h>
#include "LUTs.h"
const int CVpin = A0;
const int diffAmt = 10;
const int gateIn = 3;
unsigned int ADC_to_DAC[128];
uint8_t divisor=4;


void setup() {
  pinMode(gateIn, INPUT_PULLUP);
  populateNotes(0,0,1);

}

void loop() {
  int lastCV;
  if (!digitalRead(gateIn)){         // If gateIn is high,
    int CV = analogRead(CVpin);      // Read the CV in,
    if (CV-lastCV>diffAmt){          // If the CV has changed by a sufficient amount since last time,  
      DAC(ADC_to_DAC[CV/divisor],0); // Output the DAC code from a LUT.
    }
    lastCV = CV;
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


void populateNotes(uint8_t note, uint8_t mode, uint8_t octave){ // Create LUT of DAC codes for every possible ADC reading
  uint8_t numberNotes = 128/divisor; // Number of possible notes to play

  /* First, make a LUT of ADC to note values: */
  ADC_to_DAC[0] =  note; // Start with the first value being the same as the note
  for (byte i=1; i<numberNotes; i++){
    uint8_t notePos = i+mode; // Mode offsets major scale by an amount
    notePos %= 8;     // Mod 8 to fit it to an 8 note scale
    ADC_to_DAC[i] = ADC_to_DAC[i-1] + majorScale[notePos] + octave*12; // take previous note, add interval as determined by majorScale shifted by mode, add the relevant amount of octaves
    }

  /* Then, convert those note values to DAC codes: */
  for (byte i=0; i<numberNotes; i++){
        ADC_to_DAC[i] =  pgm_read_word_near(&voctTable[i]); // convert the note into a DAC code
  }
}
