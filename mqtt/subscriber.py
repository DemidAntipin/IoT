import serial
import time
import threading
import paho.mqtt.client as mqtt_client
import random
from uuid import getnode as get_mac
import hashlib
import numpy as np

broker = "broker.emqx.io"
pub_id=''

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

def await_pub_id(client, userdata, message):
    global pub_id
    data=str(message.payload.decode("utf-8"))
    pub_id=data

def on_message(client,userdata,message):
    if message.topic == f"lab/{pub_id}/photo/activate_stream":
        thread = threading.Thread(target=on_message_active_stream, args=(client, userdata, message))
        thread.start()
        return
    if message.topic == f"lab/{pub_id}/photo/min":
        thread = threading.Thread(target=on_message_min, args=(client, userdata, message))
        thread.start()
        return
    if message.topic == f"lab/{pub_id}/photo/max":
        thread = threading.Thread(target=on_message_max, args=(client, userdata, message))
        thread.start()
        return
    if message.topic in [f"lab/{pub_id}/photo/instant", f"lab/{pub_id}/photo/average", f"lab/{pub_id}/photo/stream"]:
        thread = threading.Thread(target=on_message_data, args=(client, userdata, message))
        thread.start()

client = mqtt_client.Client(
    mqtt_client.CallbackAPIVersion.VERSION2,
    sub_id
)

def on_message_data(client,userdata,message):
    global miin
    global maax
    global arraych
    data=str(message.payload.decode("utf-8"))
    last_message_times[message.topic]=time.time()
    print("recived message instant/average/stream - ", data)
    photo_val = float(data)
    arraych.append(photo_val)
    smoothed_data = np.convolve(arraych[-10:], np.ones(window_size)/window_size, mode='valid')
    inc,dec = split_trends(smoothed_data)
    if dec!=[]:
        if photo_val > (miin+maax)/2:
            print("aga")
            resp=send_command('u', responses['u'], connection_led)
        else:
            print("gggggggaga")
            resp=send_command('d', responses['d'], connection_led)
    else:
        resp=send_command('d', responses['d'], connection_led)
    return

def on_message_active_stream(client,userdata,message):
    global miin
    data=str(message.payload.decode("utf-8"))
    last_message_times[message.topic]=time.time()
    print("recived message active_stream- ", data)
    if data=="on":
            client.subscribe(f"lab/{pub_id}/photo/stream")
    elif data =="off":
        client.unsubscribe(f"lab/{pub_id}/photo/stream")
        del last_message_times[message.topic]
    return

def on_message_min(client,userdata,message):
    global miin
    data=str(message.payload.decode("utf-8"))
    last_message_times[message.topic]=time.time()
    print("recived message min- ", data)
    miin=float(data)
    return

def on_message_max(client,userdata,message):
    global maax
    data=str(message.payload.decode("utf-8"))
    last_message_times[message.topic]=time.time()
    print("recived message max- ", data)
    maax=float(data)
    return


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


responses = {'d':7, 'u':6}

port_led="COM4"


connection_led = serial.Serial(port_led, timeout=1)

def send_command(cmd: str, response_len: int, connection: serial.Serial) -> str:
    str_resp = None
    connection.write(cmd.encode())
    if response_len > 0:
        resp = connection.read(response_len)
        str_resp = resp.decode()
    return str_resp
confirm_sended=False
print("Connecting to broker",broker)
client.connect(broker)
client.loop_start()
print("Subscribing")
print("Awaiting pub_id")
client.subscribe(f"lab/+/get_pub_id")
client.on_message = await_pub_id
while pub_id=='':
    continue
print(f"pub_id successfully recieved {pub_id}")
client.publish(f"lab/{pub_id}/confirm", pub_id, qos=2)
print("sended confirmation")
client.on_message = on_message
client.subscribe(f"lab/{pub_id}/photo/#")
client.unsubscribe(f"lab/{pub_id}/photo/stream")
while True:
        if check_client_activity():
            time.sleep(10)
            continue
        else:
            break
resp=send_command('d', responses['d'], connection_led)
print("Publisher stopped sending messages")
print("Disconnecting from broker")
client.disconnect()
client.loop_stop()


