#include "heltec.h"
#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
    char msg[64];
} struct_message;

struct_message incomingData;

void OnDataRecv(const uint8_t * mac, const uint8_t *data, int len) {
    memcpy(&incomingData, data, sizeof(incomingData));
    
    // Atualiza o Display
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "Mensagem Recebida:");
    Heltec.display->drawString(0, 20, incomingData.msg);
    Heltec.display->display();
    
    Serial.print("Recebido: ");
    Serial.println(incomingData.msg);
}

void setup() {
    Heltec.begin(true /*DisplayEnable*/, false /*LoRaEnable*/, true /*SerialEnable*/);
    
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "Aguardando dados...");
    Heltec.display->display();

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Erro ao iniciar ESP-NOW");
        return;
    }

    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
}