import cv2
import numpy as np
import sys
from PIL import Image

if len(sys.argv) != 2:
    print("Usage: python script.py <image_path>")
    sys.exit(1)

image_path = sys.argv[1]

# Try to load the image with OpenCV first
image = cv2.imread(image_path, cv2.IMREAD_UNCHANGED)

if image is None:
    print(f"Error loading image {image_path} with OpenCV.")
    sys.exit(1)

# Get OpenCV Image Info
height, width = image.shape[:2]
channels = image.shape[2] if len(image.shape) == 3 else 1
depth = image.dtype
image_size = image.nbytes

# Try to load the image with PIL for additional information
try:
    pil_image = Image.open(image_path)
    mode = pil_image.mode
    format = pil_image.format
except Exception as e:
    print(f"Error loading image with PIL: {e}")
    mode = "N/A"
    format = "N/A"

# Print out the information
print(f"Image Path: {image_path}")
print(f"Width: {width} pixels")
print(f"Height: {height} pixels")
print(f"Channels: {channels} (e.g., 3 for RGB, 4 for RGBA, 1 for grayscale)")
print(f"Depth: {depth} (e.g., uint8, float32, etc.)")
print(f"Image Size: {image_size / 1024:.2f} KB")
print(f"Mode (PIL): {mode}")
print(f"Format (PIL): {format}")
