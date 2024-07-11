#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// 定义WiFi热点信息
const char *ssid = "ESP8266_HOTSPOT";
const char *password = "lpflpflpf";

// 创建Web服务器实例
ESP8266WebServer server(80);
String data; // 用于存储从STM32接收的数据

// 设置获取数据的处理函数
void handleRoot() {
  server.send(200, "text/plain", data);
  data = ""; // 清空数据
}

void setup() {
  Serial.begin(115200); // 初始化硬件串口
  
  // 设置热点模式
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // 设置Web服务器的路由
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // 接收串口数据
  if (Serial.available()) {
    data = Serial.readStringUntil('\n');
    Serial.print("Received data: ");
    Serial.println(data);
  }
}
