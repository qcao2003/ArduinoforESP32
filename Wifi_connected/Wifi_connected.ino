#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
// 配置TCP端口
WiFiServer ServerPort(1234);

void setup()
{
  // 打开闪光灯
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  // 初始化串口通信，波特率为115200
  Serial.begin(115200);

  // 开始连接Wi-Fi网络
  WiFi.begin("office room", "333666999");

  // 等待Wi-Fi连接成功
  while (WiFi.status() != WL_CONNECTED)
  {
    // 打印提示信息
    Serial.println("正在连接");

    // 延时1秒
    delay(1000);
  }

  // 打印连接成功的IP地址
  Serial.print("ip:");
  Serial.println(WiFi.localIP());
  // 打开TCP
  ServerPort.begin();
}

void loop()
{
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
        digitalWrite(4, HIGH);
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
