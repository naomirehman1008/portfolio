import serial
import time
import sys
import glob
import serial
import cv2
import numpy as np
import struct
import time

#globals
baud_rate = 115200
IMAGE_WIDTH = 8
IMAGE_HIGHT = 8
PROC_IMAGE_WIDTH = IMAGE_WIDTH - 3 + 1
PROC_IMAGE_HIGHT = IMAGE_HIGHT - 3 + 1
DATA_WIDTH = 8

START_CHAR = 'Z'
STOP_CHAR = 'Z'

DISPLAY_WIDTH = 500
DISPLAY_HIGHT = 500

serial_port = 'COM4'

image_path = "C:\\Users\\naomi\\Documents\\Personal-Projects\\edge_detection_host\\black_and_white.jpg"

def send_image(serial_obj, image):
    #to change data size I'll need to do some bit masking fuckery
    unit8_image = np.int8(image)
    print("uint8 representation")
    print(unit8_image)
    bytes_image = np.ndarray.tobytes(unit8_image)
    print("bytes representation")
    print(bytes_image)

    start_char_bytes = bytes(START_CHAR, 'utf-8')
    stop_char_bytes = bytes(STOP_CHAR, 'utf-8')
    serial_obj.write(start_char_bytes)
    print(start_char_bytes)
    time.sleep(1)
    serial_obj.write(bytes_image)
    print(bytes_image)
    time.sleep(1)
    serial_obj.write(stop_char_bytes)
    print(stop_char_bytes)
    time.sleep(1)
    print("package sent")

def load_image(file_path):
    image = cv2.imread(file_path)
    if image.all() == None:
        err_message = f"Failed to load image from {file_path}. Try again? (y, n)\n"
        try_again = input(err_message)
        if (try_again == 'y'):
            # unimplemented
            print("invalid input, I'd ask you to try again, but I don't have gotos!!")
            exit()
        elif (try_again == 'n'):
            exit()
        else:
            print("invalid input, I'd ask you to try again, but I don't have gotos!!")
            exit()
    return image

def send_image_over_serial(serial_obj, packet):
    serial_obj.write(packet)
    try:
        serial_obj.write(packet)
        print("Packet sent successfully.")
    except serial.SerialException as e:
        print(f"Error: {e}")

def process_input_image(image):
    resized_image =cv2.resize(image, (IMAGE_WIDTH, IMAGE_HIGHT))
    grayscale_img = cv2.cvtColor(resized_image, cv2.COLOR_BGR2GRAY)
    grayscale_img = np.interp(grayscale_img, (grayscale_img.min(),grayscale_img.max()), (0, 127))
    return grayscale_img

def recieve_image_over_serial(serial_obj):
    image_bytes = serial_obj.read(PROC_IMAGE_HIGHT * PROC_IMAGE_WIDTH)
    return image_bytes

def reconstruct_image(image_bytes):
    rec_image = np.frombuffer(image_bytes, dtype=np.int8).reshape((PROC_IMAGE_WIDTH, PROC_IMAGE_HIGHT))
    return rec_image

def display_image_blocking(image, image_name):
    cv2.namedWindow(image_name, cv2.WINDOW_NORMAL)
    cv2.resizeWindow(image_name, DISPLAY_WIDTH, DISPLAY_HIGHT)
    cv2.imshow(image_name, image)
    cv2.waitKey(0)

if __name__ == '__main__':
    serial_obj = serial.Serial(serial_port, baud_rate, timeout=5)
    image = load_image(image_path)
    image = process_input_image(image)
    send_image(serial_obj, image)
    image_bytes = recieve_image_over_serial(serial_obj)
    if(image_bytes == ''):
        print("didn't get anythin :/")
    print(image_bytes)
    rec_image = reconstruct_image(image_bytes)
    display_image_blocking(rec_image, 'edges')

