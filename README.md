# ESP32 Edge Capture

## Overview
**ESP32 Edge Capture** is a tool designed to simplify image data collection from an ESP32 camera module for training and testing AI/ML models on Edge Impulse. The tool facilitates seamless communication between an ESP32 and a PC via USB, enabling users to capture and upload labeled image data efficiently.

## Features
- USB communication between ESP32 and PC.
- Web-based interface for ease of use.
- Supports both training and testing data collection.
- Real-time image capture and upload to Edge Impulse.

## Requirements
- **Hardware:**
  - DFRobot FireBeetle-2 ESP32-S3
  - USB Cable
- **Software:**
  - Python 3.x
  - Required Python Libraries (see `requirements.txt`)
  - Edge Impulse Account

## Installation
1. **Clone the Repository:**
   ```bash
   git clone https://github.com/MukeshSankhla/ESP32-Edge-Capture.git
   cd ESP32-Edge-Capture
   ```

2. **Install Dependencies:**
   ```bash
   pip3 install -r requirements.txt
   ```

3. **Upload ESP32 Code:**
   - Open the Arduino IDE.
   - Go to `File > Open` and select the code from the `ESP32_Code` folder.
   - Select the correct board (e.g., DFRobot FireBeetle-2 ESP32-S3) and COM port.
   - Click **Upload** to flash the code onto your ESP32.

## Usage
1. **Run the Web Interface:**
   ```bash
   python app.py
   ```
2. Open the web interface in your browser (default: `http://localhost:5000`).
3. Select the COM port, input your Edge Impulse API key, and choose between `Train` or `Test` mode.
4. Enter the label for your dataset.
5. Click `Capture and Upload` to start data collection.

## How It Works
- The PC sends a command to the ESP32 to capture an image.
- The ESP32 captures the image and sends it back to the PC.
- The PC uploads the image with the specified label to Edge Impulse.

## Contribution
Contributions are welcome! Please open an issue or submit a pull request.

## License
This project is licensed under the **MIT License**.

## Acknowledgments
Special thanks to the Edge Impulse team and the open-source community for their contributions.

---
**Author:** Mukesh Sankhla  
For any queries, feel free to reach out!

---
Happy Building! 🚀
