from flask import Flask, render_template, request, jsonify, send_from_directory
import os
import requests
import serial
import serial.tools.list_ports
import time
from io import BytesIO
from PIL import Image
import json

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
        mode = request.form['mode']  # 'training' or 'testing'
        
        print(f"Mode: {mode}")
        print(f"Received label: {label}")
        print(f"Using com port: {com_port}")

        # Determine the upload URL based on mode
        upload_url = 'https://ingestion.edgeimpulse.com/api/training/files' if mode == 'training' \
            else 'https://ingestion.edgeimpulse.com/api/testing/files'
        
        print(f"Upload URL: {upload_url}")

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

        # Upload image to Edge Impulse with metadata
        headers = {
            'x-api-key': api_key,
            'x-add-date-id': '1',  # Ensures unique uploads if necessary
        }

        # Prepare metadata (with bounding boxes if needed)
        metadata = {
            "version": 1,
            "type": "bounding-box-labels",
            "boundingBoxes": {
                filename: [
                    {
                        "label": label,
                        "x": 0,  # Example values, replace with real bounding box coordinates
                        "y": 0,
                        "width": 640,
                        "height": 480
                    }
                ]
            }
        }
        bbox_label = json.dumps(metadata, separators=(',', ':'))

        # Payload with metadata
        files = [
            ('data', (filename, open(filepath, 'rb'), 'image/jpeg')),
            ('data', ('bounding_boxes.labels', bbox_label))
        ]

        print(f"Uploading {filename} to Edge Impulse with label '{label}'")
        response = requests.post(
            upload_url,
            headers=headers,
            files=files
        )

        # Log the response for debugging
        print(f"Response Status: {response.status_code}")
        print(f"Response Body: {response.text}")
        
        # Check response status and return appropriate message
        if response.status_code == 200:
            return jsonify({
                'status': 'success',
                'message': f'Image uploaded successfully to {mode} mode',
                'response': response.json() if response.text else {},
                'image_url': image_url  # Return the image URL
            })
        else:
            return jsonify({
                'status': 'error',
                'message': f'Upload failed: {response.text}'
            })

    except Exception as e:
        print(f"Error: {str(e)}")
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
