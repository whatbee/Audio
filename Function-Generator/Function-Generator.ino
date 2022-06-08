/*        To do:
 * remove interrupt functionality, put encoder() in updateControl() loop
 * 
 * 
 * 
 * 
 */


#define DONT_USE_MOZZI


#ifndef DONT_USE_MOZZI
#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
#endif

#include <Wire.h>
#include <U8g2lib.h>
#define U8X8_HAVE_HW_I2C


char *textName[] = {"Sine", "100Hz", "1.75v", "High"};
uint8_t textWidth[4] = {32,32,32,32};
const uint8_t textPosX[4] = {0, 84, 0, 84};
const uint8_t textPosY[4] = {16, 16, 60, 60};
uint8_t frameSelect=0;

/*    encoder stuff    */
const byte enPinA = 3;
const byte enPinB = 2;
bool enPinALast = LOW;
int16_t encoderVal=0;
int16_t newEnVal=0;
int16_t incEnVal=0;
uint16_t ISRtime=0;
const uint16_t ISRinterval=250;
/*    encoder stuff    */

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0,SCL,SDA,U8X8_PIN_NONE); // constructor for OLED library

void setup(void) {
  #ifndef DONT_USE_MOZZI
  startMozzi();
  aSin.setFreq(1000);
  #endif
  
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB10_tr);

  for (byte i=0; i<4; i++){ //calculate the widths of each string, then store them in the relevant array
   textWidth[i]  = u8g2.getStrWidth(textName[i]);
  }

  //pinMode(12, INPUT_PULLUP);
  pinMode(enPinA, INPUT_PULLUP);
  pinMode(enPinB, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);


attachInterrupt(digitalPinToInterrupt(enPinA), encoderISR, FALLING);

}


int8_t encoder (){
  int8_t enPinAState = digitalRead(enPinA);
  if ((enPinAState == LOW) && (enPinALast == HIGH)){       // if falling (actually rising because pullup) edge of pinA,
  //if (enPinAState =! enPinALast){ 
      enPinALast = enPinAState;
    if (digitalRead(enPinB) == LOW) {                     // and pinB is low,
      return -1;                                          // then it's going CCW so return -1
      }
    else {
      return 1;                                           // otherwise it's going CW so return 1
      }
  }
  enPinALast = enPinAState;
  return 0;                                               // if no rising edge it's not moving so return 0 
}

/*

void MakeFrame(uint8_t x){ //place frame around chosen section
  if (frameSelect > 4){frameSelect=0;}
  u8g2.drawFrame(textPosX[x]-2,textPosY[x]-2,textWidth[x],32);
}

void DrawText1(){
  for (byte i; i<1; i++){
  byte index = i;
  u8g2.drawStr(textPosX[index],textPosY[index],textName[index]);
    }
}

void DrawText2(){ //unrolled version of DrawText - only this version works for some reason
  u8g2.drawStr(textPosX[0],textPosY[0],textName[0]);
  u8g2.drawStr(textPosX[1],textPosY[1],textName[1]);
  u8g2.drawStr(textPosX[2],textPosY[2],textName[2]);
  u8g2.drawStr(textPosX[3],textPosY[3],textName[3]);
}

*/

void encoderISR(){
  if (millis() - ISRtime > ISRinterval){ 
    if (digitalRead(enPinB)){
      incEnVal++;
      }
    else {
      incEnVal--;
      }
      ISRtime=millis();
  }
}

#ifndef DONT_USE_MOZZI
void updateControl(){
  
}

AudioOutput_t updateAudio(){
  return MonoOutput::from16Bit(aSin.next() * gain); // 8 bits waveform * 8 bits gain makes 16 bits
}

#endif



void loop(void) {

  noInterrupts();
  encoderVal += incEnVal; // add the displayed value to the value incremented since last encoder polling
  incEnVal=0;             // reset incremental value
  interrupts();
  
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 20);
    u8g2.print(encoderVal);
    
  } while (u8g2.nextPage());


#ifndef DONT_USE_MOZZI
audioHook();
#endif
}
