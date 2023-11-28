// diese Datei legt fest welche Hardware wie genutzt wird
// Projekt : Weihnachtsdreieck

#ifndef _constans_h
#define _constans_h
#include <Arduino.h>
#include <FastLED.h>
#include <math.h>

// Variablen
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



#endif /* _constans_h */