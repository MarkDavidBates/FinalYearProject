# code for setting up COM port: https://www.youtube.com/watch?v=AHr94RtMj1A&ab_channel=Von

import time
import requests
import serial.tools.list_ports

ports = serial.tools.list_ports.comports
serialInst = serial.Serial()

portList = []

for onePort in ports():
    portList.append(str(onePort))
    print(str(onePort))

val = input("select Port: COM")

portVar = "COM" + str(val)

print(portVar)

serialInst.baudrate = 9600
serialInst.port = portVar
serialInst.open()

while True:
    if serialInst.in_waiting:
        packet = serialInst.readline()
        print(packet)

        values = packet.decode("utf").split(",")

        print(values)

        URL = "https://api.thingspeak.com/update?api_key=<THINGSPEAK_API_KEY>=" + values[0] + "&field2=" + values[1] + "&field3=" + values[2]

        print(URL)

        response = requests.get(URL)

        
