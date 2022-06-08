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
  
DIDR0 = 0b11111100; // turn off digital buffer, leave reserved bits as 0
DIDR1 = 0x00; // turn off digital buffer
//ADCSRA = 0b01100111; // prescale 64 (250kHz), disable ADC interrupt, start auto ADC
ADCSRA = 0b10100111; // prescale 32(500kHz), disable ADC interrupt, start auto ADC
//ADCSRA = 0b00100111; // prescale 16(1MHz), disable ADC interrupt, start auto ADC
ADCSRB = 0x00; // trigger source free running mode
ACSR =  0x01;
ADMUX = 0b00000100; // select ADC0, left adjust, use AREF

Serial.begin(9600);
}

void loop() {
  uint8_t result = ADCH; // top 8 bits of ADC
  Serial.println(result);
  delay(100);

}
