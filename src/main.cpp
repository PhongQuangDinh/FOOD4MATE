#include <WiFi.h>
#include <NTPClient.h>
#include "PubSubClient.h"
#include <Firebase.h>
#include "loadCell.h"
#include "distanceSensor.h"
#include "dcMotor.h"
#include "buzzer_button.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "test.mosquitto.org";
const int port = 1883;

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
      client.subscribe("21127668/temp"); // use this to receive shit
    }
    else{
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// print out shit that u received
void callback(char* topic, byte* message, unsigned int length)
{
  Serial.print(topic);
  String stMessage;
  for (int i = 0; i < length; i++){
    stMessage += (char)message[i];
  }
  Serial.println(stMessage);
}

// void sendRequest(){
//   Serial.println("Connectinng to ");
//   Serial.println(host);
//   Serial.println(": ");
//   Serial.println(port);

//   WiFiClient client;
//   while (!client.connect(host, port)){
//     Serial.println("connection fail");
//     delay(1000);
//   }

//   client.print("GET " + String(request) + " HTTP/1.1\r\n" + 
//                 "Host: " + host + "\r\n" +
//                 "Connection: close\r\n\r\n");
//   delay(500);

//   while(client.available()){
//     String line = client.readStringUntil('\R');
//     Serial.println(line);
//   }
//   Serial.println();
// }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(("Connecting to Wifi:"));
  wifiConnect();
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("Set server MQTT");
  client.setServer(mqttServer, port);
  client.setCallback(callback);
  // sendRequest();
}

void loop() 
{
  if(!client.connected()){
    mqttReconnect();
  }
  client.loop();
  // create content to be publish
  int temp = random(0, 100);
  char buffer[50];
  sprintf(buffer, "%d", temp);
  // you could create json content for database
  //int humidity = // use your fucking sensor here
  //sprintf(buffer,"{\"temperature\":%d,\"humidity\":%d}",temp,humidity);

  //client.publish("21127668/temp", buffer); //publish random numbers
  const char* somekindofmsg = "";
  if (temp > 50) somekindofmsg = "Hi bro what are u doing";
  else somekindofmsg = "Its fine bro I got ya";
  client.publish("21127668/temp",somekindofmsg);
  delay(5000);
}