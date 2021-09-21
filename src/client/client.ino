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
bool sec_reset = false;
unsigned long timeInitial = 0;
unsigned long timeFinal = 0;

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

void print_elapsed_time(){
  if(timeInitial == 0) 
    return;
  timeFinal = millis()/1000;
  unsigned long sec_elapsed = timeFinal - timeInitial;
  lcd.clear();
  lcd.setCursor(0,0);
  int minutes = sec_elapsed / 60;
  int hours = sec_elapsed / 3600;
  int seconds = sec_elapsed % 60;
  minutes = minutes % 60;
//  lcd.print(hours + "h " + minutes + "m " + seconds + "s");
  lcd.print(hours);
  lcd.print("h ");
  lcd.print(minutes);
  lcd.print("m ");
  lcd.print(seconds);
  lcd.print("s");
}

void print_new_alert(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Alert < 14s ago");
  lcd.setCursor(0,1);
  lcd.print("Timer start soon");
}

void loop() {
  if(msg.motion == 1){
    timeInitial = millis()/1000;
    print_new_alert();
    play_note();
    digitalWrite(LED_PIN, HIGH); // LED on
    check_push_button(); // elapses 12 seconds
    alerts_off();
    print_elapsed_time();
  }
  print_elapsed_time();
  msg.motion = 0;
}
