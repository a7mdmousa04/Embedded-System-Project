#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
// SIM800L
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

// WiFi credentials
const char* ssid = "realme 11";
const char* password = "12341234";

// Web server details
const char* serverBaseURL = "https://embeddedsysproject-production.up.railway.app/send-data";
const char* viewURL = "https://embeddedproject-production.up.railway.app/";

// SIM800L setup
#define SIM800_RX 4   // D2 -> Connect to TX of SIM800L
#define SIM800_TX 5    //D1 -> Connect to RX of SIM800L
#define ADMIN_NUMBER "+201122422764"
SoftwareSerial SerialAT(SIM800_RX, SIM800_TX); // RX, TX
TinyGsm modem(SerialAT); 

// Switch
#define SWITCH_PIN 13

// Ultrasonic sensor
#define TRIG_PIN 12 // D6
#define ECHO_PIN 14 //D5
long duration;
int receiverDistance = -1;
int senderDistance = 0;

// ESP-NOW data
typedef struct struct_message {
  int distance;
} struct_message;

struct_message receivedData;

// Callback when data is received
void onDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  senderDistance = receivedData.distance;
  Serial.print("📡 Received Sender Distance: ");
  Serial.println(senderDistance);
}

unsigned long lastHttpTime = 0;
const unsigned long httpInterval = 15000;

void setup() {
  Serial.begin(115200);
  SerialAT.begin(9600); // SIM800L
  delay(1000);
  Serial.println("🔌 Booting...");
  
  // WiFi setup
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("🌐 Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  
  // ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("❌ ESP-NOW Init Failed");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataRecv);
  
  // Sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // SIM800L setup
  Serial.println("📞 Initializing SIM800L...");
  modem.init();
  delay(2000);  // Allow time for the modem to initialize
  Serial.println("✅ SIM800L Ready");
  
  Serial.println("🌐 View data at: " + String(viewURL));
  
  //switch pin
  pinMode(SWITCH_PIN, INPUT_PULLUP);
}

bool sendHttpRequest() {
  int sensor1Value = senderDistance;
  int sensor2Value = (receiverDistance >= 0) ? receiverDistance : 0;
  String url = String(serverBaseURL) + "?sensor1=" + String(sensor1Value) + "&sensor2=" + String(sensor2Value);
  
  WiFiClientSecure client;
  client.setInsecure(); // Accept self-signed certificates
  
  HTTPClient http;
  http.begin(client, url);
  
  int httpCode = http.GET();
  Serial.print("🌐 HTTP Response Code: ");
  Serial.println(httpCode);
  http.end();
  return (httpCode == HTTP_CODE_OK);
}

void sendSMS() {
  String smsMessage = "Sender: " + String(senderDistance) + "cm\nReceiver: " + String(receiverDistance) + "cm";
  bool res = modem.sendSMS(ADMIN_NUMBER, smsMessage);
  Serial.println(res ? "📩 SMS Sent" : "❌ SMS Failed");
}

void loop() {
  // Ultrasonic read
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH, 20000);
  receiverDistance = (duration > 0) ? duration * 0.034 / 2 : -1;
  Serial.print("📏 Receiver Distance: ");
  Serial.println(receiverDistance);
  
  // Every 15 seconds, send HTTP and SMS
  if (millis() - lastHttpTime > httpInterval && WiFi.status() == WL_CONNECTED) {
    lastHttpTime = millis();
    
    // Send HTTP Request and SMS if data is available
    if (sendHttpRequest()) {
      sendSMS();
    } else {
      Serial.println("❌ Failed to send HTTP request.");
    }
  }
  
  delay(500);
}