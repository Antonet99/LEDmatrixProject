#include <FastLED.h>

#define LED_PIN 2           // pin di connessione matrice
#define COLOR_ORDER GRB     // ordine dei colori
#define CHIPSET WS2812B     // tipo di chipset
#define LAST_VISIBLE_LED 63 // ultimo led visibile

// #define BRIGHTNESS 255 //da integrare con sensore di luminosità

// parametri fisici matrice
const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 8;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];
