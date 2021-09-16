/*
 * ESPNOW for ESP8266 by Espressif
 * Full API reference documentation: https://www.espressif.com/sites/default/files/documentation/2c-esp8266_non_os_sdk_api_reference_en.pdf
 * Reference material for setup: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html#_CPPv412esp_now_initv
 * Official sample code: https://www.espressif.com/sites/default/files/documentation/esp-now_user_guide_en.pdf
 */

#include <ESP8266WiFi.h>
#include <espnow.h>

// Memory for message to be recv'd via ESPNOW
typedef struct message{
  int motion;
};
message msg;

const int LED_PIN = 2; // D4
const int SPEAKER_PIN = 15; // D8
#define SPEAKER_NOTE_FREQ 227;

// When recv data, callback here and exec this function
void recvCallback(uint8_t * sender_mac_addr, uint8_t *incomingMsg, uint8_t msg_len){
  Serial.println("Message recv'd");
  memcpy(&msg, incomingMsg, sizeof(msg));
}

void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  ESP.eraseConfig();
  // Wifi STA Mode
  WiFi.mode(WIFI_STA);
  Serial.print("Client MAC: ");
  Serial.println(WiFi.macAddress());

  
  /* Begin IO configurations */
  pinMode(LED_PIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  /* End IO configurations */


  /* Begin ESPNOW configurations */
  // ESP-NOW initialization.
  if(esp_now_init() != 0){
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

void play_note(){
  int wait_high = SPEAKER_NOTE_FREQ;
  int P = 2 * wait_high;
  int k = 1.25 * (1 / (P * 0.001));
  for(int i = 0; i < k; ++i){
    digitalWrite(SPEAKER_PIN, HIGH); // speaker noise on
    delay(wait_high);
    digitalWrite(SPEAKER_PIN, LOW); // speaker noise off
    delay(wait_high);
  }
}

void loop() {
  if(msg.motion == 1){
    digitalWrite(LED_PIN, HIGH); // LED on
    play_note();
//    digitalWrite(SPEAKER_PIN, HIGH); // speaker noise on
    delay(5000);
//    digitalWrite(SPEAKER_PIN, LOW); // speaker noise off
  }
  else{
    digitalWrite(LED_PIN, LOW); // LED off
    digitalWrite(SPEAKER_PIN, LOW); // speaker noise off
  }
  msg.motion = 0;
}
