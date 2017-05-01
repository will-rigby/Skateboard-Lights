#include <Adafruit_NeoPixel.h>
 #define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

volatile unsigned int count = 0;
unsigned long start_time = 0;
float boardspeed = 0;
float circumference = 0.071;
float distance = 0.006;
float light_delay = 50;


void setup() {
  // put your setup code here, to run once:
  attachInterrupt(digitalPinToInterrupt(2), countP, FALLING);  // device signals a low when active
  start_time = millis();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
  digitalWrite(2, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
   if(millis()>start_time+1000){
     boardspeed = count*circumference; //board speed im m/s
    if(count>0){
      light_delay = distance/boardspeed*1000;
      rainbowCycle(light_delay);
    }
    else{
      rainbowCycle(1);
    }
    Serial.println(light_delay);
    count = 0;
    start_time = millis();
  }
   delay(light_delay);
}

void countP() 
{ 
  count++;
  Serial.println(count);
}

 // Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
 
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
