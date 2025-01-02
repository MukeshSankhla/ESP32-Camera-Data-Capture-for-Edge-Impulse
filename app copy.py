from flask import Flask, render_template, request, jsonify, send_from_directory
import os
import requests
import serial
import serial.tools.list_ports
import time
from io import BytesIO
from PIL import Image

app = Flask(__name__)

def get_com_ports():
    return [port.device for port in serial.tools.list_ports.comports()]

# Set the directory to store images
image_dir = "captured_images"
if not os.path.exists(image_dir):
    os.makedirs(image_dir)

@app.route('/')
def home():
    return render_template('index.html', com_ports=get_com_ports())

@app.route('/capture', methods=['POST'])
def capture():
    try:
        # Get inputs from the form
        com_port = request.form['com_port']
        api_key = request.form['api_key']
        label = request.form['label']
        
        print(f"Received label: {label}")
        print(f"Using com port: {com_port}")

        # Initialize serial connection
        ser = serial.Serial(com_port, 921600, timeout=10)
        time.sleep(2)  # Wait for connection to stabilize
        
        # Wait for ready signal
        while ser.in_waiting:
            ser.readline()
        
        # Send capture command
        ser.write(b'CAPTURE\n')
        
        # Read image size
        size_line = ser.readline().decode().strip()
        print(f"Received size data: '{size_line}'")
        
        if not size_line.isdigit():
            raise ValueError(f"Invalid image size: {size_line}")
        
        size = int(size_line)
        
        # Read image data
        image_data = ser.read(size)
        
        # Close serial connection
        ser.close()
        
        # Save image temporarily
        img = Image.open(BytesIO(image_data))
        img_io = BytesIO()
        img.save(img_io, 'JPEG')
        img_io.seek(0)
        
        filename = "captured_image.jpg"
        filepath = os.path.join(image_dir, filename)
        
        # Save the image to the fixed filename
        with open(filepath, 'wb') as f:
            f.write(img_io.getvalue())
        
        # Prepare the URL for the image (served from static directory)
        image_url = f"/static/{filename}"

        # Upload image to Edge Impulse
        headers = {
            'x-api-key': api_key,  # Edge Impulse API key
            'x-label': label,      # Image label
        }

        with open(filepath, 'rb') as img_file:
            print(f"Uploading {filename} to Edge Impulse")
            response = requests.post(
                'https://ingestion.edgeimpulse.com/api/training/files',
                headers=headers,
                files={'data': (filename, img_file, 'image/jpeg')}
            )

        # Log the response for debugging
        print(f"Response Status: {response.status_code}")
        print(f"Response Body: {response.text}")
        
        # Check response status and return appropriate message
        if response.status_code == 200:
            return jsonify({
                'status': 'success',
                'message': 'Image captured and uploaded successfully',
                'response': response.json() if response.text else {},
                'image_url': image_url  # Return the image URL
            })
        else:
            return jsonify({
                'status': 'error',
                'message': f'Upload failed: {response.text}'
            })

    except Exception as e:
        return jsonify({
            'status': 'error',
            'message': str(e)
        })

# Static route to serve the images
@app.route('/static/<filename>')
def serve_image(filename):
    return send_from_directory(image_dir, filename)

if __name__ == '__main__':
    app.run(debug=True)
