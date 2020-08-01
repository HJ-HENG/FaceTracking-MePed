# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Purpose of Program~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# This code is intended for users to be able to tell a device (connected via USB) the distance a detected face is, from
# the center of the camera. Some comments made are created to favor simplicity over accuracy.
# This code is heavily inspired by the project given in this website:
#
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~IMPORTANT NOTES~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# Make sure the following are installed before running the program:
# >>> pip install serial
# >>> pip install pyserial
# >>> pip install opencv-python
# >>> pip install numpy
# Also make sure that the communication port is unoccupied when this code is running, errors will pop out otherwise.
# The program is configured to cameras recording with aspect ratios of 4:3 (480p)
#
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Author's Information~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
# @Author             : Harsh Dethe
# @Date               : 11/10/2018
# @Last Modified By   : Heng Hao Jun
# @Last Modified Date : 1/08/2020
# @Availability       : https://create.arduino.cc/projecthub/WolfxPac/face-tracking-using-arduino-b35b6b
#
# ========================================================Imports=======================================================

import serial  # To write serial data to the device connected via usb
import time    # Pauses the code during run time for a specified amount of time
import cv2     # Software used for the live video feed and face detection 

# =======================================================CONSTANTS======================================================

COMMUNICATION_PORT = 'COM4'                 # Modify according to the USB port the target device is connected to
CAMERA_OF_CHOICE = 1                        # Use 0 to use the computer's primary/default camera
COLOR_OF_RETICLE = (0, 223, 255)            # The color of the reticle in the center of the screen in BGR
COLOR_OF_BOX = (0, 255, 0)                  # The color of the rectangle which will surround the detected face
THICKNESS_OF_BOX = 3                        # The thickness of the rectangle which will surround the detected face
CLASSIFIER_THRESHOLD = 1.2                  # How strict the face identifier should behave (the higher the stricter)
BAUDRATE_OF_GIVEN_DEVICE = 9600             # Make sure the baudrate given matches the baudrate of the device.
FACE_DISPLACEMENT_THRESHOLD = 30            # Threshold displacement of the face from the camera
WINDOW_LENGTH = 600
MIDPOINT_OF_WINDOW = (WINDOW_LENGTH // 2, WINDOW_LENGTH // 2)


# =======================================================Functions======================================================

# Establishes a connection to the device specified in the first parameter.
#
# @param    communication_port - The communication port that the device is connected to (Check device manager)
# @returns                     - The object that allows user to read and write data to and from
def connect_to_device(communication_port: str):
    device_connected = serial.Serial(communication_port, BAUDRATE_OF_GIVEN_DEVICE)
    time.sleep(2)
    print("Device is connected")
    return device_connected


# ==========================================================Code========================================================

# Creates an object to be used to write data to the device
device = connect_to_device(COMMUNICATION_PORT)
# Opens camera to detect faces using the haarcascade classifier 
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')  # Detects faces (pre-trained classifier)
cap = cv2.VideoCapture(CAMERA_OF_CHOICE)
# Creates the window to display the window feed
cv2.namedWindow('image', cv2.WINDOW_FULLSCREEN)
cv2.resizeWindow('image', WINDOW_LENGTH, WINDOW_LENGTH)

# Runs infinitely until the 'ESC' key is pressed
while True:
    # Reads image, identifies the faces and and draws the center of the camera on the video feed
    retrieve_val, image = cap.read()
    cv2.circle(image, MIDPOINT_OF_WINDOW, 3, COLOR_OF_RETICLE, -1)  # 3rd argument:Radius   ||   5th argument: Thickness 
    color = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)                 # Changes the color space to grayscale (requirement)
    faces = face_cascade.detectMultiScale(color, CLASSIFIER_THRESHOLD)

    # Draws rectangles over the detected faces
    for (coor_x, coor_y, width, height) in faces:
        cv2.rectangle(image, (coor_x, coor_y), (coor_x + width, coor_y + height), COLOR_OF_BOX, THICKNESS_OF_BOX)
        # The center of the face detected
        center_x = int(coor_x + (coor_x + height)) / 2
        center_y = int(coor_y + (coor_y + width)) / 2
        center_of_face = (center_x, center_y)
        # Distance from the midpoint of the window
        data1 = int(center_x - MIDPOINT_OF_WINDOW[0])
        data2 = int(center_y - MIDPOINT_OF_WINDOW[1])
        data = "X{x_coor}Y{y_coor}".format(x_coor=data1, y_coor=data2)
        # ~~~~~~~~~~~~~~~~~~~~~~~~~~Writes Data~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if (abs(data1) > FACE_DISPLACEMENT_THRESHOLD) or (abs(data2) > FACE_DISPLACEMENT_THRESHOLD):
            device.write(data.encode())
        # ~~~~~~~~~~~~~~~~~~~~~~~~~~Prints Data~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        # print("Output:")
        # print("Coordinates = " + data)

    # Displays the video capture along with the drawn rectangles for detected faces
    cv2.imshow('image', image)

    # Exits the program when the 'ESC' key is pressed (27 represents the ASCII code for the 'ESC' key)
    k = cv2.waitKey(30)
    if k == 27:
        break