import paho.mqtt.client as mqtt
import time
import subprocess


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
    print(msg.payload.decode('utf-8'))

    if msg.payload.decode('utf-8') == 'immagine1':
        print("siamo in immagine 1")
        publish_img("colors_img1.txt")
    elif msg.payload.decode('utf-8') == 'immagine2':
        publish_img("colors_img2.txt")
    elif msg.payload.decode('utf-8') == 'immagine3':
        publish_img("colors_img3.txt")


def publish_img(colors_file):
    # apri file colors.txt
    print("siamo dentroooo")
    f = open(colors_file, "r")
    # leggi righe
    msg = f.readlines()
    count = 0
    # leggi riga per riga
    for line in msg:
        try:
            count += 1
            linestrip = line.strip()
            pubMsg = client.publish(
                topic='rpi/images',
                payload=linestrip.encode('utf-8'),
                qos=0,
            )

            pubMsg.wait_for_publish()
            print(pubMsg.is_published())
            print(linestrip)

        except Exception as e:
            print(e)

        time.sleep(0.001)


def client_subscriptions(client):
    # topic su cui effettuare il subscribe per ricevere la richiesta
    client.subscribe("data/reqImage")


client = mqtt.Client("rpi_client")  # Nome univoco per identificare il client
flag_connected = 0

client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.message_callback_add('data/reqImage', callback_request_image)
client.connect('192.168.0.106', 1883)
client.loop_start()
client_subscriptions(client)
print("......client setup concluso............")


while True:
    time.sleep(10)
    if (flag_connected != 1):
        print("tentativo di connessione a MQTT server..")
