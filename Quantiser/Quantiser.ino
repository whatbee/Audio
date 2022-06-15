/*
  Read CV in, and converts it to a quantised 1v/oct scale

*/
#include <SPI.h>
#include "LUTs.h"
#define QUANTISER_DEBUG

/*        Pins:         */
const int CVpin = A0;
const int gateIn = 3;

/*        Constants:         */
const int diffAmt = 10;
const uint8_t divisor=4;

unsigned int ADC_to_DAC[128];



void setup() {
  Serial.begin(9600);
  pinMode(gateIn, INPUT_PULLUP);
  populateNotes(0,0,3); //C major, 3rd octave
  
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
  #ifndef QUANTISER_DEBUG
  if (channel){x = x|0x9000;} // if channel is 1, use bitmask to select DAC-b and turn on dac (shutdown bit high)
  else {x = x|0x1000;}        // otherwise, just turn on dac
  SPI.begin();
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  SPI.transfer16(x);
  SPI.endTransaction();
  #endif
  #ifdef QUANTISER_DEBUG

  Serial.println(x);
  
  //uint8_t nPos = searchArr(x) % 12;

/* char buf[10];
    for (unsigned char i = 0; i < 5; i++){
        strcpy_P(buf, (PGM_P)pgm_read_word(&(string_table[i])));
  
    }
*/  
  //Serial.println(noteName[nPos]);
  //Serial.print(pgm_read_word_near(&noteName[nPos]));
  #endif
}

uint8_t searchArr(uint8_t in){ // find note number based on its DAC code
  for (int i=0; i<128; i++){
    if (pgm_read_word_near(&voctTable[i]) == in){
      return i;
    }
  }
  
}

void populateNotes(uint8_t note, uint8_t mode, uint8_t octave){ // Create LUT of DAC codes for every possible ADC reading
  uint8_t numberNotes = 128/divisor; // Number of possible notes to play

  /* First, make a LUT of ADC to note values: */
  ADC_to_DAC[0] =  note+octave*12; // Start with the first value being the same as the note (plus octaves)
  for (byte i=1; i<numberNotes; i++){
    uint8_t notePos = i+mode-1; // Mode offsets major scale by an amount
    notePos %= 7;             // Fit to an 8 note scale
    
    ADC_to_DAC[i] = ADC_to_DAC[i-1] + majorScale[notePos]; // Take previous note, add interval as determined by majorScale shifted by mode
    }
  
  #ifdef QUANTISER_DEBUG
  Serial.println("ADC to note values");
  Serial.print("{");
  for (int i=0; i<numberNotes; i++){
    Serial.print(ADC_to_DAC[i]);
    Serial.print(", ");
  }
  Serial.println("}");
  #endif



  /* Then, convert those note values to DAC codes: */
  for (byte i=0; i<numberNotes; i++){
        ADC_to_DAC[i] =  pgm_read_word_near(&voctTable[i]); // convert the note into a DAC code
  }


  #ifdef QUANTISER_DEBUG
  Serial.println();
  Serial.println("ADC to DAC values");
  Serial.print("{");
  for (int i=0; i<numberNotes; i++){
    Serial.print(ADC_to_DAC[i]);
    Serial.print(", ");
  }
  Serial.println("}");
  #endif
}
