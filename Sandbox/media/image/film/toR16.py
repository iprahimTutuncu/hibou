import cv2
import numpy as np
import sys

if len(sys.argv) != 3:
    print("Usage: python script.py <input_image> <output_image>")
    sys.exit(1)

image_path = sys.argv[1]
output_path = sys.argv[2]

# Load the image as a float32 image (RGB32F)
image = cv2.imread(image_path, cv2.IMREAD_UNCHANGED)

if image is None:
    print(f"Error loading image {image_path}")
    sys.exit(1)

# Ensure the image is in float32 format
if image.dtype != np.float32:
    print("Converting image to float32 format")
    image = image.astype(np.float32)

# Extract only the Red channel (R) and discard the rest
red_channel = image[:, :, 0]  # Red channel is at index 0 for RGB images

# Normalize the red channel to the range [0, 1] if it's not already in this range
# This step might not be necessary if your image is already in the range [0, 1].
red_channel = cv2.normalize(red_channel, None, 0.0, 1.0, cv2.NORM_MINMAX)

# Convert the red channel to 16-bit integer by scaling the float32 values to [0, 65535]
red_channel_int16 = np.uint16(red_channel * 65535)

# Save the red channel as a 16-bit PNG image
cv2.imwrite(output_path, red_channel_int16)

print(f"Red channel successfully saved as 16-bit PNG: {output_path}")
