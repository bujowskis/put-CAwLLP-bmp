# put-CAwLLP-bmp
A second project for the PUT Computer Architecture with Low-Level Programming - parsing BMP files

Task - write a C program to parse BMP files; Requirements:
- (3.0) program should accept one command line parameter in the format `./program PATH-TO-BMP-FILE`, and print out parsed values from the file header
- (3.5) additionally, program must print out parsed values from the info header
- (4.0) program prints out a histogram of RGB colors
- (4.5) program handles case when started with two command line parameters in the format `./program PATH-TO-INPUT-BMP-FILE PATH-TO-OUTPUT-BMP-FILE`; the output BMP file should have exactly the same values as the input BMP, but the pixel array should be modified to make the image **greyscale** using the simplest formula gray = (r + g + b) / 3
- (5.0) program uses steganography to hide a text inside BMP pixel array, input in the format `./program PATH-TO-INPUT-BMP PATH-TO-ENCODED-BMP "text to be hidden"`; the program should also ask the user if they want the message to be decoded from the output
