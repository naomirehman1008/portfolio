# Hardware Edge Detector

This project implements edge detection with a Laplace kernel on an FPGA. The host computer preprocesses an image and sends it to the FPGA over usb. The FPGA performs the convolution and sends the processed image back to the host computer, which then displays the image. I created this for the CruzHacks 2024 hackathon.

## System Explaination
#### Host Computer
The host computer can take an image as a path or using a webcam. This image is greyscaled and resized, then sent to the FPGA using PySerial. After the FPGA processes the image the host recieves the image and displays it. serial_comms.py contains the program for the host computer.
#### FPGA Side
This project uses the Basys3 FPGA dev board. It has a USB-UART bridge, which was used for communications. I used FPGADude's implementation Verilog implementation of a UART driver (https://github.com/FPGADude/Digital-Design/tree/main/FPGA%20Projects/UART). The fpga side consists of three main components: UART to image, the convolution unit, and image to UART.
