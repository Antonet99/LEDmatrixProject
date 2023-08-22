#include <iostream>
#include <Wire.h>
#include <string.h>
#include "headers/taskHandler.hpp"

using namespace std;

/**
 * @brief Definizione delle costanti e delle variabili utilizzate nel programma.
 */

// Pin del sensore di movimento PIR
int PIR_data = 19;

// Credenziali della rete Wi-Fi
const char *ssid = "Vodafone-A48487438";        // Nome della rete Wi-Fi
const char *wifi_password = "psLLfEEMA4AdGhCX"; // Password della rete Wi-Fi

// Topic MQTT per invio e richiesta di immagini
const char *topic_images = "rpi/images"; // Topic per invio immagini
const char *topic_req = "data/reqImage"; // Topic per richiesta immagini

// Credenziali per la connessione MQTT
const char *mqtt_username = "sod";        // Nome utente MQTT
const char *mqtt_password = "sod23";      // Password utente MQTT
const char *mqtt_clientID = "esp32_sod";  // ID del cliente MQTT
const char *mqtt_server = "192.168.1.21"; // Indirizzo del server MQTT
unsigned int mqtt_port = 1883;            // Porta del server MQTT

// Oggetto WiFiClient per la comunicazione con il server MQTT
WiFiClient askClient;

// Oggetto PubSubClient per la gestione della connessione MQTT
PubSubClient client(mqtt_server, mqtt_port, askClient);

/**
 * @brief Funzione per la connessione alla rete Wi-Fi.
 */
void wifiConn()
{
  Serial.print("********** Connessione al WiFi in corso: ");
  Serial.println(ssid); // Mostra il nome della rete Wi-Fi in corso di connessione

  WiFi.begin(ssid, wifi_password); // Avvia la connessione alla rete Wi-Fi

  // Attendi fino a quando la connessione non viene stabilita
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println(""); // Stampa una riga vuota
  Serial.println("-> Connessione al WiFi effettuata");
  Serial.print("-> Indirizzo IP: ");
  Serial.println(WiFi.localIP()); // Stampa l'indirizzo IP assegnato al dispositivo dopo la connessione
}

/**
 * @brief Funzione per la connessione al server MQTT.
 */
void mqttConn()
{
  Serial.print("**** Connesso al server MQTT: ");
  Serial.println(mqtt_server); // Mostra l'indirizzo del server MQTT a cui si sta connettendo

  client.setServer(mqtt_server, mqtt_port); // Imposta il server MQTT e la porta
  reconnect();                              // Richiama la funzione per gestire la riconnessione in caso di perdita di connessione
}

/**
 * @brief Funzione per riconnettersi al server MQTT in caso di perdita della connessione.
 */
void reconnect()
{
  // Ciclo con il quale si tenta la riconnessione
  while (!client.connected())
  {
    Serial.print("********** Tentativo di connessione MQTT...");

    // Si prova a connettersi al server MQTT con le credenziali fornite
    if (client.connect(mqtt_clientID, mqtt_username, mqtt_password))
    {
      Serial.println("-> client MQTT connesso");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state()); // Mostra lo stato attuale del client MQTT
      Serial.println("-> nuovo tentativo fra 5 secondi");
      delay(5000); // Attende 5 secondi prima di un nuovo tentativo
    }
  }
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
  String s = "";
  for (int i = 0; i < length; i++)
  {

    // Serial.print((char)payload[i]);
    s = s + String((char)payload[i]);
  }
  Serial.print(s);
  Serial.print('\n');
  parsePayload(s);

  // if(topic == topic_images) Serial.println((char*) payload);
  // else Serial.println("NESSUN SUBSCRIBE");
}

/**
 * @brief Funzione di configurazione iniziale eseguita all'avvio.
 */
void setup()
{
  Serial.begin(9600);

  // Connessione WiFi e MQTT
  wifiConn();
  mqttConn();

  pinMode(PIR_data, INPUT);

  // Configurazione di base della matrice LED
  // setMatrixConfig();

  // Impostazioni del client MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.connect(mqtt_clientID, mqtt_username, mqtt_password);

  // Impostazione della callback per la gestione dei messaggi MQTT
  client.setCallback(callback);

  // Pubblicazione di un messaggio sul topic di richiesta immagini
  client.publish(topic_req, "immagine 1");

  // Sottoscrizione al topic delle immagini
  client.subscribe(topic_images);

  // Inizializzazione della comunicazione I2C
  Wire.begin();
}

/**
 * @brief Funzione principale che viene eseguita in loop continuo.
 */
void loop()
{
  // Mantieni attiva la connessione MQTT e gestisci i messaggi in entrata
  client.loop();

  // Leggi lo stato del sensore PIR
  bool pir_status = digitalRead(PIR_data);

  // Trasforma lo stato del PIR in un valore intero (0 o 1)
  unsigned int pir_value = static_cast<unsigned int>(pir_status);

  // Esegui le attività corrispondenti allo stato del PIR
  // getTasks(pir_value);

  // Ritardo di 8 secondi prima della successiva iterazione
  delay(8000);
}
