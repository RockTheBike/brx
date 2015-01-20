#define VERSION "BRX lighting"

#include "Arduino.h"
#include "rtblight.h"
#include <Adafruit_NeoPixel.h>

unsigned long time = 0;
unsigned long tickcount = 0;
#define REPORT_INTERVAL 1000
unsigned long next_report_time = 0;

class Mode {
};

class BlinkMode : Mode {
	public:
	unsigned int blink_on_length;
	unsigned int blink_off_length;
	Color color;
	private:
	bool blink_on;
	millitime_t when_to_blink;
	public:
	virtual void loop( millitime_t time );
};

class LowMode : BlinkMode {
};

class HighMode : BlinkMode {
};

#define PIN_PIXEL 13
#define NUM_PIXELS 180
Adafruit_NeoPixel strip( NUM_PIXELS, PIN_PIXEL, NEO_GRB|NEO_KHZ800 );


void setup() {
	Serial.begin(57600);
	Serial.println(VERSION);
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

void draw_strip( int spot ) {
	for( int i=0; i<NUM_PIXELS; i++ )
		if( i==spot )
			strip.setPixelColor( i, 255, 255, 0 );
		else
			strip.setPixelColor( i, 0, 0, 0 );
	strip.show();
}

void draw_bits( unsigned long n ) {
	static const uint32_t dark = Adafruit_NeoPixel::Color(0,0,0);
	static const uint32_t bright = Adafruit_NeoPixel::Color(255,255,255);
	for( int i=0; i<sizeof(n)*8; i++ )
		strip.setPixelColor( i, n>>i & 1 ? bright : dark );
	strip.show();
}

void fast_chase( int n ) {
	strip.setPixelColor( (n+1)%NUM_PIXELS, 255, 255, 0 );
	strip.setPixelColor( n, 0, 0, 0 );
	strip.show();
}

const uint32_t SPRITE[] = {
	Adafruit_NeoPixel::Color(0,0,0),
	Adafruit_NeoPixel::Color(255,0,0),
	Adafruit_NeoPixel::Color(255,255,0),
	Adafruit_NeoPixel::Color(0,255,0),
	Adafruit_NeoPixel::Color(0,255,255),
	Adafruit_NeoPixel::Color(0,0,255),
	Adafruit_NeoPixel::Color(255,0,255) };
void sprite( int n ) {
	for( int i=0; i<sizeof(SPRITE)/sizeof(*SPRITE); i++ )
		strip.setPixelColor( n+i, SPRITE[i] );
	strip.show();
}

void report() {
	Serial.print("Still alive at ");
	Serial.print(tickcount);
	Serial.println("...");
}

void loop() {
	time = millis();
	//draw_strip( tickcount%NUM_PIXELS );
	//draw_bits( tickcount );
	//fast_chase( tickcount%NUM_PIXELS );
	sprite( tickcount%NUM_PIXELS );
	if( time > next_report_time ) {
		report();
		next_report_time = time + REPORT_INTERVAL;
	}
	tickcount++;
}
