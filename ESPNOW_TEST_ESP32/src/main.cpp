#include <esp_now.h>
#include <WiFi.h>

// MAC DO RECEPTOR (HELTEC)
uint8_t broadcastAddress[] = {0x8C, 0x4F, 0x00, 0xD9, 0x2F, 0x00};

typedef struct struct_message {
    char msg[64];
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Erro ao iniciar ESP-NOW");
        return;
    }

    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Falha ao adicionar peer");
        return;
    }
    Serial.println("Digite algo no terminal para enviar:");
}

void loop() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.toCharArray(myData.msg, 64);

        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
        
        if (result == ESP_OK) {
            Serial.print("Enviado: ");
            Serial.println(myData.msg);
        }
    }
}