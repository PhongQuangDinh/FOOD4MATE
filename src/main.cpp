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

WiFiUDP ntpUD;
NTPClient timeClient(ntpUD);

#define FIREBASE_HOST "https://console.firebase.google.com/project/food4mate/database/food4mate-default-rtdb/data/~2F"
#define FIREBASE_AUTH "4NACSk3E50IvJ6OgH4OSBvFmyvJoQLTaetovPTi0"
FirebaseData fireBaseData;

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

void mqttReconnect()
{
  while(!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    if(client.connect("21127668"))
    {
      Serial.println("connected");
      client.subscribe("Open/Close Tray");
      client.subscribe("Time choice");
      client.subscribe("Set Feeding Time Interval");
      client.subscribe("Food Type");
    }
    else
    {
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

bool StringEqual(const char* a, const char* b) // bruh moment :>
{
    while (*a != '\0' && *b != '\0') 
    {
        if (*a != *b)
            return false;
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}
void ExecuteUIorder(char* topic, String stMessage)
{
  if (StringEqual(topic, "Open/Close Tray"))
  {
    // int value = stMessage.toInt() + 999999;
    // Serial.println(value);
    if (stMessage == "true")
      client.publish("Open/Close Tray", "I got the order Now open the Tray!!!");
  }
}
// print out shit that u received
void callback(char* topic, byte* message, unsigned int length)
{
  Serial.print(topic);
  Serial.print(": ");
  String stMessage;
  for (int i = 0; i < length; i++)
    stMessage += (char)message[i];
  Serial.println(stMessage);

  ExecuteUIorder(topic ,stMessage);
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // hardwares first
  // //setup_loadCellCalibration();
  // setup_loadCell();
  setup_distanceSensor();
  setup_buzzer();

  Serial.println(("Connecting to Wifi:"));
  wifiConnect();
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("Set server MQTT");
  client.setServer(mqttServer, port);
  client.setCallback(callback);
  // sendRequest();

  // Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  // Firebase.reconnectWiFi(true);
  // if (!Firebase.beginStream(fireBaseData, "/Nodes/led"))
  // {
  //   Serial.println("Could not begin stream");
  //   Serial.println("REASON: " + fireBaseData.errorReason());
  //   Serial.println();
  // }

  timeClient.begin();
  timeClient.setTimeOffset(7*3600);
}

void loop() 
{
  timeClient.update();
  // Serial.println(timeClient.getFormattedTime());

  if(!client.connected()) mqttReconnect();
  client.loop();
  // // create content to be publish
  int temp = random(50, 1000);
  char buffer1[50], buffer2[50];
  sprintf(buffer1, "%d", random(0, 100));
  sprintf(buffer2, "%d", random(0, 3000));
  // // you could create json content for database
  // //int humidity = // use your fucking sensor here
  // //sprintf(buffer,"{\"temperature\":%d,\"humidity\":%d}",temp,humidity);

  // //client.publish("21127668/temp", buffer); //publish random numbers
  // const char* somekindofmsg = "";
  // if (temp > 50) somekindofmsg = "Mở vài cái topic đi bạn eii";
  // else somekindofmsg = "SIUUUUU !!!";
  // client.publish("textTopic", timeClient.getFormattedTime().c_str());
  // delay(5000);

  // // hardwares
  // // loop_loadCellCalibration();
  // loop_loadCell();
  // Serial.println(getCurWeight(), 1);


  //client.publish("Open/Close Tray", buffer);

  // write data to Firebase
  // for (int i=0; i < 10; i++)
  // {
  //   Firebase.setInt(fireBaseData,"", i);
  //   delay(1000);
  // }

  // read data from Firebase
  // if (Firebase.getInt(fireBaseData,""))
  // {
  //   if (fireBaseData.dataType() == "int")
  //   {
  //     Serial.println(fireBaseData.intData());
  //   }
  // }

  client.publish("Food Left In Tray", buffer1);
  client.publish("Food Left In Container", buffer2);
  delay(500);
}