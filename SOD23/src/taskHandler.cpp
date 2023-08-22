#include "headers/taskHandler.hpp"
BH1750 lightMeter;
CRGB leds[NUM_LEDS];

/**
 *
 */
void parsePayload(String payload)
{
}

/**
 * @brief Task per gestire il sensore di luminosità in base allo stato del sensore di movimento PIR.
 *
 * Questo task legge lo stato del sensore di movimento PIR passato come parametro e, se attivo, legge
 * la luminosità utilizzando la funzione `getLux`. Successivamente, imposta la luminosità della matrice
 * LED in base al valore letto e stampa il valore di luminosità sulla porta seriale. Se il sensore di
 * movimento PIR è disattivo, stampa un messaggio appropriato. Infine, il task si autodistrugge.
 *
 * @param parameter Puntatore all'intero che rappresenta lo stato del sensore di movimento PIR.
 */
void lightSensorTask(void *parameter)
{
    unsigned int pir_status = *(unsigned int *)parameter;

    if (pir_status)
    {
        float lux = getLux();
        FastLED.setBrightness(setBrightness(lux));

        Serial.println("Brightness: " + String(setBrightness(lux)));
    }
    else
    {
        Serial.println("Sensore di luminosità spento (movimento non rilevato)");
    }
    vTaskDelete(NULL);
}

/**
 * @brief Task per gestire la matrice LED in base allo stato del sensore di movimento PIR.
 *
 * Questo task legge lo stato del sensore di movimento PIR passato come parametro e imposta i colori
 * della matrice LED di conseguenza utilizzando la funzione `setColors`. Dopo aver completato il suo
 * compito, il task si autodistrugge.
 *
 * @param parameter Puntatore all'intero che rappresenta lo stato del sensore di movimento PIR.
 */
void ledMatrixTask(void *parameter)
{
    unsigned int pir_status = *(unsigned int *)parameter;
    setColors(pir_status);

    vTaskDelete(NULL);
}

/**
 * @brief Crea e avvia i task per gestire il sensore di luminosità e la matrice LED.
 *
 * Questa funzione crea due task: uno per gestire il sensore di luminosità e uno per gestire la matrice LED.
 * I task vengono creati con le priorità specificate e i parametri necessari vengono passati come puntatori.
 *
 * @param pir_status Lo stato del sensore di movimento PIR.
 */
void getTasks(unsigned int pir_status)
{
    // Crea il task per gestire il sensore di luminosità con priorità 1
    xTaskCreate(lightSensorTask, "LIGHT_SENSOR_TASK", 10000, (void *)&pir_status, 1, NULL);

    // Crea il task per gestire la matrice LED con priorità 2
    xTaskCreate(ledMatrixTask, "MOTION_TASK", 10000, (void *)&pir_status, 2, NULL);
}

/**
 * @brief Funzione per mappare le coordinate (x, y) su un indice di LED.
 *
 * @param x La coordinata x del LED nella matrice.
 * @param y La coordinata y del LED nella matrice.
 *
 * @return L'indice corrispondente all'array di LED.
 */
uint8_t XY(uint8_t x, uint8_t y)
{
    // Verifica se le coordinate sono fuori dai limiti della matrice
    if ((x >= kMatrixWidth) || (y >= kMatrixHeight))
    {
        return (LAST_VISIBLE_LED + 1);
    }

    // Tabella di mappatura degli indici di LED
    const uint8_t XYTable[] = {
        56, 48, 40, 32, 24, 16, 8, 0,
        57, 49, 41, 33, 25, 17, 9, 1,
        58, 50, 42, 34, 26, 18, 10, 2,
        59, 51, 43, 35, 27, 19, 11, 3,
        60, 52, 44, 36, 28, 20, 12, 4,
        61, 53, 45, 37, 29, 21, 13, 5,
        62, 54, 46, 38, 30, 22, 14, 6,
        63, 55, 47, 39, 31, 23, 15, 7};

    // Calcola l'indice basato sulle coordinate
    uint8_t i = (y * kMatrixWidth) + x;
    uint8_t j = XYTable[i];
    return j;
}

/**
 * @brief Imposta i colori dei LED in base allo stato del sensore PIR.
 *
 * Questa funzione imposta i colori dei LED sulla matrice in base allo stato del sensore PIR.
 * Se il sensore PIR rileva movimento, vengono impostati dei colori specifici per i LED.
 *
 * @param pir_status Lo stato del sensore PIR: 1 se rileva movimento, 0 altrimenti.
 */
void setColors(int pir_status)
{

    if (pir_status)
    {
        // Imposta i colori dei LED in base allo schema fornito
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

        // Mostra i colori impostati
        FastLED.show();
        // Cancella i colori e spegni i LED
        FastLED.clear();
        // Imposta la luminosità a 0 per spegnere completamente i LED
        FastLED.setBrightness(0);
    }
    else
        FastLED.show();
}

/**
 * @brief Configura la matrice LED utilizzando FastLED.
 *
 * Questa funzione configura la matrice LED utilizzando la libreria FastLED.
 * Viene specificato il tipo di chipset, il pin di collegamento dei LED, l'ordine dei colori
 * e il puntatore all'array dei LED. Inoltre, viene applicata una correzione tipica per i LED SMD5050.
 */
void setMatrixConfig()
{
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
}

/**
 * @brief Imposta la luminosità dei LED in base alla luminosità rilevata dal sensore di luminosità.
 *
 * @param lux La luminosità rilevata dal sensore di luminosità.
 *
 * @return La luminosità da impostare per i LED.
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

/**
 * @brief Ottiene il livello di illuminazione in lux dal sensore di luminosità.
 *
 * @return Il livello di illuminazione in lux.
 */
float getLux()
{
    lightMeter.begin();
    float lux = (int)lightMeter.readLightLevel(); // leggi il valore dal sensore di luminosità
    String message = "Luminosità rilevata: " + String(lux);
    Serial.println(message);
    return lux;
}
