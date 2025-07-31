#include <esp_now.h>
#include <WiFi.h>

#define LED 2

typedef struct struct_message {
  int led;
  int motor_1;
  int motor_2;
  int motor_3;
  int motor_4;
} struct_message;

struct_message receivedData;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.print("Received LED state: ");
  Serial.println(receivedData.led);
  Serial.print("Received Motor 1: ");
  Serial.println(receivedData.motor_1);
  Serial.print("Received Motor 2: ");
  Serial.println(receivedData.motor_2);
  Serial.print("Received Motor 3: ");
  Serial.println(receivedData.motor_3);
  Serial.print("Received Motor 4: ");
  Serial.println(receivedData.motor_4);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  pinMode(LED, OUTPUT);

  uint8_t senderAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Replace with actual sender MAC if known

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, senderAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("ESP-NOW Receiver Initialized");
}

void loop() {
  if (receivedData.led == 1)
  {
    digitalWrite(LED, HIGH);
  }
  else 
  {
    digitalWrite(LED, LOW);
  }
}
