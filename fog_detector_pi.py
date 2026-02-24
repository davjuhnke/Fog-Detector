import paho.mqtt.client as mqtt
import json

BROKER = "localhost"
TOPIC = "sensors/data"

def on_connect(client, userdata, flags, rc):
    print("Connected to broker!")
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    data = json.loads(msg.payload.decode())
    print(f"Temperature: {data['temperature']} °C")
    print(f"Humidity: {data['humidity']} %")
    print(f"Pressure: {data['pressure']} hPa")
    print(f"Altitude: {data['altitude']} m")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, 1883)
client.loop_forever()