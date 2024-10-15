import time
import paho.mqtt.client as mqtt_client
from uuid import getnode as get_mac
import hashlib
import serial

responses = {'p':1}

port_photo="COM3"

connection_photo = serial.Serial(port_photo, timeout=1)

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

while True:
    photo_val_resp = send_command('p', responses['p'], connection_photo)
    print(f"Publishing {photo_val_resp}")
    client.publish(f"lab/{pub_id}/led/state", photo_val_resp)
    time.sleep(0.1)

client.disconnect()
client.loop_stop()
