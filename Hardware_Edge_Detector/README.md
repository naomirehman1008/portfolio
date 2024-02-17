# Hardware Edge Detector
This project implements edge detection with a Laplace kernel on an FPGA. Convolutions are a the main operation in a CNN so I created this as a proof of concept for running a CNN with an FPGA. I chose the Laplace kernel because all of the entries are powers of two, which allowed me to use bit shifting instead of addition. The actual convolution operation is increadibly fast on the FPGA. The entire image is processed in one cycle, as opposed to about 600 operations on a CPU for an 8x8 image. I used USB and UART because that's what the Basys3 has, but if possible I would have used PCIe for transfering data to and from the FPGA. 

## System Explaination
The host computer preprocesses an image and sends it to the FPGA over USB. The FPGA performs the convolution and sends the processed image back to the host computer, which then displays the image.

#### Host Computer
The host computer can take an image as a path or using a webcam. This image is greyscaled and resized, then sent to the FPGA using PySerial. After the FPGA processes the image the host recieves the image and displays it. serial_comms.py contains the program for the host computer.

#### FPGA Side
This project uses the Basys3 FPGA dev board. It has a USB-UART bridge, which was used for communications. I used FPGADude's implementation Verilog implementation of a UART driver (https://github.com/FPGADude/Digital-Design/tree/main/FPGA%20Projects/UART). The fpga side consists of three main components: UART to image, the convolution unit, and image to UART.

## How to use
The edge_detection_vivado directory contains everything to run the program in Vivado. Open the edge_detection.xpr to open the project and generate a bitstream for the Basys 3 board. The edge_detection_host directory contains the code for the host. serial_comms.py runs the program that allows the user to choose a comm port, import an image or take a picture, and recieve the processed image. serial_test.py automatically connects to COM4 and sends a black and white image.
