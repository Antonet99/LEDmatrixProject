#include "headers/taskHandler.hpp"

// Dichiarazioni e inizializzazioni delle variabili e oggetti necessari

// Sensore di luminosità
BH1750 lightMeter;

// Matrice LED
CRGB leds[NUM_LEDS];

// Topic MQTT per le immagini
const char *topic_images = "rpi/images";

// Topic MQTT per le richieste di immagini
const char *topic_req = "data/reqImage";

// Credenziali di connessione WiFi
const char *ssid = "TP-LINK_C930";
const char *wifi_password = "88099774";

// Credenziali di connessione MQTT
const char *mqtt_username = "sod";
const char *mqtt_password = "sod23";
const char *mqtt_clientID = "esp32_sod";
const char *mqtt_server = "192.168.0.106";
unsigned int mqtt_port = 1883;

// Variabile per lo stato del sensore di movimento
unsigned int pir_value = 0;

const char* img1 = "immagine1";
const char* img2 = "immagine2";
const char* img3 = "immagine3";

// Vettore per memorizzare i colori ricevuti
vector<string> colors;

// Oggetto per la connessione WiFi
WiFiClient askClient;

// Oggetto per il client MQTT
PubSubClient client(mqtt_server, mqtt_port, askClient);

/**
 * @brief Funzione per analizzare e interpretare il payload ricevuto.
 *
 * Questa funzione prende in input un vettore di stringhe rappresentante il payload ricevuto.
 * Analizza il payload per estrarre i valori dei colori e li utilizza per impostare i colori dei LED sulla matrice.
 *
 * @param payload Un vettore di stringhe contenente il payload ricevuto.
 *
 * NOTA: La funzione utilizza una serie di operazioni di manipolazione delle stringhe per estrarre i valori dei colori.
 * La logica può essere complessa a causa della formattazione dei dati nel payload.
 */
void parsePayload(vector<string> payload)
{
    Serial.println("dentro il parser");
    string data = "";

    int countColors = 0;
    Serial.println(payload.size());
    if (payload.size() != 0)
    {
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
    }
    else
        Serial.println("dimensione del vector = 0");
    getTasks();
}

/**
 * @brief Attività per gestire il sensore di luminosità.
 *
 * Questa attività ciclicamente legge il valore del sensore di movimento e, se attivo, calcola la luminosità ambientale
 * utilizzando la funzione getLux. In base al valore di luminosità, imposta la luminosità della matrice LED utilizzando la
 * funzione setBrightness.
 *
 * NOTA: La chiamata a vTaskDelete(NULL) viene eseguita alla fine dell'attività per eliminare il task.
 */
void lightSensorTask(void *parameter)
{
    Serial.println("Valore del pir nel task sensorel luminosità: ");
    Serial.println(pir_value);

    for (;;)
    {

        if (pir_value)
        {
            float lux = getLux();
            FastLED.setBrightness(setBrightness(lux));

            Serial.println("Brightness: " + String(setBrightness(lux)));
        }
        else
        {
            Serial.println("Sensore di luminosità spento ( movimento non rilevato )");
        }

        vTaskDelay(4000);
    }
    vTaskDelete(NULL);
}

/**
 * @brief Attività per gestire la matrice LED.
 *
 * Questa attività ciclicamente imposta i colori della matrice LED in base allo stato del sensore di movimento.
 * Viene utilizzata la funzione setColors per definire i colori dei LED sulla matrice.
 *
 */
void ledMatrixTask(void *parameter)
{
    for (;;)
    {
        setColors(pir_value);
        vTaskDelay(5000);
    }

    vTaskDelete(NULL);
}

/**
 * @brief Attività per richiedere immagini tramite MQTT.
 *
 * Questa attività pubblica un messaggio di richiesta di immagine su un topic MQTT,
 * quindi si iscrive al topic delle immagini e imposta una callback per gestire le risposte.
 *
 */
void  publishImageRequest(void* parameters)
{
    int counter = 0;
    for(;;){
    pir_value = 0;
    if(counter = 0) {
        client.publish(topic_req, img1);
        counter++;
    }
    else if(counter = 1) {
        client.publish(topic_req, img2);
        counter ++;
    }

    else if(counter = 3) 
    {client.publish(topic_req, img3);
        counter = 0;
    }


    client.subscribe(topic_images);
    client.setCallback(callback);
    
    vTaskDelay(10000);


    }
    vTaskDelete(NULL);
}

/**
 * @brief Attività per monitorare lo stato del sensore PIR.
 *
 * Questa attività legge periodicamente lo stato del sensore PIR e ne
 * stampa lo stato sulla porta seriale.
 *
 * @param parameter Parametro passato all'attività (non utilizzato in questo caso).
 */
void pirStatusTask(void *parameter)
{
    for (;;)
    {
        pir_value = digitalRead(19);

        pir_value ? Serial.println("MOVIMENTO RILEVATO") : Serial.println("NESSUN MOVIMENTO RILEVATO...");
        vTaskDelay(3000);
    }
    vTaskDelete(NULL);
}

/**
 * @brief Configura e avvia tutte le attività necessarie per il sistema.
 *
 * Questa funzione crea le attività per monitorare lo stato del sensore PIR,
 * gestire le letture del sensore di luminosità e gestire la visualizzazione
 * della matrice LED.
 */
void getTasks()
{   xTaskCreate(publishImageRequest,"PUBLISH_IMAGE",10000,NULL,1,NULL);
    xTaskCreate(pirStatusTask, "PIR_STATUS_TASK", 10000, NULL, 2, NULL);
    xTaskCreate(lightSensorTask, "LIGHT_SENSOR_TASK", 10000, NULL, 3, NULL);
    xTaskCreate(ledMatrixTask, "LEDMATRIX_TASK", 10000, NULL, 4, NULL);
}

/**
 * @brief Funzione per mappare le coordinate (x, y) su un indice di LED.
 *
 * Questa funzione mappa una coppia di coordinate (x, y) sulla matrice LED
 * a un indice corrispondente nell'array di LED. Gli indici sono definiti in base
 * alla disposizione dei LED nella matrice e sono utilizzati per accedere
 * agli elementi nell'array.
 *
 * @param x La coordinata x del LED nella matrice.
 * @param y La coordinata y del LED nella matrice.
 * @return L'indice corrispondente all'array di LED.
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

/**
 * @brief Imposta i colori dei LED in base allo stato del sensore PIR.
 *
 * Questa funzione imposta i colori dei LED sulla matrice in base allo stato del sensore PIR.
 * Se il sensore PIR rileva movimento, vengono impostati dei colori specifici per i LED.
 * Altrimenti, viene richiamata solo la funzione FastLED.show() per visualizzare
 * i cambiamenti precedenti della matrice.
 *
 * @param pir_status Lo stato del sensore PIR: 1 se rileva movimento, 0 altrimenti.
 */
void setColors(int pir_status)
{

    if (pir_status)
    {
        Serial.println("Matrice accesa...");
        FastLED.show();
    }
    else
    {
        Serial.println("Matrice spenta...");
        FastLED.setBrightness(0);
        FastLED.show();
    }
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
 * @brief Calcola e imposta la luminosità dei LED sulla matrice in base al valore di luminosità ricevuto.
 *
 * Questa funzione prende in input il valore di luminosità "lux" e calcola un valore di luminosità appropriato per i LED sulla matrice.
 * Il valore "lux" viene suddiviso in intervalli e la luminosità dei LED viene regolata di conseguenza.
 *
 * @param lux Il valore di luminosità ricevuto dal sensore.
 * @return uint8_t Il valore di luminosità dei LED da impostare.
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
 * Questa funzione inizializza il sensore di luminosità, legge il valore di illuminazione
 * e restituisce il valore in formato float. Inoltre, stampa il valore letto sulla porta seriale.
 *
 * @return Il livello di illuminazione in lux.
 */
float getLux()
{
    lightMeter.begin();
    float lux = (int)lightMeter.readLightLevel();
    String message = "Luminosità rilevata: " + String(lux);
    Serial.println(message);
    return lux;
}

/**
 * @brief Connette il client MQTT al server MQTT specificato.
 *
 * Questa funzione imposta il server MQTT e tenta la connessione. Se la connessione non riesce,
 * verranno effettuati tentativi di riconnessione fino a quando la connessione non sarà stabilita.
 */
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

/**
 * @brief Connette il dispositivo al WiFi utilizzando le credenziali specificate.
 *
 * Questa funzione tenta la connessione al WiFi utilizzando le credenziali SSID e password specificate.
 * Verrà attesa la connessione stabilita prima di proseguire.
 */
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
 * @brief Inizializza la connessione al broker MQTT.
 *
 * Questa funzione imposta il server MQTT e tenta di connettersi utilizzando le credenziali fornite.
 * La connessione verrà stabilita o verranno effettuati tentativi fino a quando la connessione non sarà stabilita.
 */
void initPubSub()
{

    client.setServer(mqtt_server, mqtt_port);
    client.connect(mqtt_clientID, mqtt_username, mqtt_password);
}

/**
 * @brief Callback per la ricezione dei messaggi MQTT.
 *
 * Questa funzione viene chiamata quando un messaggio MQTT viene ricevuto sul topic sottoscritto.
 * Il payload del messaggio viene analizzato per ottenere i dati necessari per la configurazione dei colori.
 * Se il payload contiene il messaggio "fine", verrà verificato se sono stati ricevuti abbastanza colori per configurare la matrice LED.
 * In caso contrario, verrà stampato un messaggio di avviso.
 * Se il payload contiene dati colori validi, questi verranno aggiunti al vettore colors.
 *
 * @param topic Il topic del messaggio MQTT.
 * @param payload Il payload del messaggio MQTT.
 * @param length La lunghezza del payload.
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
        if (colors.size() == 64)
        {
            parsePayload(colors);
        }
        else
            Serial.println("Non sono stati ricevuti abbastanza colori");
    }
    else
    {
        colors.push_back(s);
        Serial.print('\n');
    }
}

/**
 * @brief Funzione per la gestione delle operazioni di loop del client MQTT.
 *
 * Questa funzione deve essere chiamata all'interno del loop principale per permettere al
 * client MQTT di eseguire le operazioni di loop necessarie.
 */
void clientLoop()
{
    client.loop();
}