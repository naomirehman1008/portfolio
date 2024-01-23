import numpy as np

conv_filter = np.array([[0, 1, 0], [1, -4, 1], [0, 1, 0]])
image_path = "C:\\Users\\naomi\\Documents\\Personal-Projects\\edge_detection_host\\black_and_white.jpg"

"""
for i in range(10):
    input = np.floor(np.random.rand(3,3) * 255)
    output = np.dot(input, conv_filter)
    print(input)
    print(output)


"""
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

if __name__ == '__main__':


