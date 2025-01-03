# EdgeCapture

**Effortless ESP32 Camera Data Collection for Edge Impulse**

EdgeCapture is a streamlined tool designed to simplify image data collection and uploading for Edge Impulse projects using an ESP32 camera. Say goodbye to the hassle of SD card modules and manual transfers—EdgeCapture automates the entire process with a user-friendly web interface.

## 🚀 Features
- **No SD Card Required:** Direct image transfer without the need for external storage.
- **User-Friendly Interface:** Simple web-based interface for configuration and control.
- **Real-Time Image Display:** Preview captured images before uploading.
- **Seamless Integration:** Upload directly to your Edge Impulse project.

## 🛠️ How It Works
1. **Connect ESP32 Camera:** Plug your ESP32 into your PC via USB.
2. **Launch Web Interface:** Run the Python script to open the web-based control panel.
3. **Configure Settings:** Select COM port, enter your API key, choose 'Train' or 'Test', and input a label.
4. **Capture and Upload:** Click 'Capture and Upload' to fetch, display, and send the image directly to Edge Impulse.

## 📦 Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/MukeshSankhla/ESP32-Edge-Capture.git
   ```
2. Navigate to the project folder:
   ```bash
   cd ESP32-Edge-Capture
   ```
3. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
4. Run the script:
   ```bash
   python app.py
   ```
5. Open the web interface in your browser:
   ```
   http://localhost:5000
   ```

## 🖥️ Requirements
- ESP32 Camera Module
- Python 3.x
- Required Python libraries (see `requirements.txt`)
- Edge Impulse API Key

## 🤝 Contributing
Contributions are welcome! Feel free to fork this repository, make changes, and submit a pull request.

## 📄 License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## 📧 Support
For any questions or support, please open an issue on GitHub or contact us directly.

**Happy Capturing! 📷✨**
