/*
 * ESPNOW for ESP8266 by Espressif
 * Full API reference documentation: https://www.espressif.com/sites/default/files/documentation/2c-esp8266_non_os_sdk_api_reference_en.pdf
 * Reference material for setup: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html#_CPPv412esp_now_initv
 * Official sample code: https://www.espressif.com/sites/default/files/documentation/esp-now_user_guide_en.pdf
 */

#include <ESP8266WiFi.h>
#include <espnow.h>

// Message to be sent via ESPNOW
typedef struct message{
  int motion;
};
struct message msg;

const int SENSOR_PIN = 15; // D8

// Mac address of client
uint8_t client_mac_addr[] = {0x30, 0x83, 0x98, 0xB2, 0x4D, 0xB7};

// ESP-NOW send callback
void sendCallback(uint8_t *client_mac_addr, uint8_t sendStatus){
  Serial.print("Status of message send to MAC is ");
  Serial.println(sendStatus);
}

void setup() {
  Serial.begin(115200);
  Serial.print("Server MAC: ");
  Serial.println(WiFi.macAddress());
  

  /* Begin IO configurations */
  pinMode(SENSOR_PIN, INPUT);
  /* End IO configurations */


  /* Begin ESPNOW configurations */
  WiFi.mode(WIFI_STA);

  // ESP-NOW initialization.
  if(esp_now_init() != 0){
    Serial.println("Error during esp_now_init(void)");
    return;
  }

  // Set the ESP-NOW role of the server device
  if(esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER) != 0){
    Serial.println("Error during esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER)");
    return;
  }

  // Add an ESP-NOW peer, store MAC address of target device into ESP-NOW MAC list.
  if(esp_now_add_peer(client_mac_addr, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != 0){
    Serial.println("Error during sp_now_add_peer(client_mac_addr, ESP_NOW_ROLE_SLAVE, 1, NULL, 0)");
    return;
  }

  // Register ESP-NOW send callback.
  if(esp_now_register_send_cb(sendCallback) != 0){
    Serial.println("Error during esp_now_register_send_cb(sendCallback)");
    return;
  }
  /* End ESPNOW configurations */
}

void loop() {
  // Check for motion & populate struct message
  if(digitalRead(SENSOR_PIN) == HIGH){ // PIR detected motion
    Serial.println("Motion detected.");
    Serial.println("Sending message via ESPNOW!");
    msg.motion = 1;
    // Send ESP-NOW packets.
    if(esp_now_send(NULL, (uint8_t*) &msg, sizeof(msg)) != 0){
      Serial.println("Error during esp_now_send(NULL, (uint8_t*) &msg, sizeof(msg))");
      return;
    }
    msg.motion = 0;
    Serial.println("");
  }
  else{
//    Serial.println("No motion detected");
//    Serial.println("");
    msg.motion = 0;
  }
}
