#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
import time
import json
import threading
import paho.mqtt.client as mqtt
from luma.core.interface.serial import i2c
from luma.core.render import canvas
from luma.oled.device import ssd1306

BROKER = "localhost"
TOPIC = "sensors/data"

latest_data = {
    "temperature": "--",
    "humidity": "--",
    "pressure": "--",
    "altitude": "--"
}

def get_device():
    serial = i2c(port=1, address=0x3C)
    return ssd1306(serial)

def on_connect(client, userdata, flags, reason_code, properties):
    print("Connected to broker!")
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    payload = msg.payload.decode().strip()
    if not payload:
        return
    try:
        data = json.loads(payload)
        latest_data["temperature"] = round(data["temperature"], 1)
        latest_data["humidity"]    = round(data["humidity"], 1)
        latest_data["pressure"]    = round(data["pressure"], 1)
        latest_data["altitude"]    = round(data["altitude"], 1)
    except (json.JSONDecodeError, KeyError) as e:
        print(f"Error parsing message: {e}")

def mqtt_thread():
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(BROKER, 1883)
    client.loop_forever()

def main():
    device = get_device()

    t = threading.Thread(target=mqtt_thread, daemon=True)
    t.start()

    while True:
        with canvas(device) as draw:
            draw.text((0,  0),  f"Temp:     {latest_data['temperature']} C",  fill="white")
            draw.text((0, 16),  f"Humidity: {latest_data['humidity']} %",     fill="white")
            draw.text((0, 32),  f"Pressure: {latest_data['pressure']} hPa",   fill="white")
            draw.text((0, 48),  f"Altitude: {latest_data['altitude']} m",     fill="white")
        time.sleep(1)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass