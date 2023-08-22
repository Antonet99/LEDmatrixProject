#ifndef CONSTTASKS_HPP
#define CONSTTASKS_HPP

// Includi le librerie necessarie
#include <BH1750.h>
#include <FastLED.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>

// Definizioni delle costanti
#define LED_PIN 15          // Pin di connessione per la matrice LED
#define BRIGHTNESS 25       // Luminosità predefinita
#define COLOR_ORDER GRB     // Ordine dei colori
#define CHIPSET WS2812B     // Tipo di chipset LED
#define LAST_VISIBLE_LED 63 // Ultimo LED visibile

// Dimensioni della matrice LED
const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 8;
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

#endif