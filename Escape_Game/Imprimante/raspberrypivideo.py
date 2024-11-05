# Code Python pour l'imprimante
# Un raspberry zero 2w peut être suffisant pour l'imprimante
import paho.mqtt.client as mqtt
import os

def on_connect(client, userdata, flags, rc):
    print("Connecté au broker MQTT")
    client.subscribe("escape_game/video/imprimante")

def on_message(client, userdata, msg):
    if msg.topic == "escape_game/video/imprimante" and msg.payload.decode() == "play":
        print("Lecture de la vidéo de l'imprimante")
        os.system("omxplayer /path/to/video.mp4")  # Pour lancer la vidéo sur Raspberry Pi

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.50.100", 1883, 60)
client.loop_forever()
