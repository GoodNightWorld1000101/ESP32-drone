#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};//use recivers mac address

typedef struct struct_message {
  int led;
  int motor_1;
  int motor_2;
  int motor_3;
  int motor_4;
} struct_message;

struct_message Drone_controller;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("ESP-NOW Sender Initialized");
  Serial.println("Type message and press enter:");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input == 'on')
    {
        Drone_controller.led = 1;
    }
    else if (input == 'off') 
    {
        Drone_controller.led = 0;
    }
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&Drone_controller, sizeof(Drone_controller));
    if (result == ESP_OK) {
      Serial.println("Sent: " + input);
    } else {
      Serial.println("Error sending the data");
    }
  }
}
