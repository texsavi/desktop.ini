#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your WiFi credentials
const char* ssid = "**********";
const char* password = "**********";
// Replace with your backend server URL
const char* serverUrl = "http://";  

void startCameraServer();
void setupLedFlash(int pin);

void sendImageToServer(uint8_t* imageData, size_t imageSize);
void playSoundFeedback(const char* text);

void setup() {
  // ... (rest of the setup function remains the same)

  // OpenCV Webcam capture
  cv2WebcamCapture();
}

void loop() {
  // ... (rest of the loop function remains the same)
}

void cv2WebcamCapture() {
  // ... (rest of the cv2WebcamCapture function remains the same)

  while (true) {
    cv2.Mat frame;
    cap.read(frame);  // Capture a frame from the webcam

    // Convert OpenCV Mat to ESP32 image format
    size_t imageSize = frame.total() * frame.elemSize();
    uint8_t* imageData = frame.data;

    // Send the image to the server for recognition
    sendImageToServer(imageData, imageSize);
  }

  // ... (rest of the cv2WebcamCapture function remains the same)
}

void sendImageToServer(uint8_t* imageData, size_t imageSize) {
  HTTPClient http;

  // Send the image to the server for recognition
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/octet-stream");

  // POST the image data to the server
  int httpCode = http.POST(imageData, imageSize);

  if (httpCode > 0) {
    // Handle server response if needed
    Serial.printf("Server Response Code: %d\n", httpCode);

    // Process the server response, assuming it's a text result
    String serverResponse = http.getString();
    playSoundFeedback(serverResponse.c_str());
  } else {
    Serial.printf("HTTP request failed: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void playSoundFeedback(const char* text) {
  // Use gTTS to convert text to speech and save as an MP3 file
  // Adjust the file path accordingly
  String mp3FileName = "/sdcard/feedback.mp3";  
  String gttsCommand = "gtts-cli '" + String(text) + "' --output " + mp3FileName;
  int gttsResult = system(gttsCommand.c_str());

  if (gttsResult == 0) {
    // Successfully created the MP3 file, now play it on the ESP32
    // You need to implement a function or library to play MP3 files on your ESP32
    // Example: https://github.com/earlephilhower/ESP8266Audio
    // Adjust the code based on your actual audio playback library
  } else {
    Serial.println("Error: gTTS command failed");
  }
}

