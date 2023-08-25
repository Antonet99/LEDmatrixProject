#ifndef CONSTTASKS_HPP
#define CONSTTASKS_HPP

// Includsione  librerie necessarie
#include <iostream>
#include <Wire.h>
#include <BH1750.h>
#include <FastLED.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <vector>
#include <string.h>

using namespace std;

// Definizioni costanti
#define LED_PIN 15          // Pin di connessione della matrice LED
#define BRIGHTNESS 25       // Livello di luminosità predefinito
#define COLOR_ORDER GRB     // Ordine dei colori per i LED
#define CHIPSET WS2812B     // Tipo di chipset dei LED
#define LAST_VISIBLE_LED 63 // Ultimo LED visibile

// Dimensioni della matrice LED
const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 8;
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

#endif
