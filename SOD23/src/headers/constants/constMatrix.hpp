#ifndef CONSTMATRIX_HPP
#define CONSTMATRIX_HPP
#include <FastLED.h>

#define LED_PIN 2           // pin di connessione matrice
#define BRIGHTNESS 255
#define COLOR_ORDER GRB     // ordine dei colori
#define CHIPSET WS2812B     // tipo di chipset
#define LAST_VISIBLE_LED 63 // ultimo led visibile
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)


const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 8;


extern CRGB leds[NUM_LEDS];

#endif
