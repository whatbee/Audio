/*
 * TODO:
    * Test if fast analog read works
    * Print single pixels on OLED
    * Get horizontal scroll working (on pixels, text, anything)
    * Try to get horizontal scroll to work as hoped
    * Either tie everything together into oscilloscope, or remake Hscroll to work as planned (use arrays)
    * Add options for different magnifications (use multiplex registers)
 * 
 */


void setup() {
  cli();//disable interrupts
  
  //set up continuous sampling of analog pin 0
  
  //clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;
  
  ADMUX = 0;//Clear ADMUX register
  ADMUX |= (1 << REFS0); //set reference AVcc - comment this out to use AREF
  ADMUX |= (1 << ADLAR); //left align ADC
  //since I'm reading A0, I don't need to specifiy which analog pin I want to read from (0 is default)
  
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/32=500kHz
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements
  
  sei();//enable interrupts

  Serial.begin(9600);
  
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
Serial.begin(9600);
*/
}

ISR(ADC_vect){
Serial.print(ADCL);
Serial.print(",");
Serial.println(ADCH);
}


void loop() {
 // uint8_t result = ADCH; // top 8 bits of ADC
  //Serial.println(result);
  //delay(100);

}
