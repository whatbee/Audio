/*
 * 
 * A fast oscilloscope on 0.96" on a OLED screen
 * 
 * TODO:
    * Test if fast analog read works - kinda
    * Print single pixels on OLED
    * Get horizontal scroll working (on pixels, text, anything)
    * Try to get horizontal scroll to work as hoped
    * Either tie everything together into oscilloscope, or remake Hscroll to work as planned (use arrays)
    * Add options for different magnifications (use multiplex registers)
 * 
 */
#include <SPI.h>

const byte CS = 4;
const byte dcPin = 3;
const byte reset = 2;

void setup() {
  pinMode(dcPin, OUTPUT);
  pinMode(reset, OUTPUT);
  pinMode(CS, OUTPUT);
  
  /*
  cli(); //disable interrupts
  ADCSRA = 0;
  ADCSRB = 0;
  
  ADMUX = 0;             // Clear ADMUX register
  ADMUX |= (1 << REFS0); // set reference AVcc - comment this out to use AREF
  ADMUX |= (1 << ADLAR); // left align ADC
  // defaults to A0, no need to set using register
  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler = 500kHz
  ADCSRA |= (1 << ADATE); //enable auto trigger
  ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements
  
  sei();//enable interrupts
  */
  Serial.begin(9600);
  SPI.begin();
/*  
DIDR0 = 0b11111100; // turn off digital buffer, leave reserved bits as 0
DIDR1 = 0x00; // turn off digital buffer
ADCSRB = 0x00; // trigger source free running mode
ACSR =  0x01;
//ADMUX = 0b00000100; // select ADC0, left adjust, use AREF
ADMUX = 0b00000110; // select ADC0, left adjust, use AVcc
//ADCSRA = 0b01100111; // prescale 64 (250kHz), disable ADC interrupt, start auto ADC
//ADCSRA = 0b00100111; // prescale 16(1MHz), disable ADC interrupt, start auto ADC
ADCSRA = 0b10101111; // prescale 32(500kHz), enable ADC interrupt, start auto ADC
*/



digitalWrite(reset, LOW);
delay(10);
digitalWrite(reset, HIGH);
digitalWrite(CS, LOW);



dcCmd(); // Set D/C pin high so the display interprets as a command
SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));

SPI.transfer(0xAE); // display off
SPI.transfer(0xD5); // set clockdiv
SPI.transfer(0x80); // suggested ratio
SPI.transfer(0xA8); // set multiplex
SPI.transfer(63);   // height
SPI.transfer(0xD3); // set display offset
SPI.transfer(0x00); // no offset
SPI.transfer(0x00); // set start line 0
SPI.transfer(0x8D); // charge pump
SPI.transfer(0x14); // internal Vcc
SPI.transfer(0x20); // memory mode
SPI.transfer(0x00); // idk
SPI.transfer(0xA1); // segment remap
SPI.transfer(0xC8); // Set COM output scan direction
SPI.transfer(0xDA); // set COM pins
SPI.transfer(0x02); // COM pins
SPI.transfer(0x81); // Set contrast
SPI.transfer(0x8F); // contrast
SPI.transfer(0xd9); // set precharge
SPI.transfer(0xF1); // precharge
SPI.transfer(0xDB); // Set VCOMH Deselect Level
SPI.transfer(0xA4); // Screen on
SPI.transfer(0xA6); // Normal display
SPI.transfer(0x2E); // Deactivate scroll
SPI.transfer(0xAF); // Display on

SPI.endTransaction();

  displayByte();

  for (int i=0; i<0xFF; i++){
    SPI.transfer(i);
  }

SPI.endTransaction();








}


void loop() {
 // uint8_t result = ADCH; // top 8 bits of ADC
  //Serial.println(result);
  //delay(100);

}








void displayByte(){
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(0x22); // Page address
  SPI.transfer(0x00);
  SPI.transfer(0xFF); // Page end
  SPI.transfer(0x21); // Column start address
  SPI.transfer(0x00);
  SPI.transfer(127);  // Column end address
  //SPI.endTransaction();
}

void dataByte(uint8_t x){
  digitalWrite(dcPin, LOW); // Set D/C pin high so the display interprets as a command
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(x);
  SPI.endTransaction();
  digitalWrite(dcPin, HIGH); // reset D/C low so the more common data bytes don't have to use digitalWrite every time
}

void sendByte(uint8_t x){
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(x);
  SPI.endTransaction();
}
void dcCmd(){ // set d/c pin high to signal command
  digitalWrite(dcPin, HIGH); // change this to port manipulation at some point
}
void dcDat(){ // set d/c pin low to signal data
  digitalWrite(dcPin, LOW);   // change this to port manipulation at some point
}
/*
ISR(ADC_vect){
Serial.print(ADCL);
Serial.print(",");
Serial.println(ADCH);
}
*/
