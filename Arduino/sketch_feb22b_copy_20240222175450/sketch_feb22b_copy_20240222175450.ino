#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// WiFi credentials
const char* ssid = "MMU-Wifi";
const char* password = "pass1234";

// Adafruit IO credentials
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "Bettech"
#define AIO_KEY         "aio_pUOc89WGO4JyG24JVZOutjHgrDSu"
#define AIO_TOPIC    "Bettech/feeds/temphumidity"
WiFiClient client;

// Create an MQTT client instance
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Define your Adafruit IO feed
Adafruit_MQTT_Publish feed = Adafruit_MQTT_Publish(&mqtt, AIO_TOPIC);

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");

  // Connect to Adafruit IO
  connectToAdafruitIO();
}

void loop() {
  // Publish the value to the Adafruit IO feed
  publishToFeed("40");

  delay(5000); // Publish every 5 seconds (adjust as needed)
}

void connectToAdafruitIO() {
  Serial.print("Connecting to Adafruit IO...");
  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
  }
  Serial.println("Connected to Adafruit IO");
}

void publishToFeed(String value) {
  // Convert the String to const char*
  const char* valueStr = value.c_str();

  if (!feed.publish(valueStr)) {
    Serial.println("Failed to publish to Adafruit IO feed!");
  } else {
    Serial.println("Value published to Adafruit IO feed: " + value);
  }
}