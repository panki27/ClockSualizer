/*
MIDI Clock Visualizer by Panki
 */
 
#include <Adafruit_NeoPixel.h>
#define PIN 3 // LED Strip digital in
#define analogPin A1
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
 
volatile unsigned long oldTime=0, debounce=20;
int Bar = 0;
int activeColor = 0;
int counter = 0;
int Chaser1=0, Chaser2=strip.numPixels();
int peak =0, lastPeak = 0, val = 0; 
byte currentCol=0;

byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xfb;
int play_flag = 0;
byte data;

uint32_t Colors[6] = {strip.Color(255, 0, 0), strip.Color(200, 0, 255), //different colors to rotate through
                      strip.Color(255, 0, 0), strip.Color(0, 255, 0),
                      strip.Color(255, 255, 255), strip.Color(0, 255, 0)};
                            
void setup() {
  //  Set hairless MIDI baud rate:
  Serial.begin(19200);
  // Serial.begin(9200);
  // Set Interrupt pin:
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  attachInterrupt(0, resetPhrase, LOW);
  
  strip.begin();
  strip.show(); 
  
}

void loop() {
 if(Serial.available() > 0) {
    data = Serial.read();
    if(data == midi_start) {
      play_flag = 1;
    }
    else if(data == midi_continue) {
      play_flag = 1;
    }
    else if(data == midi_stop) {
      play_flag = 0;
    }
    else if((data == midi_clock) && (play_flag == 1)) {
      Sync();
    }
  } 
 /* colorWipe(strip.Color(0,0,0));
  int sensorValue = analogRead(A1);
  meterA(sensorValue/10);*/
}

void Sync() {
// do something for every MIDI Clock pulse when the sequencer is running

 centerBarRainbow(); 
 counter++;
 
 if(counter == 24){  // 1 beat = 24 ticks
    counter = 0;
    //colorWipe(Colors[activeColor]);
    //currentCol++;
    Bar++; //
  }
  
  if(Bar == 16){  //1 phrase = 16 bars 
    Bar = 0; 
    currentCol +=30;
    /*if(activeColor == 5){
      activeColor = 0;
    } else {
      activeColor++;
    }*/
  }
  /*if(counter == 3){ // this will set the duration of the led on
    colorWipe(strip.Color(0, 0, 0));
  }*/
  
}

void resetPhrase(){ // used to manually sync with phrase
  if((millis() - oldTime) > debounce) { 
    counter = 0; 
    Bar = 0;
    activeColor = 0;
    oldTime = millis();  
  }
}

void AdvanceChaser() {
  colorWipe(strip.Color(0,0,0));
  strip.setPixelColor(Chaser1, 0, 0, 255);
  strip.setPixelColor(Chaser2, 0, 0, 255);

  if(Chaser1 == strip.numPixels()){
    Chaser1 = 0;
  }
  if(Chaser2 == 0){
    Chaser2 = strip.numPixels();
  }

  Chaser1++;
  Chaser2--;
  strip.show();
}

void meterA(int amplitude){
  for(int i = 0; i<amplitude; i++){
    strip.setPixelColor(i, 0, 255, 0);
  }
  strip.show();
}

void centerBarRainbow(){  // DONE: Rotating, contrasting colors (migrated from BeatStrip)
  val = analogRead(A0);
  val-= 350;
  peak = round(val/2);

  if( peak < lastPeak){
    lastPeak = lastPeak -2;
  }
  else {
    lastPeak = peak;
  }

  if(currentCol > 255){
    currentCol = 0;
  } 
  
  for(int i2 = 0; i2<(30-lastPeak); i2++){ //OFF STATE 
    strip.setPixelColor(i2, Wheel(currentCol));
    strip.setPixelColor((60-i2), Wheel(currentCol)); 
  }
  strip.show();
  for(int i = 0; i<lastPeak; i++){ // ON State
    strip.setPixelColor(i+30, Wheel(currentCol-128));
    strip.setPixelColor((30-i), Wheel(currentCol-128));
  }

  strip.show();
}

void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();

}

uint32_t Wheel(byte WheelPos) { // Generate colors from a value of 0-255
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
