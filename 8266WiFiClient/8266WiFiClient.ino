/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "office room"
#define STAPSK "333666999"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// 配置TCP端口
WiFiServer ServerPort(1234);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // 打开TCP
  ServerPort.begin();
}

void loop() {
    // 等待客户端连接
  WiFiClient client = ServerPort.available();
  if (client) {
    Serial.println("New client connected");
    
    while (client.connected()) {
      // 检查是否有数据可供读取
      if (client.available()) {
        // 读取客户端发送的数据
        String data = client.readStringUntil('\n');
        Serial.print("Received data: ");
        Serial.println(data);
        digitalWrite(LED_BUILTIN, LOW);  // Turn off LED
        delay(1000);                      // Wait for one second
        digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED on
        delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
        // 发送响应到客户端
        String response = "Server received: " + data;
        client.println(response);
      }
    }
    
    // 断开与客户端的连接
    client.stop();
    Serial.println("Client disconnected");
  }
}
