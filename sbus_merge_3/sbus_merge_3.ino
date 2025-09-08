/*This sketch merge signsls from two input SBUS lines into one output. Board used is Arduino Mega2560
 * At boot it sends an output packet with default values, about 500 for second line related channels and 302 for channel #2 (count from zero) for line #1 channels,
 * so connection to servos and transmitting of output signals can be checked without signals on input lines
 * Then, after 3 seconds controller starts to listen to input lines. When a valid packet comes on input 1 - an output packet is sent, channels 0...7 of it 
 * repeating data in input #1 packet. Channels 8...15 contain data from the last received packet on input #2 or, if nothing was received - default values.
 * pins used on Arduino Mega 2560: input #1 - 19, input #2 - 17, output - 14, LED1 - 31, LED2 - 33.
 * Sketch uses library  Bold Flight Systems SBUS by Brian Taylor
 * by now library files are placed into sketch's folder so should be no need to install in Arduino IDE
 * debuggiing signals sent by usb interface (115200 bod): at boot: version of software, on receiving a valid packet on line #1: "rx1", on receiving a valid packet on line #2: "rx2"
 * LEDs flash on boot to check it's cercuits
 */
#include "SBUS.h"
//#include <SBUS.h>


SBUS sbus_input_1(Serial1);
SBUS sbus_input_2(Serial2);
SBUS sbus_output(Serial3);

uint16_t inputChannels_1[16];
uint16_t inputChannels_2[16];
uint16_t outputChannels[16];

int const ledPin1 = 31;
int const ledPin2 = 33;

bool failSafe;
bool lostFrame;

bool statusRX1 = false;
bool statusRX2 = false;

bool ledState1 = false; //false > off, true > on
bool ledState2 = false;
unsigned long const ledTimout = 200; // milliseconds
unsigned long timeOfLastPaket_1 = 0;
unsigned long timeOfLastPaket_2 = 0;

void setup() {
  // begin the SBUS communication
  sbus_input_1.begin();
  sbus_input_2.begin();
  sbus_output.begin();

Serial.begin(115200);
Serial.println("sbus_merge_3");

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);

//default values for ch1
inputChannels_1[2] = 302;

//default values for ch2
inputChannels_2[0] = 500;
inputChannels_2[1] = 501;
inputChannels_2[2] = 502;
inputChannels_2[3] = 503;
inputChannels_2[4] = 504;
inputChannels_2[5] = 505;
inputChannels_2[6] = 506;
inputChannels_2[7] = 507;
delay(1000);

  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);

sbus_output.write(&outputChannels[0]);
delay(3000);

}

void loop() {
  // look for a good SBUS packet from the receiver 2
  if(sbus_input_2.read(&inputChannels_2[0], &failSafe, &lostFrame)){
    bool statusRX1 = true;
  //  Serial.print("rx: inputChannels_2[2]= ");
   // Serial.println(inputChannels_2[2]); 
         timeOfLastPaket_2 = millis();
       Serial.println("rx2"); 
  }

  // look for a good SBUS packet from the receiver 1
  if(sbus_input_1.read(&inputChannels_1[0], &failSafe, &lostFrame)){

// merging buses
for (byte i = 0; i < 8; i = i + 1) {
  outputChannels[i] = inputChannels_1[i];
}
for (byte i = 0; i < 8; i = i + 1) {
  outputChannels[i + 8] = inputChannels_2[i];
}

sbus_output.write(&outputChannels[0]);
  timeOfLastPaket_1 = millis();
 Serial.println("rx1"); 


/*
   Serial.println("----------------------------------");
 Serial.print("rx: inputChannels_1[0]= ");
 Serial.println(inputChannels_1[0]);  
 Serial.print("rx: inputChannels_1[1]= ");
 Serial.println(inputChannels_1[1]);   
 Serial.print("rx: inputChannels_1[2]= ");
 Serial.println(inputChannels_1[2]);
  Serial.print("rx: inputChannels_1[3]= ");
 Serial.println(inputChannels_1[3]);
  Serial.print("rx: inputChannels_1[4]= ");
 Serial.println(inputChannels_1[4]);
 Serial.print("rx: inputChannels_1[5]= ");
 Serial.println(inputChannels_1[5]);
  Serial.print("rx: inputChannels_1[6]= ");
 Serial.println(inputChannels_1[6]);
  Serial.print("rx: inputChannels_1[7]= ");
 Serial.println(inputChannels_1[7]);


 Serial.print("rx: inputChannels_2[2]= ");
 Serial.println(inputChannels_2[2]);

 Serial.print("rx: outputChannels[0]= ");
 Serial.println(outputChannels[0]);  
 Serial.print("rx: outputChannels[1]= ");
 Serial.println(outputChannels[1]);   
 Serial.print("rx: outputChannels[2]= ");
 Serial.println(outputChannels[2]);
  Serial.print("rx: outputChannels[3]= ");
 Serial.println(outputChannels[3]);
  Serial.print("rx: outputChannels[4]= ");
 Serial.println(outputChannels[4]);
 Serial.print("rx: outputChannels[5]= ");
 Serial.println(outputChannels[5]);
  Serial.print("rx: outputChannels[6]= ");
 Serial.println(outputChannels[6]);
  Serial.print("rx: outputChannels[7]= ");
 Serial.println(outputChannels[7]);
  Serial.print("rx: outputChannels[8]= ");
 Serial.println(outputChannels[8]);
  Serial.print("rx: outputChannels[9]= ");
 Serial.println(outputChannels[9]);
  Serial.print("rx: outputChannels[10]= ");
 Serial.println(outputChannels[10]);
  Serial.print("rx: outputChannels[11]= ");
 Serial.println(outputChannels[11]);
  Serial.print("rx: outputChannels[12]= ");
 Serial.println(outputChannels[12]);
  Serial.print("rx: outputChannels[13]= ");
 Serial.println(outputChannels[13]);
  Serial.print("rx: outputChannels[14]= ");
 Serial.println(outputChannels[14]);
  Serial.print("rx: outputChannels[15]= ");
 Serial.println(outputChannels[15]); 
 */
  }
 
  if (millis() - timeOfLastPaket_1 < ledTimout) {    //receiving is OK - line 1
  digitalWrite(ledPin1, HIGH);
  }
  else {  digitalWrite(ledPin1, LOW);
  }

    if (millis() - timeOfLastPaket_2 < ledTimout) {    //receiving is OK - line 2
  digitalWrite(ledPin2, HIGH);
  }
  else {  digitalWrite(ledPin2, LOW);
  }
}
