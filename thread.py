import threading
import serial
import numpy as np
import time
import socket
from queue import Queue

def ReceiverThread():
    # Code from pickingUp.py
    serial_port = '/dev/cu.usbmodem1101'
    baud_rate = 115200
    arduino = serial.Serial(serial_port, baud_rate)  # Open serial connection
    # Constants for angle conversion
    deg_to_rad = np.pi / 180
    #rad_to_deg = 1 / deg_to_rad
    time.sleep(1)   # wait 1 second for the connection to stabilize

    while True:
        # Wait until data is available in the serial buffer
        while arduino.inWaiting() == 0:
            pass

        # Read and process the incoming serial data
        data_packet = arduino.readline()  # Read a line of data from the serial port
        data_packet = str(data_packet, 'utf-8')  # Convert the byte data to a string
        split_packet = data_packet.split(",")  # Split the string into roll, pitch, yaw values

        # Convert the string data to radians
        roll = float(split_packet[0]) * deg_to_rad
        pitch = float(split_packet[1]) * deg_to_rad
        yaw = float(split_packet[2]) * deg_to_rad + np.pi

        # enqueue roll, pitch, and yaw
        receiverQueue.put(roll)
        receiverQueue.put(pitch)
        receiverQueue.put(yaw)



def SenderThread():
    # Set up the TCP server
    HOST = '127.0.0.1'  # Localhost
    PORT = 10200

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((HOST, PORT))
    server_socket.listen()
    print("Waiting for connection...")
    conn, addr = server_socket.accept()
    print(f"Connected by {addr}")


    try:
        while True:
            # read in data from receiver data queue
            roll = receiverQueue.get()
            pitch = receiverQueue.get()
            yaw = receiverQueue.get()

            # Send the data as a formatted string
            data = f"{roll},{pitch},{yaw}\n"

            try:
                    conn.sendall(data.encode())
                    
            except BrokenPipeError:
                print("Client disconnected. Waiting for reconnection...")
                break  # Exit the inner loop to wait for a new connection

            time.sleep(0.1) # in second, for sampling rate
    finally:
        conn.close()
        server_socket.close()


# Set up a queue for receiving data    
receiverQueue = Queue()
# connect the receiving thread to its function
thread1 = threading.Thread(target=ReceiverThread)
# start the thread
thread1.start()

# Set up a queue for sending data to TCP client
thread2 = threading.Thread(target=SenderThread)
# start the thread
thread2.start()