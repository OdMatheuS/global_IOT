/*
Documentação:
https://www.mathworks.com/help/thingspeak/channel-settings.html#keys
*/
// Inclusão de bibliotecas
#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// Nome da rede Wi-Fi emulada pelo Wokwi
const char* ssid = "Wokwi-GUEST";

// Senha da rede Wi-Fi
const char* password = "";

// Cria o objeto "client"
WiFiClient  client;

// Define um nome para o canal de envio dos dados
unsigned long myChannelNumber = 1;

// Define uma variável que recebe a código chave de escrita da API 
//obs substituir pela API KEY do THINGSPEAK
const char * myWriteAPIKey = "XXYY";

// Variáveis de tempo
unsigned long lastTime = 0;

// O ciclo de postagens no ThingSpeak será de 30 segundos.
unsigned long timerDelay = 30000;

// Variável de temperatura
float temperatura;

// Define o pino do sensor DHT22
#define dhtPin 5

// Define o sensor com DHT22
#define DHTTYPE DHT22

// Cria o objeto "dht"
DHT dht(dhtPin, DHTTYPE);

// Rotina de configuração
void setup() {
  // Inicializa a porta serial com baud rate de 115200 bits/segundo
  Serial.begin(115200);

  // Conecta à rede Wi-Fi com os parâmetros que foram informados
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // Aguarda a conexão...
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Imprime na porta serial o endereço IP da rede e inicia o servidor
  Serial.println("");
  Serial.print("Conectado a: ");
  Serial.println(ssid);
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  Serial.println("Aguarde 30 segundos para a enviar a requisição");
   
  // Inicializa o sensor DHT22
  dht.begin();

  // Define o modo de operação do Wi-Fi (STATION)
  WiFi.mode(WIFI_STA);
  
  // Inicializa ThingSpeak
  ThingSpeak.begin(client);
}
// Rotina principal
void loop() {
  // Inicia o temporizador de 30 segundos.
  // Os valores serão atualizados a cada 30 segundos no gráfico do ThingSpeak.
  if ((millis() - lastTime) > timerDelay) {
    
    // Conecta ou reconecta a rede Wi-Fi
    if(WiFi.status() != WL_CONNECTED){
      // Informa mensagem de conexão no Monitor Serial
      Serial.print("Tentando conectar...");
      // Conecta (no início) ou reconecta (se "cair") a rede Wi-Fi a cada 5 segundos
      // Recebe os parâmetros com as credenciais da rede, definidas no cabeçalho do código
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password);
        delay(2000);
      }
      // Informa mensagem de conexão no Monitor Serial
      Serial.println("\nConectado.");
    }

  // Efetua a leitura da temperatura
  temperatura = dht.readTemperature();

  // Define uma variável "envio", que receberá os parâmetros
  // – Canal do ThingSpeak
  // – Variável "temperatura"
  // – Chave de escrita da API do ThingSpeak
  int envio = ThingSpeak.writeField(myChannelNumber, 1, temperatura, myWriteAPIKey);

  // Caso o ThingSpeak retorne o valor 200, a requisição de envio dos dados foi bem sucedida
  // Escreve no Monitor Serial a mensagem de atualização e o valor a temperatura
  if(envio == 200){
    Serial.println("Canal 1 (Temperatura) do ThingSpeak atualizado");
    Serial.print("Temperatura: ");
    Serial.println(temperatura);
    Serial.println("");
 }
 // Caso o ThingSpeak retorne outro valor, a requisição de envio não foi bem sucedida
 // Escreve no Monitor Serial a mensagem de erro e o código do erro
  else{
    Serial.println("Problema na atualização do canal. Código do erro HTTP" + String(envio));
  }
  // Reinicia o temporizado
  lastTime = millis();
}
}
