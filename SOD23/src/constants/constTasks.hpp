#ifndef CONSTTASKS_HPP
#define CONSTTASKS_HPP
#include <BH1750.h>
#include <FastLED.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define LED_PIN 15         // pin di connessione matrice
#define BRIGHTNESS 25
#define COLOR_ORDER GRB     // ordine dei colori
#define CHIPSET WS2812B     // tipo di chipset
#define LAST_VISIBLE_LED 63 // ultimo led visibile


const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 8;
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)






#endif