#include <ESP8266WiFi.h>
#include <espnow.h>

// Ultrasonic sensor pins
#define TRIG_PIN 2  // D4
#define ECHO_PIN 4  // D2

long duration;
int senderDistance;

// 🔹 Replace with the MAC Address of the Receiver ESP8266
uint8_t receiverMAC[] = {0xC8, 0xC9, 0xA3, 0x57, 0xA7, 0xBE}; // Example MAC address of the receiver

// Data structure to send
typedef struct struct_message {
    int distance;
} struct_message;

struct_message dataToSend;

// Callback function when data is sent
void onDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.print("Send Status: ");
    if (sendStatus == 0) {
        Serial.println("Success");
    } else {
        Serial.println("Fail");
    }
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    
    // Initialize ESP-NOW
    if (esp_now_init() != 0) {
        Serial.println("❌ ESP-NOW Initialization Failed");
        return;
    }
    
    // Set the role for the ESP8266 (this device will be the controller)
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    
    // Register the callback function for send status
    esp_now_register_send_cb(onDataSent);
    
    // Add the receiver as a peer
    if (esp_now_add_peer(receiverMAC, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != 0) {
        Serial.println("❌ Failed to add peer");
        return;
    }
    
    // Set up ultrasonic sensor pins
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Serial.println("✅ ESP-NOW Initialized and Ready to Send Data!");
}

void loop() {
    // Measure distance using ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH, 20000); // Timeout after 20ms
    senderDistance = (duration > 0) ? duration * 0.034 / 2 : -1;

    // Only send data if a valid distance is measured
    if (senderDistance > 0) {
        dataToSend.distance = senderDistance;
        
        // Send data to the receiver using ESP-NOW
        esp_now_send(receiverMAC, (uint8_t *)&dataToSend, sizeof(dataToSend));

        Serial.print("Sent Distance: ");
        Serial.println(senderDistance);
    } else {
        Serial.println("Failed to read sensor!");
    }

    delay(500);  // Delay to avoid flooding with too many messages
}
