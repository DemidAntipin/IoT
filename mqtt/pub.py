import time
import paho.mqtt.client as mqtt_client
from uuid import getnode as get_mac
import hashlib
import serial
import numpy as np
import time

responses = {'p':1, 's':1}

arr = np.full(100, np.nan)
arrive_time = np.full(100, np.nan)

port_photo="COM4"

connection_photo = serial.Serial(port_photo, timeout=1)

def clean_messages(arr, arrive_time, age):
    current=time.time()
    to_delete=0
    for t in arrive_time:
        if t is np.nan:
            continue
        if current-t>=age:
            to_delete+=1
        else:
            break
    arr[:(100-to_delete)]=arr[to_delete:]
    arr[(100-to_delete):]=np.nan
    arrive_time[:(100-to_delete)]=arrive_time[to_delete:]
    arrive_time[(100-to_delete):]=np.nan
    return arr, arrive_time

def send_command(cmd: str, response_len: int, connection: serial.Serial) -> str:
    str_resp = None
    connection.write(cmd.encode())
    if response_len > 0:
        resp = connection.read(response_len)
        str_resp = resp.decode()
    return str_resp

broker="broker.emqx.io"

h=hashlib.new('sha256')
mac=get_mac()
h.update(str(mac).encode())
pub_id=h.hexdigest()[:10]
print(f"Listen me at id {pub_id}")

client=mqtt_client.Client(mqtt_client.CallbackAPIVersion.VERSION2, pub_id)

print("Connecting to broker",broker)
print(client.connect(broker))
client.loop_start()
print("Publishing")

instant=time.time()
average=time.time()
stream=time.time()
stream_start=time.time()
min_max_time=time.time()-5
stream_mode=False
min_val=256
max_val=0
while True:
    arr, arrive_time=clean_messages(arr, arrive_time, 2)
    photo_val_resp = send_command('p', responses['p'], connection_photo)
    try:
        photo_val_resp=ord(photo_val_resp)
        max_val=max(max_val,photo_val_resp)
        min_val=min(min_val,photo_val_resp)
    except:
        continue
    arr[:99]=arr[1:]
    arr[-1]=photo_val_resp
    arrive_time[:99]=arrive_time[1:]
    arrive_time[-1]=time.time()
    if (time.time()-min_max_time>=5):
        client.publish(f"lab/{pub_id}/photo/min", min_val)
        client.publish(f"lab/{pub_id}/photo/max", max_val)
        min_max_time=time.time()
    if (time.time()-instant>=1):
        client.publish(f"lab/{pub_id}/photo/instant", arr[-1])
        instant=time.time()
    if (time.time()-average>=1):
        client.publish(f"lab/{pub_id}/photo/average", np.nanmean(arr))
        average=time.time()
    if stream_mode and time.time()-stream>=0.1:
        client.publish(f"lab/{pub_id}/photo/stream", arr[-1])
        stream=time.time()
    if time.time()-stream_start>10:
        mode=send_command('s', responses['s'], connection_photo)
        if mode=='n':
            client.publish(f"lab/{pub_id}/photo/active_stream", 'on')
            stream_mode=True
        else:
            client.publish(f"lab/{pub_id}/photo/active_stream", 'off')
            stream_mode=False
        stream_start=time.time()
            
client.disconnect()
client.loop_stop()
