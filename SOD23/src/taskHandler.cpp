#include "headers/taskHandler.hpp"

const char *topic_images = "rpi/images";
const char *topic_req = "data/reqImage";
const char *ssid = "Vodafone-A48487438";
const char *wifi_password = "psLLfEEMA4AdGhCX";

const char *mqtt_username = "sod";   // my mqtt username
const char *mqtt_password = "sod23"; // my mqtt password
const char *mqtt_clientID = "esp32_sod";
const char *mqtt_server = "192.168.1.3"; // my mqtt server address
unsigned int mqtt_port = 1883;
vector<string> colors;
BH1750 lightMeter;
CRGB leds[NUM_LEDS];
WiFiClient askClient;
PubSubClient client(mqtt_server, mqtt_port, askClient); // my clientID

/**
 *
 */
void parsePayload(vector<string> payload)
{
    Serial.println("dentro il parser");
    string data = "";

    int countColors = 0;
    Serial.println(payload.size());

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            string s = payload[countColors];
            string a = s.replace(0, 5, "");

            const char *n1 = s.substr(0, a.find_first_of(",")).c_str();
            string replace1 = a.replace(0, a.find_first_of(",") + 1, "");
            Serial.println("numero 1 : ");
            Serial.println(n1);
            const char *n2 = replace1.substr(0, replace1.find_first_of(",")).c_str();
            Serial.println("numero 2 : ");
            Serial.println(n2);
            string replace2 = replace1.replace(0, replace1.find_first_of(",") + 1, "");
            string replace3 = replace2.replace(replace2.find_first_of(")"), 1, "");
            const char *n3 = replace3.substr(0).c_str();
            Serial.println("numero 3 : ");
            Serial.println(n3);
            leds[XY(j, i)] = CRGB((uint8_t)atoi(n1), (uint8_t)atoi(n2), (uint8_t)atoi(n3));
            countColors++;
        }
    }

    // Cancella il task corrente
    vTaskDelete(NULL);
}

// Task per gestire il sensore di luminosità
void lightSensorTask(void *parameter)
{
    // Legge lo stato del sensore di movimento
    unsigned int pir_status = *(unsigned int *)parameter;

    for (;;)
    {
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
            Serial.println("Sensore di luminosità spento ( movimento non rilevato )");
        }
        vTaskDelay(5000);
    }
    // Cancella il task corrente
    vTaskDelete(NULL);
}

/**
 *
 */
void ledMatrixTask(void *parameter)
{
    // Legge lo stato del sensore di movimento e imposta i colori della matrice LED
    unsigned int pir_status = *(unsigned int *)parameter;
    // setColors(pir_status);

    // Cancella il task corrente
    vTaskDelete(NULL);
}
void imageRequestTask(void *parameter)
{
    unsigned int pir_status = *(unsigned int *)parameter;
    for (;;)
    {
        client.publish(topic_req, "immagine1");

        setColors(pir_status);
        vTaskDelay(5000);
    }
    client.setCallback(callback);
    client.subscribe(topic_images);
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
    xTaskCreate(imageRequestTask, "IMAGE_REQUEST_TASK", 10000, (void *)&pir_status, 1, NULL);
    xTaskCreate(lightSensorTask, "LIGHT_SENSOR_TASK", 10000, (void *)&pir_status, 2, NULL);
    xTaskCreate(ledMatrixTask, "LEDMATRIX_TASK", 10000, (void *)&pir_status, 3, NULL);
}
/**
 *
 */
uint8_t XY(uint8_t x, uint8_t y)
{
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
        FastLED.show();
        FastLED.clear();
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

void mqttConn()
{
    Serial.print("**** Connesso al server MQTT : ");
    Serial.println(mqtt_server);

    client.setServer(mqtt_server, mqtt_port);
    reconnect();
}

/**
 * Tentativo di riconnessione al server MQTT in caso di insuccesso.
 */
void reconnect()
{
    // ciclo con il quale si tenta la riconnessione
    while (!client.connected())
    {
        Serial.print("********** Tentativo di connessione MQTT...");
        if (client.connect(mqtt_clientID, mqtt_username, mqtt_password))
        {
            Serial.println("-> client MQTT connesso");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println("-> nuovo tentativo fra 5 secondi");
            delay(5000);
        }
    }
}
void wifiConn()
{

    Serial.print("********** Connessione al WiFi in corso :");
    Serial.println(ssid);

    WiFi.begin(ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("->Connessione al WiFi effettuata");
    Serial.println("-> indirizzo IP: ");
    Serial.println(WiFi.localIP());
}
/**
 *
 */
void initPubSub()
{

    client.setServer(mqtt_server, mqtt_port);
    client.connect(mqtt_clientID, mqtt_username, mqtt_password);
}
/**
 * Funzione di callback utilizzata per verificare la corretta ricezione dei dati
 * ed il payload con le informazioni.
 * @param topic topic dal quale vengono ricevuti i dati (topic su cui è stato effettuato il subscribe)
 * @param payload dati effettivi ricevuti dal topic
 * @param lenght lunghezza del messaggio.
 */
void callback(char *topic, byte *payload, unsigned int length)
{
    char *message = new char[length];
    Serial.print("Messaggio ricevuto dal topic : ");
    Serial.print(topic);
    Serial.print('\n');
    string s = "";
    // string strTot = "";

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        s = s + (char)payload[i];
    }

    if (s == "fine")
    {
        parsePayload(colors);
    }
    else
    {
        colors.push_back(s);
        Serial.print('\n');
    }

    // if(topic == topic_images) Serial.println((char*) payload);
    // else Serial.println("NESSUN SUBSCRIBE");
}
/**
 *
 */
void clientLoop()
{
    client.loop();
}