#include <WiFi.h>
#include <NTPClient.h>
#include "PubSubClient.h"
#include <Firebase.h>
#include "loadCell.h"
#include "distanceSensor.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* host = "maker.ifttt.com"; //"api.openweathermap.org"; 
const char* request = "/trigger/get_event_ok/with/key/n85lziFzLRfSHvs8n_MDSj0IdxGWEfzvVf04CZkMPnQ"; // "/data/2.5/weather?lat19.808855=&lon=105.708653&units=metric&appid=e482262113202e9966790c16d88c4039"; 
const int port = 80;

WiFiClient espClient;
PubSubClient client(espClient);

void wifiConnect()
{
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void mqttReconnect(){
  while(!client.connected()){
    Serial.println("Attempting MQTT connection...");
    if(client.connect("21127668")){
      Serial.println(" connected");
      client.subscribe("21127668/led");
    }
    else{
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

/*void callback(char* topic, byte* message, unsigned int length){
  Serial.print(topic);
  String stMessage;
  for (int i = 0; i < length; i++){
    stMessage += (char)message[i];
  }
  Serial.println(stMessage);
}*/

void sendRequest(){
  Serial.println("Connectinng to ");
  Serial.println(host);
  Serial.println(": ");
  Serial.println(port);

  WiFiClient client;
  while (!client.connect(host, port)){
    Serial.println("connection fail");
    delay(1000);
  }

  client.print("GET " + String(request) + " HTTP/1.1\r\n" + 
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  delay(500);

  while(client.available()){
    String line = client.readStringUntil('\R');
    Serial.println(line);
  }
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(("Connecting to Wifi:"));
  wifiConnect();
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  sendRequest();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  // this speeds up the simulation
  // if(!client.connected()){
  //   mqttReconnect();
  // }
  // client.loop();

  // int temp = random(0, 100);
  // char buffer[50];
  // sprintf(buffer, "%d", temp);
  // client.publish("21127730/temp: ", buffer);
  delay(5000);
}