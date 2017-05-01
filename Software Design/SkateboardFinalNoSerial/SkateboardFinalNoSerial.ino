#include <PinChangeInterrupt.h>
#define PCINT_PIN A0
#define PCINT_MODE RISING
#define PCINT_FUNCTION Button

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(4);

#include <Adafruit_NeoPixel.h>
#define PIN 11//The pin the neopixel's are connected to

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(27, PIN, NEO_GRB + NEO_KHZ800);

volatile unsigned int count = 0;
unsigned long start_time = 0; //this value is used to check if one second has been exceeded when counting the RPS
float boardspeed = 0; //The speed the board is travelling at
float circumference = 0.071; //Circumference of the wheels, enter in the value for your board, used to determine speed from rpm
float distance = 0.006; //distance between neopixels
float light_delay = 50; //the value to delay shifting the LED's by
int speedlength = 1; //The length of the LED chain
int mode = 0; //this variable is used to determine which mode the KED's should be on.
int lastButtonPress = 0;

void setup()
{
    DDRD = 0x00; // Set Port D as input
    PORTD = 0xFF; // Turn on internal pull-up resistors
    //attachInterrupt(digitalPinToInterrupt(2), countP, FALLING); // device signals a low when active
    attachInterrupt(digitalPinToInterrupt(3), Button, RISING); // device signals a high when active
    pinMode(PCINT_PIN, INPUT);
    attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(PCINT_PIN), Button, RISING);
    start_time = millis();
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    digitalWrite(2, HIGH);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32);
    display.clearDisplay();
    testdrawtriangle();
  delay(2000);
  display.clearDisplay();
   
  testfilltriangle();
  delay(2000);
  

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(16,12);
  display.print("WILL");
  display.setCursor(96,12);
  display.print("RIGBY");
  display.display();
  delay(2000);
}

void loop()
{
    if (mode == 0) {
       display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0,16);
      display.print("RAINBOW");
      display.display();
        Mode1();
    }
    else if (mode == 1) {
        Mode2();
    }
    else if (mode == 2) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0,16);
      display.print("SAFETY LITE");
      display.display();
        Mode3();
    }
}


void Button()
{
  if(millis() > lastButtonPress+300){
    lastButtonPress = millis();
    mode = mode + 1;
    if (mode == 3) {
        mode = 0;
       
    }
    for(int x = 0; x<mode+1; x++){
      for(int n = 0; n<27; n++){
        strip.setPixelColor(n, 0, 255, 0);
        strip.show();
        delay(10);
        strip.show();
      }
    }
   
    
  }
}

void Mode2()
{
    int red = 35;
    int green = 135;
    int blue = 217;
    //Blue lights
    for (int n = 0; n < 30; n++) {
        strip.setPixelColor(n, red, green, blue);
        strip.setPixelColor(n+10, red, green, blue);
        strip.setPixelColor(n+20, red, green, blue);
        strip.setPixelColor(n-10, red, green, blue);
        strip.setPixelColor(n-20, red, green, blue);
        strip.setPixelColor(n - 3, 0, 0, 0);
        strip.setPixelColor(n+17, 0, 0, 0);
        strip.setPixelColor(n+27, 0, 0, 0);
        strip.setPixelColor(n-13, 0, 0, 0);
        strip.setPixelColor(n-23, 0, 0, 0);
        strip.show();
        if (millis() > start_time + 1000) {
            count = (PIND);
            boardspeed = count*2 * circumference; //board speed im m/s
            if (count > 0) {
                light_delay = distance / boardspeed * 1000;
            }
            else {
                light_delay = 100;
            }
            
            display.clearDisplay();
            display.stopscroll();
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(4,16);
            display.print(boardspeed);display.print("m/s");
            display.display();
            
            count = 0;
            start_time = millis();
        }
        delay(light_delay);
    }
}

void Mode1()
{
  rainbowCycle(1);
}

void Mode3(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,10);
  display.print("Bar Graph");
  for( int i=1; i<= strip.numPixels(); i++){  // Attempt to reverse direction of rainbow from 17 to 0
      strip.setPixelColor(strip.numPixels() - i, 0, 0, 0);
    }
  strip.show();
  for (int i = 0; i<256; i = i+1){
    for (int n = 0; n<27; n = n+3){
      strip.setPixelColor(n, Wheel(i & 255));
      }
    strip.show();
    delay(20);
  }
  
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    
    //for(i=0; i< strip.numPixels(); i++) {     // Original code
    for(i=1; i<= strip.numPixels(); i++){  // Attempt to reverse direction of rainbow from 17 to 0
      strip.setPixelColor(strip.numPixels() - i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    if (WheelPos < 85) {
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else {
        WheelPos -= 170;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}

void testdrawtriangle(void) {
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
    delay(1);
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
    delay(1);
  }
}
