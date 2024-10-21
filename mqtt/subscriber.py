import serial
import time
import paho.mqtt.client as mqtt_client
import random
from uuid import getnode as get_mac
import hashlib
import numpy as np

broker = "broker.emqx.io"
##pub_id="bc27bab771"
pub_id="3e29f1e565"

if not pub_id:
    raise RuntimeError("Publisher is not define")
h=hashlib.new('sha256')
mac=get_mac()
h.update(str(mac).encode())
sub_id = h.hexdigest()[10:20]

INACTIVITY_TIMEOUT = 20
last_message_times = {}

arraych=[]
window_size = 3

global miin
miin=0
global maax
maax=0
def check_client_activity():
    curtime = time.time()
    for client, last_message_time in last_message_times.items():
        if curtime - last_message_time >= INACTIVITY_TIMEOUT:
            return False
    return True

def on_message(client,userdata,message):
    global miin
    global maax
    data=str(message.payload.decode("utf-8"))
    last_message_times[message.topic]=time.time()
    print("recived message - ", data)
    if message.topic==f"lab/{pub_id}/photo/activate_stream":
        if data=="on":
            client.subscribe(f"lab/{pub_id}/photo/stream")
        elif data =="off":
            client.unsubscribe(f"lab/{pub_id}/photo/stream")
            del last_message_times[message.topic]
        return
    if message.topic ==(f"lab/{pub_id}/photo/min"):
        miin=data
        return
    if message.topic ==(f"lab/{pub_id}/photo/max"):
        maax=data
        return
    
    if message.topic in [f"lab/{pub_id}/photo/instant",f"lab/{pub_id}/photo/average",f"lab/{pub_id}/photo/stream"]:
        photo_val = float(data)
        arraych.append(photo_val)
        smoothed_data = np.convolve(arraych, np.ones(window_size)/window_size, mode='valid')
        inc,dec = split_trends(smoothed_data)
        if dec!=[]:
            if photo_val > (miin+maax)/2:
                resp=send_command('u', responses['u'], connection_led)
            else:
                resp=send_command('d', responses['d'], connection_led)
        else:
            resp=send_command('d', responses['d'], connection_led)

client = mqtt_client.Client(
    mqtt_client.CallbackAPIVersion.VERSION2,
    sub_id
)


def split_trends(data):
    increasing = []
    decreasing = []
    
    for i in range(1, len(data)):
        if data[i] > data[i - 1]:
            increasing.append(data[i])
            if decreasing:
                decreasing.clear()
        elif data[i] < data[i - 1]:
            decreasing.append(data[i])
            if increasing:
                increasing.clear()
    
    return increasing, decreasing


responses = {'d':7, 'u':6, 'p':1}

port_led="COM3"


connection_led = serial.Serial(port_led, timeout=1)

def send_command(cmd: str, response_len: int, connection: serial.Serial) -> str:
    str_resp = None
    connection.write(cmd.encode())
    if response_len > 0:
        resp = connection.read(response_len)
        str_resp = resp.decode()
    return str_resp
client.on_message = on_message
print("Connecting to broker",broker)
client.connect(broker)
client.loop_start()
print("Subscribing")
client.subscribe(f"lab/{pub_id}/photo/instant")
while True:
        if check_client_activity():
            time.sleep(10)
            continue
        else:
            break
resp=send_command('d', responses['d'], connection_led)
client.disconnect()
client.loop_stop()


