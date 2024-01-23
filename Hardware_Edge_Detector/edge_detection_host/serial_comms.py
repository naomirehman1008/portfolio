import serial
import time
import sys
import glob
import serial
import cv2
import numpy as np
import struct

# implementation note: USB is half duplex so processing a video is not practical.

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

# from https://stackoverflow.com/questions/12090503/listing-available-com-ports-with-python
def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

def scan_serial_ports():
    ports = serial_ports()
    if not ports:
        print("No serial ports found.")
        return 0

    print("Available serial ports:")
    for port in ports:
        print(port)
    
    serial_port = input('please select a port\n').strip()
    while(serial_port not in ports):
        serial_port = input("invalid serial port, please try again!\n").strip()

    return serial_port

def take_picture():
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error opening camera")
        exit()
    ret, image = cap.read()
    if(ret):
        print("Image captured successfully")
        return image
    else:
        try_again = input("Failed to cature image. Try again? (y, n)\n")
        if(try_again == 'y'):
            #UNIMPLEMENTED BC NO GOTO!!
            exit()
        elif (try_again == 'n'):
            exit()
        else:
            print("invalid input, I'd ask you to try again, but I don't have gotos!!\n")
            exit()

def load_image(file_path):
    image = cv2.imread(file_path)
    if not image:
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

def process_input_image(image):
    resized_image =cv2.resize(image, (IMAGE_WIDTH, IMAGE_HIGHT))
    grayscale_img = cv2.cvtColor(resized_image, cv2.COLOR_BGR2GRAY)
    grayscale_img = np.interp(grayscale_img, (grayscale_img.min(),grayscale_img.max()), (0, 127))
    return grayscale_img

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
    serial_obj.write(bytes_image)
    print(bytes_image)
    serial_obj.write(stop_char_bytes)
    print(stop_char_bytes)
    print("package sent")

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


#MAIN LOOP
if __name__ == '__main__':
    
    # scan for and select serial port
    serial_port = scan_serial_ports()
    if(not serial_port):
        print("no serial ports found. try plugging in the board.")
        exit()
    serial_obj = serial.Serial(serial_port, baud_rate, timeout=5)
    
    # select an image
    input_mode = input("Please enter 'camera' to take a picture, or a path to an image\n")
    if(input_mode == 'camera'):
        input_image = take_picture()
    else:
        input_image = load_image(input_mode)
    
    # pre process image
    processed_image = process_input_image(input_image)

    # display image
    print(processed_image)
    """
    cv2.namedWindow('Grayscale Image', cv2.WINDOW_NORMAL)
    cv2.resizeWindow('Grayscale Image', DISPLAY_WIDTH, DISPLAY_HIGHT)
    cv2.imshow('original', input_image)
    cv2.imshow('Grayscale Image', processed_image)
    # Check for the 'q' key to exit the loop and close the window
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    """
    # prepare image to send
    send_image(serial_obj, processed_image)

    # receive processed image bytes object from fpga
    convolved_image_bytes = recieve_image_over_serial(serial_obj)
    print(convolved_image_bytes)
    # reconstruct image from bytes object
    rec_image = reconstruct_image(convolved_image_bytes)
    display_image_blocking(rec_image, 'edges')
    #display image
    print("finished")
