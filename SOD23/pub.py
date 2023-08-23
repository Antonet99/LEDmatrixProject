import time
import paho.mqtt.client as mqtt


def on_publish(client, userdata, mid):
	print("Messaggio pubblicato")


client = mqtt.Client("rpi_client2") #this name should be unique
client.on_publish = on_publish
client.connect('192.168.1.3',1883)
client.loop_start()

#apri file colors.txt 
f = open("colors.txt","r")

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

	time.sleep(0.01)
