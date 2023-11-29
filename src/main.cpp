/*
 Beschreibung   : Weihnachtsdreieck
				: Umbau mit Mittelsteg und nur noch 152 LED
 Arduino     	: UNO
 Modul ID    	: 11101 mit Gehäuse
 Datum       	: 22.11.2023 (umprogrammiert)  - 15.12.2019
 Version:       : V3.0 (Änderung des Programms)
 Schaltung in	: keine
 Hardwareinfo	: DI -> 6 (Anschluß Pin 6, Steuerung, +5V , GND)
 ToDo			: Taster zum weiterschalten der Programme, Poti für die Laufgeschwindigkeit
 Status      	: OK
 Einsatz	    : mein Zimmer
 Prog-Software  : PlatformIO auf Big5
 Vorlage        : Weihnachtsdreieck von 2022
 Hinweis		: aufteilung der Funktionen in eigene Dateien

*/

// ****************** Biblioteken einbindenn **********************************
#include "Arduino.h"
#include <FastLED.h>
#include <constans.h>
// #include <Streaming.h>
// #define DEBUG // Auskommentieren wenn Serial.print nicht ausgeführt werden soll

FASTLED_USING_NAMESPACE
// #define FASTLED_ALLOW_INTERRUPTS 1
// #define FASTLED_INTERRUPT_RETRY_COUNT 1

// ******************** Variablen definieren **********************************
// Variablen
// #define DATA_PIN 6
// #define LED_TYPE WS2811
// #define COLOR_ORDER GRB
// #define NUM_LEDS 152 // 165				// 60 + 60 + 22 + 22 = 164
// #define HALF_NUM (NUM_LEDS / 2)
// #define PHASEDELAY 30 // orginal 10
// #define PHASEDELAY3 0
// #define FRAMES_PER_SECOND 240
// #define Helligkeit 180
// #define BRIGHTNESS Helligkeit
// #define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
// CRGB leds[NUM_LEDS];

#define PHASEDELAY2 80 // Millisekunden zwischen den Phasen
// #define REDVAL    128  // 0..255 (red value of LED)
// #define GREENVAL    0  // 0..255 (green value of LED)
// #define BLUEVAL     0  // 0..255 (blue value of LED)
String Versionen("V3.0");
int Anz = 5;
int Speed = 3;				 // A= = Low 0 ist am schnellsten und 1023 = 5V
void rainbow();				 // Regenbogenfarben laufend
void sinelon();				 // zweiPunkte in je einer Hälfte hin und her laufend
void bpm();					 // Kamelion 7 Farbe hin und herlaufend
void juggle();				 // mehrere Punkte mit Schweif in einander laufend
void Rain_schwinge();		 // Farben schwingen hin und her
void larsonScanner();		 // mehere Punkte laufen wie Nightrider hin und her
void Farbige_sinus_wellen(); // sehr bunt und ruhig
void larsonScanner2();		 // langsam mit Farbwechsel
void Wetter_sim();			 // heftige Farbwechsel wie ein Gewitter
void ledsHeartbeat(uint8_t hue, uint8_t saturation, uint8_t brightness, uint32_t duration);
void ledsHeartbeatShiftColor(uint8_t saturation, uint8_t brightness, uint32_t duration);
void ledsHeartbeatRandomColor(uint8_t saturation, uint8_t brightness, uint32_t duration);
void Herzschlag();			   // von der Mitte nach aussen pulsierend
void Seiten_Farbe();		   // Ab der Hälfte zwei verschiedene Farben umlaufen
void Band_mitte_nach_aussen(); // Band läuft von mitte nach aussen in wechselnen Farben
void nextPattern();
void Test();
void Regenbogen();
void die_zente_LED();
void Ver();

void setup()
{
	Serial.begin(9600);
	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.show();
    Ver();
} //************************* Ende setup **************************************
// Liste der durchzuführenden Muster. Jede ist als separate Funktion definiert.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {	Regenbogen, 
								larsonScanner, 
								Rain_schwinge, 
								larsonScanner2, 
								Test,
								Band_mitte_nach_aussen, 
								Seiten_Farbe, 
								Farbige_sinus_wellen, 
								juggle, 
								bpm, 
								sinelon, 
								rainbow};
// SimplePatternList gPatterns = {Rain_schwinge, Herzschlag, larsonScanner2, Test, Band_mitte_nach_aussen, Seiten_Farbe, Farbige_sinus_wellen, larsonScanner, juggle, bpm, sinelon, Wetter_sim, rainbow };

uint8_t gCurrentPatternNumber = 0; // Indexnummer des aktuell ausgewählten Musters
uint8_t gHue = 0;				   // rotierende "Grundfarbe", die von vielen Mustern verwendet wird
// Rufen Sie die aktuelle Musterfunktion einmal auf und aktualisieren Sie das 'leds'-Array
uint8_t thisbeat = 13;
uint8_t thatbeat = 28;
uint8_t thisfade = 2;
uint8_t thissat = 255; // Die Sättigung, wobei 255 = brillante Farben sind.
uint8_t thisbri = 255;
uint8_t bpm_Speed = 8;	   // Geschwindigkeit vom Punkt  (org. 5)
uint8_t fade_amount = 1;   // "Länge vom Schweif"
uint8_t saturation = 0xFF; // Sättigung
uint8_t brightness = Helligkeit;
uint8_t duration = 0x78; // E6=230s Dauer 78 = 120sec
uint8_t Patternr_alt;
uint8_t Patternr_neu;


void loop()
{
	Patternr_neu = gCurrentPatternNumber;
	FastLED.setBrightness(Helligkeit);
	gPatterns[gCurrentPatternNumber]();		 // nächstes P abspielenrogramm
	FastLED.show();							 // Senden Sie das LED-Array an den eigentlichen LED-Streifen
	FastLED.delay(1000 / FRAMES_PER_SECOND); // Fügen Sie eine Verzögerung ein, um die Bildfrequenz gering zu halten
	EVERY_N_MILLISECONDS(20) { gHue++; }	 // ziehe langsam die "Grundfarbe" durch den Regenbogen
    if ( (Patternr_neu == Patternr_alt)) 
	{
		Serial.print("ops");
	}
	else
	{
		Serial.print("Nächstes Programm Nr.: ");
		Serial.println(gCurrentPatternNumber);
		Patternr_neu = gCurrentPatternNumber;
	}

	
	EVERY_N_SECONDS(60) { nextPattern(); }	 // orginal 10       // Muster regelmäßig ändern
	Patternr_alt = gCurrentPatternNumber;
} //************************* Ende Loop ***************************************
//************ Unterprogramme *************************************************

void Ver() {
// #ifdef DEBUG
  Serial.print("Version: ");
  Serial.println(Versionen);
  Serial.print("Name: ");
  Serial.println("Weihnachtsdreieck");
  Serial.print("Datum: ");
  Serial.println("22.11.2023  ");
  Serial.println("Programmiert mit PlatformIO auf Big5");
  Serial.print("-----------");
  Serial.print("");
  Serial.println();
// #endif
}

void nextPattern() // Füge eins zur aktuellen Pattern-Nummer hinzu bis zum Ende
{
	gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void Test()
{
	#ifdef DEBUG
    	Serial.println("Test");
	#endif
	// Hier sind alle LED's an und es laufen alle Farben nach einander zufällig ab
	uint8_t hue = random(0x00, 0xFF);
	for (int pos2 = NUM_LEDS; pos2 > 0; pos2--)
	{
		leds[pos2] = CHSV(hue, 250, 250);
		// leds[NUM_LEDS, NUM_LEDS -2] = CRGB::Black;
		//	leds(NUM_LEDS -1, 0);
		//    	leds[pos2-2] = CRGB::Black;
		//    	leds[NUM_LEDS - pos2] = CHSV(hue, 250, 250);
		//    	leds[pos2+2] = CRGB::Black;
		FastLED.delay(40);
		//    	leds[pos2+2] = CRGB::Black;
	}

	//	for( int pos = 0; pos <= HALF_NUM; pos++){
	//      leds[pos] = CRGB::Black;
	//      leds[NUM_LEDS - pos] = CRGB::Black;
	//      FastLED.delay(40);
	//	}
}

void Band_mitte_nach_aussen()
{
	#ifdef DEBUG
    	Serial.println("Band_mitte_nach_aussen");
	#endif
	uint8_t hue = random(0x00, 0xFF);
	for (int pos2 = HALF_NUM; pos2 >= 0; pos2--)
	{
		leds[pos2] = CHSV(hue, 250, 250);
		leds[NUM_LEDS - pos2] = CHSV(hue, 250, 250);
		FastLED.delay(40);
	}

	for (int pos = 0; pos <= HALF_NUM; pos++)
	{
		leds[pos] = CRGB::Black;
		leds[NUM_LEDS - pos] = CRGB::Black;
		FastLED.delay(40);
	}
}

void Seiten_Farbe()
{
	#ifdef DEBUG
	  Serial.println("SeitenFarben");
	#endif
	for (int pos = 0; pos <= HALF_NUM; pos++)
	{
		leds[pos] = CHSV(gHue, 255, 192);
		leds[NUM_LEDS - pos] = CHSV(gHue * 3, 200, 192);
		FastLED.delay(40);
	}
	FastLED.show();
}

void ledsHeartbeat(uint8_t hue, uint8_t saturation, uint8_t brightness, uint32_t duration)
{
		#ifdef DEBUG
		   Serial.println("ledsHeartbeat";
		#endif
	const uint8_t patternHeartbeat[56] =
		{
			0, 0, 0, 0, 1, 2, 3, 4,
			5, 6, 7, 8, 9, 10, 11, 12,
			13, 14, 15, 15, 14, 13, 12, 11,
			10, 9, 8, 7, 8, 9, 10, 11,
			12, 13, 14, 15, 15, 14, 13, 12,
			11, 10, 9, 8, 7, 6, 5, 4,
			3, 2, 1, 0, 0, 0, 0, 0};
	bool oddLeds = NUM_LEDS % 2;
	uint8_t ledFactor[NUM_LEDS];

	for (int16_t led = 0; led <= NUM_LEDS / 2; led++)
	{
		ledFactor[led] = map(led + 1, 1, (NUM_LEDS / 2), 1, 17);
		if (!oddLeds)
		{
			ledFactor[NUM_LEDS / 2] = 17; /* map befehl umschreiben!!!*/
		}
		ledFactor[NUM_LEDS - 1 - led] = map(led + 1, 1, (NUM_LEDS / 2), 1, 17);
	}

	static uint32_t lastMillis;
	static uint8_t ctPattern;
	if (millis() - lastMillis >= duration)
	{
		lastMillis = millis();
		ctPattern++;
		if (ctPattern >= 56)
			ctPattern = 0;
		for (int led = 0; led < NUM_LEDS; led++)
		{
			leds[led] = CHSV(hue, saturation, brightness * (ledFactor[led] * patternHeartbeat[ctPattern]) / 255);
		}
		FastLED.show();
	}
}

void ledsHeartbeatShiftColor(uint8_t saturation, uint8_t brightness, uint32_t duration)
{
	const uint8_t patternHeartbeat[56] =
		{
			0, 0, 0, 0, 1, 2, 3, 4,
			5, 6, 7, 8, 9, 10, 11, 12,
			13, 14, 15, 15, 14, 13, 12, 11,
			10, 9, 8, 7, 8, 9, 10, 11,
			12, 13, 14, 15, 15, 14, 13, 12,
			11, 10, 9, 8, 7, 6, 5, 4,
			3, 2, 1, 0, 0, 0, 0, 0};
	bool oddLeds = NUM_LEDS % 2;
	uint8_t ledFactor[NUM_LEDS];
	static uint8_t hue;

	for (int led = 0; led <= NUM_LEDS / 2; led++)
	{
		ledFactor[led] = map(led + 1, 1, (NUM_LEDS / 2), 1, 17);
		if (!oddLeds)
		{
			ledFactor[NUM_LEDS / 2] = 17; /* map befehl umschreiben!!!*/
		}
		ledFactor[NUM_LEDS - 1 - led] = map(led + 1, 1, (NUM_LEDS / 2), 1, 17);
	}

	static uint32_t lastMillis;
	static uint8_t ctPattern;
	if (millis() - lastMillis >= duration)
	{
		lastMillis = millis();
		ctPattern++;
		if (ctPattern >= 56)
		{
			ctPattern = 0;
			hue = millis() % 255;
		}
		if (patternHeartbeat[ctPattern] == 0)
		{
			if (hue >= 255)
				hue = 0;
			else
				hue++;
		}
		for (int led = 0; led < NUM_LEDS; led++)
		{
			leds[led] = CHSV(hue, saturation, brightness * (ledFactor[led] * patternHeartbeat[ctPattern]) / 255);
		}
		FastLED.show();
	}
}

void ledsHeartbeatRandomColor(uint8_t saturation, uint8_t brightness, uint32_t duration)
{
	const uint8_t patternHeartbeat[56] =
		{
			0, 0, 0, 0, 1, 2, 3, 4,
			5, 6, 7, 8, 9, 10, 11, 12,
			13, 14, 15, 15, 14, 13, 12, 11,
			10, 9, 8, 7, 8, 9, 10, 11,
			12, 13, 14, 15, 15, 14, 13, 12,
			11, 10, 9, 8, 7, 6, 5, 4,
			3, 2, 1, 0, 0, 0, 0, 0};
	bool oddLeds = NUM_LEDS % 2;
	uint8_t ledFactor[NUM_LEDS];
	static uint8_t hue;

	for (int led = 0; led <= NUM_LEDS / 2; led++)
	{
		ledFactor[led] = map(led + 1, 1, (NUM_LEDS / 2), 1, 17);
		if (!oddLeds)
		{
			ledFactor[NUM_LEDS / 2] = 17; /* map befehl umschreiben!!!*/
		}
		ledFactor[NUM_LEDS - 1 - led] = map(led + 1, 1, (NUM_LEDS / 2), 1, 17);
	}

	static uint32_t lastMillis;
	static uint8_t ctPattern;
	if (millis() - lastMillis >= duration)
	{
		lastMillis = millis();
		ctPattern++;
		if (ctPattern >= 56)
		{
			ctPattern = 0;
			hue = millis() % 255;
		}
		for (int led = 0; led < NUM_LEDS; led++)
		{
			leds[led] = CHSV(hue, saturation, brightness * (ledFactor[led] * patternHeartbeat[ctPattern]) / 255);
		}
		FastLED.show();
	}
}

void Herzschlag()
{
	ledsHeartbeat(gHue, saturation, brightness, duration);	   // Farbe, Sättigung, Helligkeit, Dauer
	ledsHeartbeatShiftColor(saturation, brightness, duration); // Sättigung, Helligkeit, Dauer
	ledsHeartbeatRandomColor(saturation, brightness, duration);
}

void Wetter_sim()
{
	uint8_t hue = random(0x00, 0xFF);
	uint8_t saturation = 0xFF;
	// uint8_t brightness = 0xFF; // 0 -15
	uint8_t interval = random(0x10, 0x50);
	uint8_t patternHeartbeat[0x30] = {
		0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, // 0x10
		8, 8, 9, 9, 8, 7, 6, 5, 4, 5, 6, 7, 8, 9, 9, 8, // 0x20
		8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, // 0x30
	};
	uint8_t ledFactor[NUM_LEDS];

	static bool init;
	if (!init)
	{
		for (int led = 0; led < NUM_LEDS / 2; led++)
		{
			ledFactor[led] = led + 2;
			ledFactor[led] = map(led, 0, NUM_LEDS / 2, 0, 26 /*28 + (brightness - 15)*/) + 2;
			ledFactor[(NUM_LEDS - 1) - led] = map(led, 0, NUM_LEDS / 2, 0, 26 /*28 + (brightness - 15)*/) + 2;
			ledFactor[(NUM_LEDS - 1) - led] = led + 1;
		}
		init = false;
	}

	static uint32_t lastMillis;
	static uint8_t ctPattern;
	if (millis() - lastMillis >= interval)
	{
		ctPattern++;
		if (ctPattern >= 0x30)
		{
			ctPattern = random(100, 255);
		}
		for (int led = 0; led < NUM_LEDS; led++)
		{
			leds[led] = CHSV(hue, saturation, ledFactor[led] * patternHeartbeat[ctPattern]);
		}
		FastLED.show();
		lastMillis = millis();
	}
}

void larsonScanner2()
{
	#ifdef DEBUG
		Serial.println("larsonScanner2");
	#endif
	byte ledcount = NUM_LEDS;
	byte sat = 255;
	byte hue = (millis() / 100) % 255;
	static unsigned long previousMillis = millis();
	static int i; // Schrittzaehler
	static int led = 0;
	static boolean reverse = false;
	byte dim;

	if (i > 32)
	{
		i = 0;

		if (reverse)
			led--;
		else
			led++;

		// Anfang erreicht
		if (led < 0)
		{
			led = 0;
			reverse = !reverse;
		}

		// Ende erreicht
		if (led >= ledcount)
		{
			led = ledcount - 1;
			reverse = !reverse;
		}
	}

	if (millis() - previousMillis > PHASEDELAY3)
	{
		previousMillis = millis();
		memset(leds, 0, ledcount * 3); // LED Array zuruecksetzen
		dim = i * 4;
		i++;

		// Aktuelle Led-2
		if (led - 2 >= 0 && !reverse)
			leds[led - 2] = CHSV(hue, sat, 128 - dim); // FadeDown
		else if (led - 2 >= 0 && reverse)
			leds[led - 2] = CHSV(hue, sat, dim); // FadeUp

		// Aktuelle Led-1
		if (led - 1 >= 0 && !reverse)
			leds[led - 1] = CHSV(hue, sat, 255 - dim); // FadeDown
		else if (led - 1 >= 0 && reverse)
			leds[led - 1] = CHSV(hue, sat, 127 + dim); // FadeUp

		// Aktuelle Led
		leds[led] = CHSV(hue, sat, 255);

		// Aktuelle Led+1
		if (led + 1 < ledcount && !reverse)
			leds[led + 1] = CHSV(hue, sat, 127 + dim); // FadeUp
		else if (led + 1 < ledcount && reverse)
			leds[led + 1] = CHSV(hue, sat, 255 - dim); // FadeDown

		// Aktuelle Led+2
		if (led + 2 < ledcount && !reverse)
			leds[led + 2] = CHSV(hue, sat, dim); // FadeUp
		else if (led + 2 < ledcount && reverse)
			leds[led + 2] = CHSV(hue, sat, 128 - dim); // FadeDown

		FastLED.show();
	}
}

void larsonScanner()
{
	#ifdef DEBUG
		Serial.println("larsonScanner");
	#endif 
	int led1 = 0;
	static boolean reverse = false;
	int i, ledcount = NUM_LEDS;
	byte hue;
	byte r = gHue;
	byte g = gHue;
	byte b = 255;

	static unsigned long time = millis();
	if (millis() - time > PHASEDELAY)
	{
		hue = (millis() / 100) % 255;
		time = millis();
		// larsonScanner(NUM_LEDS,hue ,255 , 192);
	}
	for (i = 0; i < ledcount; i++)
		leds[i] = CRGB(0, 0, 0); // clear led array
	if (reverse)
		led1--;
	else
		led1++;
	if (led1 < 0)
	{
		led1 = 0;
		reverse = !reverse;
	}
	if (led1 >= ledcount)
	{
		led1 = ledcount - 1;
		reverse = !reverse;
	}
	// volle Helligkeit für das mittlere Pixel
	leds[led1] = CHSV(hue, g, b);
	// Reduzieren Sie die Helligkeit für benachbarte Pixel
	if (led1 + 1 < ledcount)
		leds[led1 + 1] = CRGB(r / 2, g / 2, b / 2);
	if (led1 - 1 >= 0)
		leds[led1 - 1] = CRGB(r / 2, g / 2, b / 2);
	// Reduzieren Sie die Helligkeit für zweite Nachbarpixel noch mehr
	if (led1 + 4 < ledcount)
		leds[led1 + 2] = CRGB(r / 4, g / 4, b / 4);
	if (led1 - 4 >= 0)
		leds[led1 - 2] = CRGB(r / 4, g / 4, b / 4);
	FastLED.show();
}

void Farbige_sinus_wellen()
{
	#ifdef DEBUG
		Serial.println("Farbige Sinus_wellen");
	#endif
	// fadeToBlackBy( leds, NUM_LEDS, 255);
	for (byte i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = CHSV(i * 25, 255, beatsin8(62)); // CHSV(Farbton ,Farbsättigung , Helligkeit)
		FastLED.delay(50);
	}
	for (byte k = NUM_LEDS; k > 0; k--)
	{
		leds[k] = CHSV(k * 10, 255, beatsin8(62)); // CHSV(Farbton ,Farbsättigung , Helligkeit)
		FastLED.delay(50);
	}
}

void rainbow()
{
	#ifdef DEBUG
		Serial.println("Rainbow");
	#endif// FastLEDs eingebauter Regenbogengenerator
	fill_rainbow(leds, NUM_LEDS, gHue, 2);
}

void sinelon()
{
	Serial.println("sinelon");
	// zwei farbiger Punkt, der hin und her fegt, mit verblassenden Spuren im Gleichlauf bis zur Hälfte
	fadeToBlackBy(leds, HALF_NUM, 20); // 20
	int pos1 = beatsin16(13, 0, HALF_NUM - 1);
	leds[pos1] += CHSV(gHue, 255, 192);

	fadeToBlackBy(leds, NUM_LEDS, 20); // 20
	int pos2 = beatsin16(13, HALF_NUM, NUM_LEDS - 1);
	leds[pos2] += CHSV(gHue, 255, 192);
}

void Rain_schwinge()
{
	#ifdef DEBUG
		Serial.println("Rainschwinge");
	#endif
	for (byte i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = CHSV((i * 10) + beatsin8(42), 255, beatsin8(125, 100, 192));
	}
}

// void Band() {
//	for (int i = 0; i < HALF_NUM+1; i++ ) {
//        leds[i] = (gHue++, 255, 192);
//        FastLED.delay(20);
//        leds[i] =CRGB::Black;
//	}
//	for (int z = NUM_LEDS; z >HALF_NUM; z-- ) {
//        leds[z] = (gHue++, 255, 192);
//        FastLED.delay(20);
//        leds[z] =CRGB::Black;
//	}
// }

void bpm()
{
	#ifdef DEBUG
		Serial.println("bmp");
	#endif// farbige Streifen, die mit einer definierten Beats-Per-Minute (BPM) pulsieren
	uint8_t BeatsPerMinute = 42;
	CRGBPalette16 palette = PartyColors_p;
	uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
	for (int i = 0; i < NUM_LEDS; i++)
	{ // 9948
		leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
	}
}

void juggle()
{
	#ifdef DEBUG
		Serial.println("jungle");
	#endif
	// acht farbige Punkte, die sich synchron zueinander verweben
	fadeToBlackBy(leds, NUM_LEDS, 20);
	byte dothue = 0;
	for (int i = 0; i < 8; i++)
	{
		leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
		dothue += 32;
		FastLED.delay(40);
	}
}

void Regenbogen()
{ // ca 20 Leds mit ständig wechselnen Frarben und Schweif
  // Alle LEDS mit ständig wechselner Farbe
    #ifdef DEBUG
	  Serial.println("Regenbogen - P1");
	#endif
	for (int z = 0; z < Anz; z++)
	{						// Schleife für Anzahl der Durchläufe
		static uint8_t hue; // Var für Farbton
		for (int i = 0; i < NUM_LEDS; i++)
		{
			fadeToBlackBy(leds, NUM_LEDS, 20);
			leds[i] = CHSV(hue++, 255, 255);
			//   leds[NUM_LEDS] = leds(NUM_LEDS - 4 );
			fadeToBlackBy(leds, NUM_LEDS, 40);
			leds[i] = CHSV(hue++, 255, 255);
			//   leds(NUM_LEDS, NUM_LEDS * 4) = leds(NUM_LEDS * 4 , 0);
			//    (144,143) = (144,0)
			//   leds(NUM_LEDS,NUM_LEDS-1) = leds(NUM_LEDS - 1 ,0);
			FastLED.delay(40);
		}
	}
}

