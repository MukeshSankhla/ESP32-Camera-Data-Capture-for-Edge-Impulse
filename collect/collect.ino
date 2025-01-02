#include "esp_camera.h"

// Camera pins
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     45
#define SIOD_GPIO_NUM     1
#define SIOC_GPIO_NUM     2

#define Y9_GPIO_NUM       48
#define Y8_GPIO_NUM       46
#define Y7_GPIO_NUM       8
#define Y6_GPIO_NUM       7
#define Y5_GPIO_NUM       4
#define Y4_GPIO_NUM       41
#define Y3_GPIO_NUM       40
#define Y2_GPIO_NUM       39
#define VSYNC_GPIO_NUM    6
#define HREF_GPIO_NUM     42
#define PCLK_GPIO_NUM     5

#include "DFRobot_AXP313A.h"
DFRobot_AXP313A axp;

void setup() {
  Serial.begin(921600); // Higher baud rate for faster image transfer
  
  while(axp.begin() != 0){
    Serial.println("init error");
    delay(1000);
  }
  axp.enableCameraPower(axp.eOV2640);

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
  config.xclk_freq_hz = 20000000;
  
  // Reduce frame size and quality to save memory
  config.frame_size = FRAMESIZE_VGA;      // Smaller frame size (320x240)
  config.pixel_format = PIXFORMAT_JPEG;    // Use JPEG format
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_DRAM;  // Use DRAM instead of PSRAM
  config.jpeg_quality = 12;                // Reduce quality slightly
  config.fb_count = 1;                     // Single frame buffer

  // Initialize camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Additional camera settings to optimize memory usage
  sensor_t * s = esp_camera_sensor_get();
  if (s) {
    s->set_brightness(s, 1);     // Increase brightness slightly
    s->set_contrast(s, 1);       // Increase contrast slightly
    s->set_saturation(s, -2);    // Reduce saturation
    s->set_special_effect(s, 0); // No special effects
    s->set_whitebal(s, 1);       // Enable white balance
    s->set_awb_gain(s, 1);       // Enable AWB gain
    s->set_wb_mode(s, 0);        // Auto WB
    s->set_gainceiling(s, (gainceiling_t)GAINCEILING_2X);
  }

  Serial.println("READY"); // Signal that the camera is ready
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    
    if (command == "CAPTURE") {
      // Take picture
      camera_fb_t * fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("ERROR");
        return;
      }
      delay(100);
      esp_camera_fb_return(fb);
      delay(100);
      // Send image size first
      Serial.println(fb->len);
      delay(100);  // Small delay to allow data transmission to be completed
      // Send the image data
      Serial.write(fb->buf, fb->len);
      delay(100);
    }
  }
  delay(10);
}
