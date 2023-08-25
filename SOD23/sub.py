import paho.mqtt.client as mqtt
import time


def on_connect(client, userdata, flags, rc):
   global flag_connected
   flag_connected = 1
   client_subscriptions(client)
   print("Connesso a MQTT server")

def on_disconnect(client, userdata, rc):
   global flag_connected
   flag_connected = 0
   print("Non connesso a MQTT server")

def callback_request_image(client, userdata, msg):
    print('Immagine richiesta: ', msg.payload.decode('utf-8'))
    
    
def client_subscriptions(client):
    client.subscribe("data/reqImage") #topic su cui effettuare il subscribe per ricevere la richiesta

client = mqtt.Client("rpi_client") #Nome univoco per identificare il client
flag_connected = 0

client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.message_callback_add('data/reqImage', callback_request_image)
client.connect('192.168.1.3',1883)
# start a new thread
client.loop_start()
client_subscriptions(client)
print("......client setup concluso............")


while True:
    time.sleep(10)
    if (flag_connected != 1):
        print("tentativo di connessione a MQTT server..")
        
   
