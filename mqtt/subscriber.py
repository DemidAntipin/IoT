import serial
import time
import paho.mqtt.client as mqtt_client
import random
from uuid import getnode as get_mac
import hashlib

broker = "broker.emqx.io"
##pub_id="bc27bab771"
pub_id="784b020c14"

if not pub_id:
    raise RuntimeError("Publisher is not define")
h=hashlib.new('sha256')
mac=get_mac()
h.update(str(mac).encode())
sub_id = h.hexdigest()[10:20]

INACTIVITY_TIMEOUT = 20
last_message_times = {}

def check_client_activity():
    curtime = time.time()
    for client, last_message_time in last_message_times.items():
        if curtime - last_message_time >= INACTIVITY_TIMEOUT:
            return False
    return True

def on_message(client,userdata,message):
    data=str(message.payload.decode("utf-8"))
    last_message_times[message.topic]=time.time()
    print("recived message - ", data)
    try:
        photo_val = ord(data)
    except:
        return
    if photo_val > light_limit:
        resp=send_command('u', responses['u'], connection_led)
    else:
        resp=send_command('d', responses['d'], connection_led)

client = mqtt_client.Client(
    mqtt_client.CallbackAPIVersion.VERSION2,
    sub_id
)



responses = {'d':7, 'u':6, 'p':1}

light_limit = 50

port_led="COM4"


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
client.subscribe(f"lab/{pub_id}/led/state")
while True:
        if check_client_activity():
            time.sleep(10)
            continue
        else:
            break
resp=send_command('d', responses['d'], connection_led)
client.disconnect()
client.loop_stop()
