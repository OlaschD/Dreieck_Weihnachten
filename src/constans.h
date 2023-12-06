// diese Datei legt fest welche Hardware wie genutzt wird
// Projekt : Weihnachtsdreieck

#ifndef _constans_h
#define _constans_h
#include <Arduino.h>
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

// Variablen FastLED
#define DATA_PIN 6
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 152 // 165				// 60 + 60 + 22 + 22 = 164
#define HALF_NUM (NUM_LEDS / 2)
#define PHASEDELAY 30 // orginal 10
#define PHASEDELAY3 0
#define FRAMES_PER_SECOND 240
#define Helligkeit 180
#define BRIGHTNESS Helligkeit
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
CRGB leds[NUM_LEDS];

// Variablen Adafruit Neopixel
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define PIN			6
#define NUMPIXEL	152
		// der wievielte Punkt wird angesteuert
int count = 5;		// der wievielte Punkt wird angesteuert
#define DELAYVAL 500
int Durchlauf = 5;




#endif /* _constans_h */