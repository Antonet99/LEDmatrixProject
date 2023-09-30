import time
import paho.mqtt.client as mqtt


def on_publish(client, userdata, mid):
	print("Messaggio pubblicato")


def callback_request_image(client, userdata, msg):
    print('Immagine richiesta: ', msg.payload.decode('utf-8'))
    if msg.payload.decode('utf-8') == 'immagine1':
    	publish_img("colors_img1.txt")
    elif msg.payload.decode('utf-8') == 'immagine2':
        publish_img("colors_img2.txt")
    elif msg.payload.decode('utf-8') == 'immagine3':
        publish_img("colors_img3.txt")



def publish_img(colors_file):
	#apri file colors.txt 
	f = open(colors_file,"r")
	#leggi righe
	msg = f.readlines()
	count = 0
	#leggi riga per riga
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
	
    
        
client = mqtt.Client("rpi_client2") 
client.on_publish = on_publish
client.connect('192.168.0.106',1883)
client.message_callback_add('data/reqImage', callback_request_image)
client.loop_start()



