#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int OUTPUT_PIN = D1;
const unsigned int SENDING_TIME_MICROSEG = 560;

const char* ssid = "SSID";
const char* password = "Password";

const char* mqtt_server = "IP_MQTT"; 

uint8_t header[] = { 0x00, 0x00, 0x3F, 0xD5, 0x55, 0x5D, 0xDD, 0xD7, 0x77 };
uint8_t on[] = { 0x77, 0x55, 0x55, 0x77, 0x77, 0x77 };
uint8_t off[] = { 0x5D, 0x55, 0x5D, 0x77, 0x77, 0x77 };
uint8_t increase[] = { 0x55, 0x55, 0x77, 0x77, 0x77, 0x77 };
uint8_t decrease[] = { 0x75, 0x55, 0x57, 0x77, 0x77, 0x77 };
uint8_t red[] = { 0x57, 0x55, 0x5D, 0xD7, 0x77, 0x77 };
uint8_t green[] = { 0x75, 0xD5, 0x55, 0xD7, 0x77, 0x77 };
uint8_t blue[] = { 0x5D, 0xD5, 0x57, 0x57, 0x77, 0x77};
uint8_t white[] = { 0x77, 0x75, 0x55, 0x57, 0x77, 0x77 };

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
}

void SEND_SECUENCE(uint8_t* datos, int lenght) {
  for (int i = 0; i < 9; i++) {
    for (int bit = 7; bit >= 0; bit--) {
      bool valorBit = (header[i] >> bit) & 0x01;
      Serial.print(valorBit);
      digitalWrite(OUTPUT_PIN, valorBit ? HIGH : LOW);
      delayMicroseconds(SENDING_TIME_MICROSEG);
    }}

  for (int i = 0; i < lenght; i++) {
    for (int bit = 7; bit >= 0; bit--) {
      bool valorBit = (datos[i] >> bit) & 0x01;
      Serial.print(valorBit);
      digitalWrite(OUTPUT_PIN, valorBit ? HIGH : LOW);
      delayMicroseconds(SENDING_TIME_MICROSEG); 
    }
  }
  Serial.println("");
  digitalWrite(OUTPUT_PIN, LOW);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String mensaje = "";

  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }

  Serial.print("Mensaje recibido: ");
  Serial.println(mensaje);
  
    if(mensaje == "encendido"){
      SEND_SECUENCE(on, sizeof(on));
    }
    else if(mensaje == "apagado" ){
      SEND_SECUENCE(off, sizeof(off));
    }
    else if(mensaje == "subir"){ 
      SEND_SECUENCE(increase, sizeof(increase));
    }
    else if(mensaje == "bajar"){ 
      SEND_SECUENCE(decrease, sizeof(decrease));
    }
    else if(mensaje == "rojo"){ 
      SEND_SECUENCE(red, sizeof(red));
    } 
    else if(mensaje == "verde"){ 
      SEND_SECUENCE(green, sizeof(green));
    } 
    else if(mensaje == "azul"){ 
      SEND_SECUENCE(blue, sizeof(blue));
    } 
    else if(mensaje == "blanco"){ 
      SEND_SECUENCE(white, sizeof(white));
    } 
  }
  
void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("conectado");
      client.subscribe("led/color");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" intentando en 5 segundos");
      delay(5000);
    }
  }
}


void setup() {
  pinMode(OUTPUT_PIN, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
