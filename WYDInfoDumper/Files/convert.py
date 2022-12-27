import os
import struct
import array

def read_bytes(filename):
    with open(filename, "rb") as f:
        while True:
            byte = f.read(1)
            if not byte:
                break
            yield byte

def write_bytes(filename, bytes):
    with open(filename, "wb") as f:
        for b in bytes:
            f.write(struct.pack("B", b))

def convert_to_image(filename):
    bytes = read_bytes(filename)

    # Create an array of unsigned 8-bit integers.
    data = array.array("B", bytes)

    # Get the width (4096) and height (4096) from the array.
    width = data[0]
    height = data[1]

    # Get the rest of the data in the array.
    pixels = data[2:]

    # Convert the pixels to a list of tuples (R, G, B)
    pixels = [(r, g, b) for r, g, b in zip(pixels[::3], pixels[1::3], pixels[2::3])]

    # Create a new file name based on the original file name.
    new_filename = os.path.splitext(filename)[0] + ".bmp"

    # Write the pixels as a bitmap file.
    write_bytes(new_filename, pixels)

if __name__ == "__main__":
    convert_to_image("HeightMap.dat")