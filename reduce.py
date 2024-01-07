import struct
import sys
import tensorflow as tf
import numpy as np
from tensorflow.keras.models import load_model

def create_output(input_name, output_name, autoencoder):
    # Read the input file that will create the x image dataset
    with open(input_name, 'rb') as f:
        # Read the header information
        magic_number = int.from_bytes(f.read(4), 'big')
        num_images = int.from_bytes(f.read(4), 'big')
        num_rows = int.from_bytes(f.read(4), 'big')
        num_cols = int.from_bytes(f.read(4), 'big')
        
        # Read the image data
        image_data = f.read()
    
    # Convert binary data to numpy array
    x = np.frombuffer(image_data, dtype=np.uint8)
    x = x.reshape(num_images, num_rows, num_cols)
    x = x.astype('float32') / 255.0  # Normalize the input data
    
    # Apply the autoencoder model to the input data
    output_imgs = autoencoder.encoder.predict(x)
    output_imgs = np.clip(output_imgs * 255, 0, 255).astype(np.uint8)  # Ensure values are within [0, 255]
    
    # Writing data as a binary file with big endian architecture
    with open(output_name, 'wb+') as images_file:
        # Writing metadata
        images_file.write(struct.pack('>IIII', magic_number, num_images, num_rows, num_cols))
        # Writing image data
        for image in output_imgs:
            images_file.write(image.tobytes())

if(len(sys.argv) < 9) :
    print('Please run again providing arguments in wanted format!')
    exit(1)

# Load the saved model
loaded_autoencoder = load_model('autoencoder', compile=False)
create_output(sys.argv[2],sys.argv[6],loaded_autoencoder)
create_output(sys.argv[4],sys.argv[8],loaded_autoencoder)
