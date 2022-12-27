import os
import struct
import array
import sys
from PIL import Image

def read_file(filename):
    with open(filename, "rb") as f:
        data = f.read()
    return data

def get_bytes(data):
    return array.array("B", data)

def convert_bytes_to_image(bytes, width, height):
    image = Image.new("L", (width, height))
    image.putdata(bytes)
    return image

def main():
    filename = "HeightMap.dat"
    data = read_file(filename)
    width = 4096
    height = 4096
    bytes = get_bytes(data)
    image = convert_bytes_to_image(bytes, width, height)
    image.save("heightmap.png", "PNG")

main()