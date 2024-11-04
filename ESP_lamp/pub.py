import time
import paho.mqtt.client as mqtt_client
from uuid import getnode as get_mac
import hashlib
import questionary
import math

topic=questionary.text("What topic should I publish to?").ask()

broker="broker.emqx.io"

h=hashlib.new('sha256')
mac=get_mac()
h.update(str(mac).encode())
pub_id=h.hexdigest()[:10]

client=mqtt_client.Client(mqtt_client.CallbackAPIVersion.VERSION2, pub_id)

print("Connecting to broker",broker)
print(client.connect(broker))
client.loop_start()
print("Publishing")

is_on = False;

i=-1
while time.time()%60>0.001:
    continue
while True:
    curtime=time.time()%60
    if curtime>20+math.floor(i/2) and curtime<40-math.ceil(i/2):
        if not is_on:
            client.publish(topic, "turn__on")
            is_on=True
    if curtime%60>40-math.ceil(i/2):
        if is_on:
            client.publish(topic, "turn_off")
            is_on=False;
    if curtime%60<0.001:
        i+=1
        time.sleep(1)
        if i>30:
            i=0
            
            
client.disconnect()
client.loop_stop()
