#include "headers/constants/constants.hpp"
#include "headers/connections.hpp"
#include "headers/constants/constMqtt.hpp"

/**
 * Topic utilizzato per pubblicare e per ottenere dati.
 * Può essere considerato come un canale di comunicazione tra Esp32 e Rpi.
 */
const char *topic = "sod/mutual_connection";

/**
 * Effettua connessione al WiFi e mostra indirizzo IP locale assegnato.
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
 * Effettua connessione dell' ESP32 al server MQTT
 */
void mqttConn()
{
  Serial.print("**** Connesso al server MQTT : ");
  Serial.println(mqtt_server);

  client.setServer(mqtt_server, mqtt_port);
  reconnect();
}

/**
 * Riconnessione al server MQTT
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
