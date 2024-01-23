#### Hardware Edge Detector

This implements edge detection with a Laplace kernel on an FPGA. The host computer preprocesses an image and sends it to the FPGA over usb. The FPGA performs the convolution and sends back the processed image. The host computer then displays the image.
