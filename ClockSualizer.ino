/*
MIDI Clock Visualizer by Panki
 */
 
#include <Adafruit_NeoPixel.h>
#define PIN 3 // LED Strip digital in
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
 
volatile unsigned long oldTime=0, debounce=20;
int Bar = 0;
int activeColor = 0;
int counter = 0;

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
}

void Sync() {
// do something for every MIDI Clock pulse when the sequencer is running
  
  if(counter == 24){  // 1 beat = 24 ticks
    counter = 0;
    colorWipe(Colors[activeColor]);
    Bar++; //
  }
  
  if(Bar == 16){  //1 phrase = 16 bars 
    Bar = 0;
    if(activeColor == 5){
      activeColor = 0;
    } else {
      activeColor++;
    }
  }
  if(counter == 3){ // this will set the duration of the led on
    colorWipe(strip.Color(0, 0, 0));
  }
  counter++;
}

void resetPhrase(){ // used to manually sync with phrase
  if((millis() - oldTime) > debounce) { 
    counter = 0; 
    Bar = 1;
    activeColor = 0;
    oldTime = millis();  
  }
}


void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();

}
