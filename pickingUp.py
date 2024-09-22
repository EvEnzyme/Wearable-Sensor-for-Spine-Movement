from vpython import *
from time import *
import numpy as np
import math
import serial

# Setup serial connection to the Arduino (or other microcontroller)
serial_port = '/dev/cu.usbmodem101'
baud_rate = 115200
arduino = serial.Serial(serial_port, baud_rate)  # Open serial connection
sleep(1)  # Wait for the connection to stabilize

# Scene setup for the VPython window
scene.range = 5  # Set the scene's range to 5 units
scene.width = 600  # Set the scene window's width
scene.height = 600  # Set the scene window's height
scene.forward = vector(-1, -1, -1)  # Set initial camera view direction

# Constants for angle conversion
deg_to_rad = np.pi / 180
rad_to_deg = 1 / deg_to_rad

# Fixed arrows representing the global X, Y, and Z axes
x_axis_arrow = arrow(length=4, shaftwidth=0.1, color=color.white, axis=vector(1, 0, 0))
y_axis_arrow = arrow(length=4, shaftwidth=0.1, color=color.white, axis=vector(0, 1, 0))
z_axis_arrow = arrow(length=4, shaftwidth=0.1, color=color.white, axis=vector(0, 0, 1))

# Arrows representing the orientation of the object
front_arrow = arrow(length=4, shaftwidth=0.1, color=color.purple, axis=vector(1, 0, 0))
up_arrow = arrow(length=1, shaftwidth=0.1, color=color.magenta, axis=vector(0, 1, 0))
side_arrow = arrow(length=2, shaftwidth=0.1, color=color.orange, axis=vector(0, 0, 1))

sensor_object = box(length=1, width=0.75, height=0.1, pos=vector(0, 0, 0), color=color.blue)

# Main loop to continuously read data from the IMU and update the 3D visualization
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
    yaw = float(split_packet[2]) * deg_to_rad + np.pi  # Adjust yaw by π for proper alignment

    # Print the current orientation angles (in degrees) for debugging
    print(f"Roll = {roll * rad_to_deg:.2f}°, Pitch = {pitch * rad_to_deg:.2f}°, Yaw = {yaw * rad_to_deg:.2f}°")

    rate(50)  # Control the update rate (50 frames per second)

    # Calculate direction vectors based on the yaw, pitch, and roll angles
    forward_vector = vector(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch))  # Forward direction
    global_up_vector = vector(0, 1, 0)  # The global Y-axis (up direction)
    side_vector = cross(forward_vector, global_up_vector)  # Side direction (perpendicular to forward and up)
    adjusted_up_vector = cross(side_vector, forward_vector)  # Adjusted up direction (perpendicular to forward and side)

    # Update the orientation of the arrows representing the object's direction
    front_arrow.axis = forward_vector
    side_arrow.axis = side_vector
    up_arrow.axis = adjusted_up_vector

    # Update the orientation of the object itself
    sensor_object.axis = forward_vector
    sensor_object.up = adjusted_up_vector

    # Keep the arrow lengths consistent
    side_arrow.length = 2
    front_arrow.length = 4
    up_arrow.length = 1
