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
int speedlength = 1; //The length of the LED chain


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
  for(int n = 0; n < speedlength; n++){
   strip.setPixelColor(n, 0, 0, 255);
   strip.show();
  }
   if(millis()>start_time+1000){
     boardspeed = count*circumference; //board speed im m/s
    if(count>0){
      speedlength = 1;
    }
    else{
      speedlength = boardspeed/10*60;
    }
   }  
}

void countP() 
{ 
  count++;
  Serial.println(count);
}

