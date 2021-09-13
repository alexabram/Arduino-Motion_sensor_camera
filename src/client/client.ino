/*
 * ESPNOW for ESP8266 by Espressif
 * Full API reference documentation: https://www.espressif.com/sites/default/files/documentation/2c-esp8266_non_os_sdk_api_reference_en.pdf
 * Reference material for setup: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html#_CPPv412esp_now_initv
 * Official sample code: https://www.espressif.com/sites/default/files/documentation/esp-now_user_guide_en.pdf
 */

#include <ESP8266WiFi.h>
#include <espnow.h>

char msg; // Memory for message recv'd via ESPNOW

const int LED_PIN = 2; // D4

// When recv data, callback here and exec this function
void recvCallback(uint8_t * sender_mac_addr, uint8_t *msg, uint8_t msg_len){
  digitalWrite(LED_PIN, HIGH); // turn LED on
  delay(10000);
  digitalWrite(LED_PIN, LOW); // turn LED off
}

void setup() {
  Serial.begin(115200);

  
  /* Begin IO configurations */
  pinMode(LED_PIN, OUTPUT);
  /* End IO configurations */


  /* Begin ESPNOW configurations */
  // ESP-NOW initialization.
  if(esp_now_init(void) != 0){
    Serial.println("Error during esp_now_init(void)");
    return;
  }

  // Set the ESP-NOW role of the client device
  if(esp_now_set_self_role(ESP_NOW_ROLE_SLAVE) != 0){
    Serial.println("Error during esp_now_set_self_role(ESP_NOW_ROLE_SLAVE)");
    return;
  }

  // Register ESP-NOW receive callback
  if(esp_now_register_recv_cb(recvCallback) != 0){
    Serial.println("Error during esp_now_register_recv_cb(recvCallback)");
    return;
  }
  /* End ESPNOW configurations */
}

void loop() {
  // put your main code here, to run repeatedly:

}
