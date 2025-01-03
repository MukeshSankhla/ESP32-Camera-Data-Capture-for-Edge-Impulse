ESP32-Edge-Capture

ESP32-Edge-Capture is a tool designed to streamline data collection from an ESP32 camera module using a USB connection and integrate it seamlessly with Edge Impulse for machine learning projects. The tool provides an intuitive web interface to manage image capture and upload workflows for training and testing AI models.

📚 Overview

This project enables users to capture images using an ESP32 camera module and upload them directly to the Edge Impulse platform via a user-friendly Python-based web interface.

Key Features:

📸 Real-time Image Capture: Capture images using the ESP32 camera module.

🚀 Seamless Edge Impulse Integration: Upload images directly to Edge Impulse for training and testing AI models.

🖥️ User-Friendly Web Interface: Easily manage capture, labeling, and upload via a local web GUI.

🔌 USB Communication: ESP32 communicates with the PC via USB for efficient data transfer.

🛠️ Prerequisites

Make sure you have the following installed:

Python 3.x

ESP-IDF or Arduino IDE

Required Python libraries:

pip install flask pyserial requests

ESP32 Camera module

Edge Impulse account

💻 Installation

Clone the repository:

git clone https://github.com/MukeshSankhla/ESP32-Edge-Capture.git
cd ESP32-Edge-Capture

Install the required dependencies:

pip install -r requirements.txt

Flash the ESP32 firmware using Arduino IDE or ESP-IDF.

🚀 Usage

Connect your ESP32 camera module to your PC via USB.

Start the web interface:

python app.py

Open the web GUI in your browser at http://localhost:5000.

Select the COM port, input your Edge Impulse API key, choose between Test or Train, enter a label, and click Capture and Upload.

The image will be captured by the ESP32 and uploaded to Edge Impulse.

📝 Configuration

Update the config.json file with your Edge Impulse API details if needed:

{
  "api_key": "your_api_key_here"
}

🐞 Troubleshooting

Ensure the correct COM port is selected.

Verify your API key is valid.

Restart both the ESP32 and the Python script if encountering connectivity issues.

🤝 Contributing

Contributions are welcome! Feel free to fork this repository, submit issues, or make pull requests.

📜 License

This project is licensed under the MIT License.

📧 Contact

For any queries or support, feel free to reach out via GitHub Issues.

Happy Coding! 🚀

