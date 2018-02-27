#include <elapsedMillis.h>
#include <Adafruit_DotStar.h>
#include <SPI.h> 
#include <Bounce2.h>


#define NUMPIXELS 30 // Number of LEDs in strip
#define DATAPIN    11
#define CLOCKPIN   13
#define MASTODON 5
#define MAMMOTH 6
#define SENSOR  A2

#define PULSEMAX 500
#define SESSION 30000

Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

Bounce debouncer = Bounce();
elapsedMillis pulse = 0;
elapsedMillis session = SESSION;

bool busy = false;
int outState = LOW;

void setup() {
  // put your setup code here, to run once:
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

  pinMode(SENSOR,INPUT_PULLUP);
  debouncer.attach(SENSOR);
  debouncer.interval(300);

  pinMode(MASTODON,OUTPUT);
  digitalWrite(MASTODON,outState);
  pinMode(MAMMOTH,OUTPUT);
  digitalWrite(MAMMOTH,outState);
  
}

  // Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

void loop() {
  debouncer.update();
  if(!busy){
    if(debouncer.fell()){
      outState = HIGH;
      digitalWrite(MASTODON,outState);
      digitalWrite(MAMMOTH,outState);
      pulse = 0;
      session = 0;
      busy = true;
    }
  }
  else{
    if ((pulse > PULSEMAX)&&outState){ //turn off the outputs after the desired pulse length
      outState = LOW;
      digitalWrite(MASTODON,outState);
      digitalWrite(MAMMOTH,outState);
    }
    if (session > SESSION){
      busy = false;
    }
  strip.setPixelColor(head, color); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(20);                        // Pause 20 milliseconds (~50 FPS)

  if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
    head = 0;                       //  Yes, reset head index to start
    if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
      color = 0xFF0000;             //   Yes, reset to red
  }
  if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
}
  
  }
  

