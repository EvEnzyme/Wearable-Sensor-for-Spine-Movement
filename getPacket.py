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

# How many data points to record
kmax = 200;

# Loop through and collect data as it is available
for k in range(kmax):
    try:
        # Read the line
        s_bytes = arduinoData.readline()
        decoded_bytes = s_bytes.decode("utf-8").strip('\r\n')
        print(decoded_bytes)

        # Parse the line
        if k == 0:
            values = decoded_bytes.split(",")
        else:
            values = [float(x) for x in decoded_bytes.split()]
        print(values)

        # Write to CSV
        writer = csv.writer(f,delimiter=",")
        writer.writerow(values)

    except:
        print("Error encountered, line was not recorded.")

f.close()

