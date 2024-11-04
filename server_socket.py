import socket
import time

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
        # dummy data
        roll = 1.0
        pitch = 2.0
        yaw = 3.0

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
