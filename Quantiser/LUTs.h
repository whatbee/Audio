const uint8_t majorScale[] = {2, 2, 1, 2, 2, 2, 1};
/*
PROGMEM const char noteName0[] = "C";
PROGMEM const char noteName1[] = "C#";
PROGMEM const char noteName2[] = "D";
PROGMEM const char noteName3[] = "D#";
PROGMEM const char noteName4[] = "E";
PROGMEM const char noteName5[] = "F";
PROGMEM const char noteName6[] = "F#";
PROGMEM const char noteName7[] = "G";
PROGMEM const char noteName8[] = "G#";
PROGMEM const char noteName9[] = "A";
PROGMEM const char noteName10[] = "A#";
PROGMEM const char noteName11[] = "B";
//const char* noteName[] = {noteName0, noteName1, noteName2, noteName3, noteName4, noteName5, noteName6, noteName7, noteName8, noteName9, noteName10, noteName11}; // Starting at C
PGM_P PROGMEM const char* noteName[] = {noteName9, noteName10, noteName11, noteName0, noteName1, noteName2, noteName3, noteName4, noteName5, noteName6, noteName7, noteName8}; // Starting at A (as V/oct does)
*/

char* noteName[] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
/*
PROGMEM const char modeName0[] = "Ionian";
PROGMEM const char modeName1[] = "Dorian";
PROGMEM const char modeName2[] = "Phrygian";
PROGMEM const char modeName3[] = "Lydian";
PROGMEM const char modeName4[] = "Mixolydian";
PROGMEM const char modeName5[] = "Aeolian";
PROGMEM const char modeName6[] = "Locrian";
PGM_P PROGMEM const char* modeName[]  = {modeName0, modeName1, modeName2, modeName3, modeName4, modeName5, modeName6};

PROGMEM const char modeNum0[] = "I";
PROGMEM const char modeNum1[] = "II";
PROGMEM const char modeNum2[] = "III";
PROGMEM const char modeNum3[] = "IV";
PROGMEM const char modeNum4[] = "V";
PROGMEM const char modeNum5[] = "VI";
PROGMEM const char modeNum6[] = "VII";
PGM_P PROGMEM const char* modeNum[]  = {modeNum0, modeNum1, modeNum2, modeNum3, modeNum4, modeNum5, modeNum6};
*/

/*			Python generation of voctTable:

import sys
if sys.version_info[0] < 3:
    from __future__ import division # why did python 3 change the way division works???
import math
maxbits = (2**12) # bit depth of DAC
tablesize = 128   # number of notes in table
semitones = 12    # number of semitones per octave

f= open("voct table.txt","w")
f.write("const uint16_t voctTable [] = {")
for i in range(tablesize):
	x= math.trunc(round((((i/semitones)*maxbits)/((tablesize-1)/semitones))))	
	f.write("%s" % x)
	if i<(tablesize-1):
		f.write(",")
	i+=1
f.write("}")
f.close()

*/

PROGMEM const uint16_t voctTable [] = {0,32,65,97,129,161,194,226,258,290,323,355,387,419,452,484,516,548,581,613,645,677,710,742,774,806,839,871,903,935,968,1000,1032,1064,1097,1129,1161,1193,1226,1258,1290,1322,1355,1387,1419,1451,1484,1516,1548,1580,1613,1645,1677,1709,1742,1774,1806,1838,1871,1903,1935,1967,2000,2032,2064,2096,2129,2161,2193,2225,2258,2290,2322,2354,2387,2419,2451,2483,2516,2548,2580,2612,2645,2677,2709,2741,2774,2806,2838,2870,2903,2935,2967,2999,3032,3064,3096,3128,3161,3193,3225,3257,3290,3322,3354,3386,3419,3451,3483,3515,3548,3580,3612,3644,3677,3709,3741,3773,3806,3838,3870,3902,3935,3967,3999,4031,4064,4096};


/*
    for (int i=0.0; i<128; i++){
    note[i] = static_cast<unsigned int>round(((i/12.0)*4096.0)/(127.0/12.0)); //  create LUT
}
 */
