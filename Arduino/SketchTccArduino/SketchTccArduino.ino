#include <WiFi.h>
#include <HTTPClient.h> // Implantei uma nova biblioteca para transferir os dados para uma "api"
#include <DHT.h>

#define DHTPIN 15     // Pino de dados do sensor DHT
#define DHTTYPE DHT22 // Tipo do sensor DHT

const char* ssid = "SSID";        
const char* password = "SENHA";  

// URL para onde os dados serão enviados
const char* serverName = "http://meuservidor.com/api/maquinas";

// Dados simulados de máquinas
float machineTemperatures[] = {22.5, 24.0, 26.7};
float machineHumidities[] = {50.0, 45.3, 60.2};
int totalMachines = 3;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    dht.begin();

    // Conectando ao WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }
    Serial.println("Conectado ao WiFi!");
}

void loop() {
    for (int i = 0; i < totalMachines; i++) {
        enviarDados(i, machineTemperatures[i], machineHumidities[i]);
        delay(5000); // Intervalo de envio (5 segundos para simulação)
    }
}

// Função para enviar os dados de uma máquina para a API
void enviarDados(int machineID, float temperature, float humidity) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        http.begin(serverName);
        http.addHeader("Content-Type", "application/json");

        // Criação do JSON
        String jsonPayload = "{\"id\": " + String(machineID + 1) +
                             ", \"temperatura\": " + String(temperature) +
                             ", \"umidade\": " + String(humidity) + "}";

        // Enviando o POST
        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode > 0) {
            Serial.print("Resposta do servidor: ");
            Serial.println(httpResponseCode);
        } else {
            Serial.print("Erro ao enviar: ");
            Serial.println(http.errorToString(httpResponseCode).c_str());
        }

        http.end();
    } else {
        Serial.println("WiFi desconectado!");
    }
}
