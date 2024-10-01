import serial
import time

responses = {'d':7, 'u':6, 'p':1}

port_led="COM4"
port_photo="COM5"

connection_led = serial.Serial(port_led, timeout=1)
connection_photo = serial.Serial(port_photo, timeout=1)

def send_command(cmd: str, response_len: int, connection: serial.Serial) -> str:
    str_resp = None
    connection.write(cmd.encode())
    if response_len > 0:
        # connection.in_waiting <-> available
        resp = connection.read(response_len)
        str_resp = resp.decode()
    return str_resp

while True:
    photo_val_resp = send_command('p', responses['p'], connection_photo)
    try:
        photo_val = ord(photo_val_resp)
    except:
        continue
    #resp=send_command('u', responses['u'], connection_led)
    #time.sleep(1)
    #resp=send_command('d', responses['d'], connection_led)
    #time.sleep(1)
    if photo_val > 300/4:
       resp=send_command('u', responses['u'], connection_led)
    else:
        resp=send_command('d', responses['d'], connection_led)
    #for command in responses:
    #    resp = send_command(command, responses[command])
    #    print(resp)
    #    time.sleep(1)
