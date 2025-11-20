// ====== BIBLIOTECAS ======================================
#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

// ====== WIFI =============================================
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ====== MQTT / FIWARE ====================================
const char* mqttServer = "20.15.161.64";
int mqttPort = 1883;
const char* mqttTopic = "/TEF/lamp001/attrs";

// ====== LCD ==============================================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ====== PINOS ============================================
#define TRIG 13
#define ECHO 12
#define BTN_AZUL 4
#define BTN_VERMELHO 5
#define LED_VERDE 26
#define LED_VERMELHO 27
#define LED_AZUL 25
#define BUZZER 14

WiFiClient espClient;
PubSubClient client(espClient);

// ====== VARIÁVEIS ========================================
unsigned long tempoUltimaMissao = 0;
unsigned long intervaloMissao = 15000;  // 15 segundos
bool missaoAtiva = false;

unsigned long ultimoBlink = 0;
bool estadoBlink = false;

unsigned long debounceAzul = 0;
unsigned long debounceVerm = 0;

// ====== FUNÇÕES ==========================================

long medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH);
  long dist = duracao * 0.034 / 2;

  // LOG: distância medida
  Serial.print("[SENSOR] Distancia medida: ");
  Serial.print(dist);
  Serial.println(" cm");

  return dist;
}

void tocarNovaMissao() {
  tone(BUZZER, 900, 120);
  delay(120);
  tone(BUZZER, 1200, 120);
  delay(120);
  noTone(BUZZER);
}

// Animação simples no LCD
void animacaoPensando() {
  static int frame = 0;

  lcd.setCursor(0, 1);
  if (frame == 0) lcd.print("Pensando .     ");
  if (frame == 1) lcd.print("Pensando ..    ");
  if (frame == 2) lcd.print("Pensando ...   ");

  frame = (frame + 1) % 3;
}

// Gráfico de barras baseado na distância
void graficoDistancia(int dist) {
  int barras = map(dist, 0, 50, 16, 0);
  if (barras < 0) barras = 0;
  if (barras > 16) barras = 16;

  lcd.setCursor(0, 0);
  for (int i = 0; i < 16; i++) {
    if (i < barras) lcd.print("|");
    else lcd.print(" ");
  }
}

// ====== WIFI + LCD =============================================
void conectarWiFi() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectando...");
  lcd.setCursor(0, 1);
  lcd.print("WiFi...");

  Serial.println("[WIFI] Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\n[WIFI] WiFi conectado!");
  Serial.print("[WIFI] IP: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi conectado!");
  delay(1200);
}

// ====== MQTT =============================================
void reconectarMQTT() {
  while (!client.connected()) {
    Serial.println("[MQTT] Tentando conectar ao broker...");
    if (client.connect("ESP32Lamp001")) {
      Serial.println("[MQTT] Conectado com sucesso!");
    } else {
      Serial.print("[MQTT] Falha. rc=");
      Serial.print(client.state());
      Serial.println(" - tentando de novo em 1s");
      delay(1000);
    }
  }
}

// ====== SETUP ============================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n[SETUP] Iniciando sistema...");

  lcd.init();
  lcd.backlight();

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BTN_AZUL, INPUT_PULLUP);
  pinMode(BTN_VERMELHO, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);

  conectarWiFi();
  client.setServer(mqttServer, mqttPort);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema Pronto!");
  Serial.println("[SETUP] Sistema pronto, aguardando missoes...");
  delay(1200);
}

// ====== LOOP =============================================
void loop() {

  if (!client.connected()) reconectarMQTT();
  client.loop();

  long distancia = medirDistancia();

  lcd.clear();
  graficoDistancia(distancia);

  // ===== NOVA MISSAO =====================================
  if (!missaoAtiva && millis() - tempoUltimaMissao >= intervaloMissao) {
    
    if (distancia <= 10) {  
      missaoAtiva = true;

      lcd.setCursor(0, 1);
      lcd.print("NOVA MISSAO!");

      Serial.println("[MISSAO] Nova missao disparada! (Sergio perto: <= 10cm)");
      
      digitalWrite(LED_AZUL, HIGH);
      tocarNovaMissao();
    } else {
      // opcional: log pra saber que o tempo passou, mas Sergio tá longe
      Serial.println("[MISSAO] Tempo da missao chegou, mas Sergio esta longe (>10cm)");
      tempoUltimaMissao = millis();  // reinicia o timer mesmo assim ou comente se quiser esperar
    }
  }

  // ===== MISSÃO ATIVA – aguardando resposta do Sérgio =====
  if (missaoAtiva) {

    animacaoPensando();  // animação no LCD

    // Piscar LED azul (IA pensando)
    if (millis() - ultimoBlink > 250) {
      ultimoBlink = millis();
      estadoBlink = !estadoBlink;
      digitalWrite(LED_AZUL, estadoBlink);
    }

    String estado = "OK";

    // Sérgio entendeu (debounce)
    if (digitalRead(BTN_AZUL) == LOW && millis() - debounceAzul > 300) {
      debounceAzul = millis();

      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_VERMELHO, LOW);
      digitalWrite(LED_AZUL, LOW);

      estado = "SK-103";
      missaoAtiva = false;
      tempoUltimaMissao = millis();

      lcd.setCursor(0, 1);
      lcd.print("Entendeu      ");

      Serial.println("[RESPOSTA] Sergio ENTENDEU (botao azul)");
    }

    // Sérgio não entendeu
    if (digitalRead(BTN_VERMELHO) == LOW && millis() - debounceVerm > 300) {
      debounceVerm = millis();

      digitalWrite(LED_VERMELHO, HIGH);
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AZUL, LOW);

      estado = "SK-NOT-UNDERSTOOD";
      missaoAtiva = false;
      tempoUltimaMissao = millis();

      lcd.setCursor(0, 1);
      lcd.print("Nao entendeu  ");

      Serial.println("[RESPOSTA] Sergio NAO ENTENDEU (botao vermelho)");
    }

    // Se a missão foi respondida (estado mudou)
    if (!missaoAtiva && (estado == "SK-103" || estado == "SK-NOT-UNDERSTOOD")) {
      // LED azul acende durante envio
      digitalWrite(LED_AZUL, HIGH);

      // envia para FIWARE
      String payloadDist = "i|" + String(distancia);
      String payloadState = "s|" + estado;

      Serial.println("[FIWARE] Enviando dados para IoT Agent:");
      Serial.print("         Payload 1 -> ");
      Serial.println(payloadDist);
      Serial.print("         Payload 2 -> ");
      Serial.println(payloadState);

      client.publish(mqttTopic, payloadDist.c_str());
      delay(50);
      client.publish(mqttTopic, payloadState.c_str());

      digitalWrite(LED_AZUL, LOW);
    }
  }

  // ===== SEM MISSÃO ======================================
  if (!missaoAtiva) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    // LED azul só acende quando tem missão ou envio
  }

  delay(120);
}
