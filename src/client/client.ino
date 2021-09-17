/*
 * ESPNOW for ESP8266 by Espressif
 * Full API reference documentation: https://www.espressif.com/sites/default/files/documentation/2c-esp8266_non_os_sdk_api_reference_en.pdf
 * Reference material for setup: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html#_CPPv412esp_now_initv
 * Official sample code: https://www.espressif.com/sites/default/files/documentation/esp-now_user_guide_en.pdf
 */

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Memory for message to be recv'd via ESPNOW
typedef struct{
  int motion;
} message;
message msg;

const int LED_PIN = 2; // D4
const int SPEAKER_PIN = 15; // D8
const int PUSH_BUTTON_PIN = 16; // D0
#define SPEAKER_NOTE_FREQ 227;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // LCD addr, 16 cols, 2 rows

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
//  Serial.print("Client MAC: ");
//  Serial.println(WiFi.macAddress());

  
  /* Begin IO configurations */
  pinMode(LED_PIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(PUSH_BUTTON_PIN, INPUT);
  
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello World");
  delay(1000);
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
  int wait = SPEAKER_NOTE_FREQ; // wait unit
  int P = 2 * wait; // wait unit
  int k = 1.25 * (1 / (P * 0.001)); // loop repeat
  for(int i = 0; i < k; ++i){
    digitalWrite(SPEAKER_PIN, HIGH); // speaker noise on
    delay(wait);
    digitalWrite(SPEAKER_PIN, LOW); // speaker noise off
    delay(wait);
  }
}

void check_push_button(){
  int count = 0;
  while(count++ < 12)
  {
    int read = digitalRead(PUSH_BUTTON_PIN);
    delay(30);
    if(read == HIGH){ // if push_button is pressed
      return;
    }
    else{
      delay(1000);
    }
  }
}

void alerts_off(){
  digitalWrite(LED_PIN, LOW); // LED off
}

void loop() {
  if(msg.motion == 1){
    play_note();
    digitalWrite(LED_PIN, HIGH); // LED on
    check_push_button(); // UNCOMMENT IF USING PUSH_BUTTON - elapses 12 seconds
    alerts_off();
  }
  msg.motion = 0;
}
