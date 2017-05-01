#include <Adafruit_NeoPixel.h>
#define PIN 6//The pin the neopixel's are connected to

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

volatile unsigned int count = 0;
unsigned long start_time = 0; //this value is used to check if one second has been exceeded when counting the RPS
float boardspeed = 0; //The speed the board is travelling at
float circumference = 0.071; //Circumference of the wheels, enter in the value for your board, used to determine speed from rpm
float distance = 0.006; //distance between neopixels
float light_delay = 50; //the value to delay shifting the LED's by
int speedlength = 1; //The length of the LED chain
int mode = 0; //this variable is used to determine which mode the KED's should be on.

void setup()
{
    attachInterrupt(digitalPinToInterrupt(2), countP, FALLING); // device signals a low when active
    attachInterrupt(digitalPinToInterrupt(3), Button, RISING); // device signals a high when active
    start_time = millis();
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    Serial.begin(9600);
    digitalWrite(2, HIGH);
}

void loop()
{
    if (mode == 0) {
        Mode1();
    }
    else if (mode == 1) {
        Mode2();
    }
    else {
        Mode3();
    }
}

void countP()
{
    count++;
    Serial.println(count);
}

void Button()
{
    mode = mode++;
    if (mode == 3) {
        mode = 0;
    }
    for(int x = 0; x<mode+1; x++){
      for(int n = 0; n<60; n++){
        strip.setPixelColor(n, 255, 255, 255);
        strip.show();
        delay(100);
        strip.show();
      }
    }
}

void Mode1()
{
    //Blue lights
    for (int n = 0; n < 63; n++) {
        strip.setPixelColor(n, 0, 0, 255);
        strip.setPixelColor(n - 3, 0, 0, 0);
        strip.show();
        if (millis() > start_time + 1000) {
            boardspeed = count * circumference; //board speed im m/s
            if (count > 0) {
                light_delay = distance / boardspeed * 1000;
            }
            else {
                light_delay = 100;
            }
            Serial.println(light_delay);
            count = 0;
            start_time = millis();
        }
        delay(light_delay);
    }
}

void Mode2()
{
    // put your main code here, to run repeatedly:
    if (millis() > start_time + 1000) {
        boardspeed = count * circumference; //board speed im m/s
        if (count > 0) {
            light_delay = distance / boardspeed * 1000;
            rainbowCycle(light_delay);
        }
        else {
            rainbowCycle(1);
        }
        Serial.println(light_delay);
        count = 0;
        start_time = millis();
    }
    delay(light_delay);
}

void Mode3()
{
    // put your main code here, to run repeatedly:
    for (int n = 0; n < speedlength; n++) {
        strip.setPixelColor(n, 0, 0, 255);
        strip.show();
    }
    if (millis() > start_time + 1000) {
        boardspeed = count * circumference; //board speed im m/s
        if (count > 0) {
            speedlength = 1;
        }
        else {
            speedlength = boardspeed / 10 * 60;
        }
    }
}

void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
        for (i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
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

