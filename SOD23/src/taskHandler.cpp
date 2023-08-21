#include "headers/taskHandler.hpp"
BH1750 lightMeter;
CRGB leds[NUM_LEDS];

/**
 *
 */
void parsePayload(String payload)
{
}

// Task per gestire il sensore di luminosità
void lightSensorTask(void *parameter)
{
    // Legge lo stato del sensore di movimento
    unsigned int pir_status = *(unsigned int *)parameter;

    // Se il sensore di movimento è attivo
    if (pir_status)
    {
        // Ottiene la luminosità e imposta la luminosità dei LED
        float lux = getLux();
        FastLED.setBrightness(setBrightness(lux));

        // Stampa la luminosità sulla porta seriale
        Serial.println("Brightness: " + String(setBrightness(lux)));
    }
    else
    {
        // Stampa un messaggio sulla porta seriale indicando che il sensore di luminosità è spento
        Serial.println("Sensore di luminosità spento ( movimento non rilevato )");
    }
    // Cancella il task corrente
    vTaskDelete(NULL);
}

// Task per gestire la matrice LED
void ledMatrixTask(void *parameter)
{
    // Legge lo stato del sensore di movimento e imposta i colori della matrice LED
    unsigned int pir_status = *(unsigned int *)parameter;
    setColors(pir_status);

    // Cancella il task corrente
    vTaskDelete(NULL);
}

/**
 * Crea due task per gestire sensore di luminosità e matrice LED.
 * Argomento: stato del sensore di movimento.
 * La funzione chiama la funzione xTaskCreate per creare il task lightSensorTask con priorità 1 e il task ledMatrixTask con priorità 2.
 * Entrambi i task ricevono lo stato del sensore di movimento come parametro.
 */
void getTasks(unsigned int pir_status)
{
    // Crea il task per gestire il sensore di luminosità con priorità 1
    xTaskCreate(lightSensorTask, "LIGHT_SENSOR_TASK", 10000, (void *)&pir_status, 1, NULL);

    // Crea il task per gestire la matrice LED con priorità 2
    xTaskCreate(ledMatrixTask, "MOTION_TASK", 10000, (void *)&pir_status, 2, NULL);
}

uint8_t XY(uint8_t x, uint8_t y)
{
    // any out of bounds address maps to the first hidden pixel
    if ((x >= kMatrixWidth) || (y >= kMatrixHeight))
    {
        return (LAST_VISIBLE_LED + 1);
    }

    const uint8_t XYTable[] = {
        56, 48, 40, 32, 24, 16, 8, 0,
        57, 49, 41, 33, 25, 17, 9, 1,
        58, 50, 42, 34, 26, 18, 10, 2,
        59, 51, 43, 35, 27, 19, 11, 3,
        60, 52, 44, 36, 28, 20, 12, 4,
        61, 53, 45, 37, 29, 21, 13, 5,
        62, 54, 46, 38, 30, 22, 14, 6,
        63, 55, 47, 39, 31, 23, 15, 7};

    uint8_t i = (y * kMatrixWidth) + x;
    uint8_t j = XYTable[i];
    return j;
}

void setColors(int pir_status)
{

    if (pir_status)
    {
        leds[XY(0, 0)] = CRGB(255, 255, 0);
        leds[XY(1, 0)] = CRGB(255, 255, 0);
        leds[XY(2, 0)] = CRGB(255, 255, 0);
        leds[XY(3, 0)] = CRGB(35, 78, 29);
        leds[XY(4, 0)] = CRGB(35, 78, 29);
        leds[XY(5, 0)] = CRGB(35, 78, 29);
        leds[XY(6, 0)] = CRGB(255, 255, 0);
        leds[XY(7, 0)] = CRGB(255, 255, 0);
        leds[XY(0, 1)] = CRGB(255, 255, 0);
        leds[XY(1, 1)] = CRGB(255, 255, 0);
        leds[XY(2, 1)] = CRGB(20, 160, 0);
        leds[XY(3, 1)] = CRGB(20, 160, 0);
        leds[XY(4, 1)] = CRGB(35, 78, 29);
        leds[XY(5, 1)] = CRGB(153, 119, 76);
        leds[XY(6, 1)] = CRGB(255, 255, 0);
        leds[XY(7, 1)] = CRGB(255, 255, 0);
        leds[XY(0, 2)] = CRGB(255, 255, 0);
        leds[XY(1, 2)] = CRGB(20, 160, 0);
        leds[XY(2, 2)] = CRGB(20, 160, 0);
        leds[XY(3, 2)] = CRGB(20, 160, 0);
        leds[XY(4, 2)] = CRGB(20, 160, 0);
        leds[XY(5, 2)] = CRGB(20, 160, 0);
        leds[XY(6, 2)] = CRGB(20, 160, 0);
        leds[XY(7, 2)] = CRGB(20, 160, 0);
        leds[XY(0, 3)] = CRGB(20, 160, 0);
        leds[XY(1, 3)] = CRGB(255, 255, 0);
        leds[XY(2, 3)] = CRGB(20, 160, 0);
        leds[XY(3, 3)] = CRGB(0, 0, 0);
        leds[XY(4, 3)] = CRGB(20, 160, 0);
        leds[XY(5, 3)] = CRGB(0, 0, 0);
        leds[XY(6, 3)] = CRGB(0, 0, 0);
        leds[XY(7, 3)] = CRGB(20, 160, 0);
        leds[XY(0, 4)] = CRGB(20, 160, 0);
        leds[XY(1, 4)] = CRGB(20, 160, 0);
        leds[XY(2, 4)] = CRGB(0, 0, 0);
        leds[XY(3, 4)] = CRGB(0, 0, 0);
        leds[XY(4, 4)] = CRGB(20, 160, 0);
        leds[XY(5, 4)] = CRGB(20, 160, 0);
        leds[XY(6, 4)] = CRGB(20, 160, 0);
        leds[XY(7, 4)] = CRGB(20, 160, 0);
        leds[XY(0, 5)] = CRGB(20, 160, 0);
        leds[XY(1, 5)] = CRGB(255, 255, 0);
        leds[XY(2, 5)] = CRGB(20, 160, 0);
        leds[XY(3, 5)] = CRGB(0, 0, 0);
        leds[XY(4, 5)] = CRGB(20, 160, 0);
        leds[XY(5, 5)] = CRGB(0, 0, 0);
        leds[XY(6, 5)] = CRGB(0, 0, 0);
        leds[XY(7, 5)] = CRGB(20, 160, 0);
        leds[XY(0, 6)] = CRGB(255, 255, 0);
        leds[XY(1, 6)] = CRGB(20, 160, 0);
        leds[XY(2, 6)] = CRGB(20, 160, 0);
        leds[XY(3, 6)] = CRGB(20, 160, 0);
        leds[XY(4, 6)] = CRGB(20, 160, 0);
        leds[XY(5, 6)] = CRGB(20, 160, 0);
        leds[XY(6, 6)] = CRGB(20, 160, 0);
        leds[XY(7, 6)] = CRGB(20, 160, 0);
        leds[XY(0, 7)] = CRGB(255, 255, 0);
        leds[XY(1, 7)] = CRGB(255, 255, 0);
        leds[XY(2, 7)] = CRGB(20, 160, 0);
        leds[XY(3, 7)] = CRGB(20, 160, 0);
        leds[XY(4, 7)] = CRGB(35, 78, 29);
        leds[XY(5, 7)] = CRGB(255, 255, 0);
        leds[XY(6, 7)] = CRGB(255, 255, 0);
        leds[XY(7, 7)] = CRGB(255, 255, 0);
        FastLED.show();
        FastLED.clear();
        FastLED.setBrightness(0);
    }
    else
        FastLED.show();
}
/*
 *
 */

void setMatrixConfig()
{
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
}
/*
 *
 */

/*uint8_t setBrightness(float lux)
{
    uint8_t brightness = 0;
    if (lux > 0 && lux < 40)
        brightness = 2;
    else if (lux > 40 && lux < 120)
        brightness = 50;
    else if (lux > 120 && lux < 200)
        brightness = 100;
    else if (lux > 200 && lux < 300)
        brightness = 180;
    return brightness;
}
*/

uint8_t setBrightness(float lux)
{
    uint8_t brightness = 0;
    switch (static_cast<int>(lux))
    {
    case 1 ... 39:
        brightness = 2;
        break;
    case 40 ... 119:
        brightness = 50;
        break;
    case 120 ... 199:
        brightness = 100;
        break;
    case 200 ... 299:
        brightness = 180;
        break;
    default:
        // Default case
        brightness = 0;
        break;
    }
    return brightness;
}

/*
 *
 */
float getLux()
{
    lightMeter.begin();
    float lux = (int)lightMeter.readLightLevel(); // leggi il valore dal sensore di luminosità
    String message = "Luminosità rilevata: " + String(lux);
    Serial.println(message);
    return lux;
}
