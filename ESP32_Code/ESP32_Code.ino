/*
Project: ESP32 Edge Capture
Author: Mukesh Sankhla
Website: https://www.makerbrains.com
GitHub: https://github.com/MukeshSankhla

Description:
This program configures an ESP32 camera module to capture images and transfer them via Serial communication.
The configuration is optimized for reduced memory usage and efficient image handling, making it suitable for integration with tools like Edge Impulse.
*/

#include "esp_camera.h"
#include "DFRobot_AXP313A.h"

// Camera pins configuration
#define PWDN_GPIO_NUM     -1  // Power down pin, not used
#define RESET_GPIO_NUM    -1  // Reset pin, not used
#define XCLK_GPIO_NUM     45  // XCLK signal pin
#define SIOD_GPIO_NUM     1   // I2C SDA pin
#define SIOC_GPIO_NUM     2   // I2C SCL pin
#define Y9_GPIO_NUM       48  // Data pin 9
#define Y8_GPIO_NUM       46  // Data pin 8
#define Y7_GPIO_NUM       8   // Data pin 7
#define Y6_GPIO_NUM       7   // Data pin 6
#define Y5_GPIO_NUM       4   // Data pin 5
#define Y4_GPIO_NUM       41  // Data pin 4
#define Y3_GPIO_NUM       40  // Data pin 3
#define Y2_GPIO_NUM       39  // Data pin 2
#define VSYNC_GPIO_NUM    6   // Vertical sync signal pin
#define HREF_GPIO_NUM     42  // Horizontal reference signal pin
#define PCLK_GPIO_NUM     5   // Pixel clock signal pin

DFRobot_AXP313A axp;  // Power management IC instance

void setup() {
  // Initialize Serial communication
  Serial.begin(921600); // Higher baud rate for faster image transfer

  // Initialize the AXP313A power management IC
  while (axp.begin() != 0) {
    Serial.println("AXP313A initialization failed");
    delay(1000);
  }
  
  // Enable power for the OV2640 camera
  axp.enableCameraPower(axp.eOV2640);

  // Configure the camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;  // 20 MHz clock frequency
  config.frame_size = FRAMESIZE_VGA;  // Frame size: 640x480
  config.pixel_format = PIXFORMAT_JPEG;  // Image format: JPEG
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_DRAM;  // Store frame buffer in DRAM
  config.jpeg_quality = 12;  // Adjust image quality (lower = better quality)
  config.fb_count = 1;  // Single frame buffer

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera initialization failed with error 0x%x\n", err);
    return;
  }

  // Configure additional camera settings for optimized performance
  sensor_t *s = esp_camera_sensor_get();
  if (s) {
    s->set_brightness(s, 1);     // Slightly increase brightness
    s->set_contrast(s, 1);       // Slightly increase contrast
    s->set_saturation(s, -2);    // Reduce saturation
    s->set_special_effect(s, 0); // Disable special effects
    s->set_whitebal(s, 1);       // Enable white balance
    s->set_awb_gain(s, 1);       // Enable auto white balance gain
    s->set_wb_mode(s, 0);        // Auto white balance mode
    s->set_gainceiling(s, (gainceiling_t)GAINCEILING_2X);
  }

  Serial.println("READY");  // Indicate the camera is ready
}

void loop() {
  // Check for Serial commands
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    
    if (command == "CAPTURE") {
      // Capture an image
      camera_fb_t *fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("ERROR");  // Notify if the capture failed
        return;
      }
      delay(100);
      esp_camera_fb_return(fb);  // Return the frame buffer to free memory
      delay(100);
      
      // Send the image size to the Serial
      Serial.println(fb->len);
      delay(100);  // Allow time for size transmission
      
      // Transmit the image data via Serial
      Serial.write(fb->buf, fb->len);
      delay(100);  // Allow time for data transmission
    }
  }
  delay(10);  // Short delay to reduce CPU usage
}
