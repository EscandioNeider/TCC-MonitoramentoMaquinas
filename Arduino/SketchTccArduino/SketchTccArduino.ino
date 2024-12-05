#include <WiFi.h>  
#include <Adafruit_Sensor.h>  
#include <DHT.h>  
#include <Wire.h>  
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h>  

// Definições do sensor DHT e o pino que ele está conectado  
#define DHTPIN 15     // Pino de dados do sensor DHT  
#define DHTTYPE DHT22 // Definir se é DHT11 ou DHT22  

// Definições do OLED (largura e altura) 
#define SCREEN_WIDTH 128 // Largura do OLED 
#define SCREEN_HEIGHT 64 // Altura do OLED 

// Endereço I2C do display OLED 
#define OLED_RESET -1  // Sem pino de reset 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 

// Variáveis para a conexão WiFi  
const char* ssid = "SEU_SSID"; // Colocar aqui o nome da rede WiFi  
const char* password = "SUA_SENHA"; // Colocar aqui a senha do WiFi  

// Instância do sensor DHT  
DHT_Unified dht(DHTPIN, DHTTYPE);  

// Definindo o pino do botão 
#define buttonPin 4 // Pino onde o botão está conectado 

// Variáveis para armazenar o estado das máquinas 
int currentMachine = 0; // Variável para rastrear qual máquina está sendo exibida 
int totalMachines = 3; // Supondo que você tenha 3 máquinas 

// Dados simulados de temperatura e umidade para cada máquina 
float machineTemperatures[] = {22.5, 24.0, 26.7}; // Exemplo de temperaturas das máquinas 
float machineHumidities[] = {50.0, 45.3, 60.2}; // Exemplo de umidades das máquinas 

void setup() { 
  // Iniciando a comunicação serial (importante para debugar)  
  Serial.begin(115200); 

  // Inicializando o sensor DHT  
  dht.begin(); 

  // Inicializando o display OLED 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Verifica o endereço I2C do OLED 
    Serial.println(F("Falha ao iniciar o display OLED")); 
    for(;;); // Fica em loop se falhar 
  } 

  display.clearDisplay(); 
  display.setTextSize(1);             // Define o tamanho do texto (1 é o padrão) 
  display.setTextColor(SSD1306_WHITE); // Cor do texto (branco) 
  display.setCursor(0, 0); 
  display.print("Iniciando..."); // Mensagem para saber que o sistema está iniciando 
  display.display(); // Exibe a mensagem no OLED 
  delay(2000); 

  // Conectando ao WiFi  
  conectarWiFi(); 

  // Configurando o botão 
  pinMode(buttonPin, INPUT_PULLUP); // Configura o pino do botão como entrada com resistor de pull-up 

  // Sistema pronto, informando no OLED  
  display.clearDisplay(); 
  display.setCursor(0, 0);  
  display.print("Sistema Pronto!");  
  display.display(); 
  delay(2000); 
} 

void loop() { 
  // Verifica se o botão foi pressionado 
  if (digitalRead(buttonPin) == LOW) { 
    delay(200); // Debounce do botão 
    currentMachine++; 
    // Se ultrapassar o número total de máquinas, volta para a primeira 
    if (currentMachine >= totalMachines) { 
      currentMachine = 0; 
    } 

    // Mostra as informações da máquina atual 
    mostrarDados(currentMachine); 

    // Aguarda o botão ser liberado 
    while (digitalRead(buttonPin) == LOW); 
  } 

  delay(100); // Pequeno atraso para evitar leitura constante do botão 
} 

// Função para mostrar os dados da máquina atual no OLED 

void mostrarDados(int machineIndex) { 
  display.clearDisplay(); // Limpa o display 
  display.setCursor(0, 0); 
  display.print("Maquina "); 
  display.print(machineIndex + 1); 
  
  display.setCursor(0, 20); 
  display.print("Temp: "); 
  display.print(machineTemperatures[machineIndex]); 
  display.print("C"); 

  display.setCursor(0, 40); 
  display.print("Humid: "); 
  display.print(machineHumidities[machineIndex]); 
  display.print("%"); 

  display.display(); // Atualiza o display com as informações 
} 

// Função para ler os sensores e exibir os dados no Serial 
void lerSensores() {  
  sensors_event_t event;  

  // Lendo a temperatura do DHT  
  dht.temperature().getEvent(&event);  
  if (!isnan(event.temperature)) {  
    Serial.print("Temperatura: ");  
    Serial.print(event.temperature);  
    Serial.println(" °C");  
  } else {  
    Serial.println("Erro ao ler a temperatura!");  
  }  

  // Lendo a umidade do DHT  
  dht.humidity().getEvent(&event);  
  if (!isnan(event.relative_humidity)) {  
    Serial.print("Umidade: ");  
    Serial.print(event.relative_humidity);  
    Serial.println(" %");  
  } else {  
    Serial.println("Erro ao ler a umidade!");  
  }  
}  

// Função para conectar o ESP32 ao WiFi  
void conectarWiFi() {  
  Serial.print("Conectando-se a ");  
  Serial.println(ssid);  

  WiFi.begin(ssid, password);  

  // Enquanto não conectar, vai mostrar os pontos no Serial  
  while (WiFi.status() != WL_CONNECTED) {  
    delay(1000);  
    Serial.print(".");  
  }  

  // Conexão WiFi bem-sucedida, exibe o IP no Serial  
  Serial.println("");  
  Serial.println("WiFi conectado!");  
  Serial.print("IP: ");  
  Serial.println(WiFi.localIP());  
}
