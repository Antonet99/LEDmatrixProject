#include "headers/constants/constants.hpp"
#include "headers/connections.hpp"
#include "headers/constants/constMqtt.hpp"

/**
 * Topic utilizzato per pubblicare e per ottenere dati. 
 * Può essere considerato come un canale di comunicazione tra Esp32 e Rpi.
*/
const char* topic = "sod/mutual_connection";

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
 * Funzione di callback utilizzata per verificare la corretta ricezione dei dati 
 * ed il payload con le informazioni.
 * @param topic topic dal quale vengono ricevuti i dati (topic su cui è stato effettuato il subscribe)
 * @param payload dati effettivi ricevuti dal topic
 * @param lenght lunghezza del messaggio.
*/
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Messaggio ricevuto dal topic :");
  Serial.print(topic);
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

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