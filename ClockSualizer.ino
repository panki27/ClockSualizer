/*
MIDI Clock Visualizer by Panki
 */
 
#include <Adafruit_NeoPixel.h>
#define PIN 3
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
 

unsigned long Time;
unsigned long lastTimestamp = 0;
unsigned long currentTimestamp;
int Bar = 0;
int activeColor = 0;

int counter = 0;
byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xfb;
int play_flag = 0;
byte data;

uint32_t Colors[6] = {strip.Color(0, 0, 255), strip.Color(200, 0, 255), //different colors to rotate through
                            strip.Color(255, 0, 0), strip.Color(0, 255, 0),
                            strip.Color(255, 255, 255), strip.Color(0, 255, 0)};
                            
void setup() {
  //  Set MIDI baud rate:
  Serial.begin(19200);
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
}

void Sync() {
// do something for every MIDI Clock pulse when the sequencer is running
  if(Bar == 15){
    Bar = 0;
    if(activeColor = 5){
      activeColor = 0;
    } else {
      activeColor++;
    }
  }
  if(counter == 23){
    counter = 0;
    colorWipe(Colors[activeColor]);
    Bar++;
   
   // digitalWrite(LEDPIN, HIGH);
  }
  if(counter == 3){ // this will set the duration of the led on
    //digitalWrite(LEDPIN, LOW);
    colorWipe(strip.Color(0, 0, 0));
  }
  counter++;
  Bar++;
}

void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();

}
