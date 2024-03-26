from serial.tools import list_ports
import serial
import time
import csv

# find out which com port has arduino connected
ports = list_ports.comports()
for port in ports: 
    print(port)

f = open("data.csv", "w", newline='')
f.truncate()

arduinoData = serial.Serial('/dev/cu.usbmodem1101', 115200)

# Reset arduino
arduinoData.setDTR(False)
time.sleep(1)
arduinoData.flushInput()
arduinoData.setDTR(True)


while (True):
    # while (arduinoData.in_waiting == 0):
    #     pass

    try: 
        # read a line of data
        rawDataPacket = arduinoData.readline()
        
        dataPacket = rawDataPacket.decode("utf-8").strip('\r\n')
        print(dataPacket)
    
    except:
        print("error reported")

