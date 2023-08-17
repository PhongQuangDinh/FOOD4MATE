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

bool onFeedTime = false;
bool Food_is_out = false;
bool Food_is_prepared = false;
bool CallOnce = true;
int Away_from_food_timer = 1000; //900000; // around 15 mins if pet away from eating food
int away_from_food_counter = Away_from_food_timer;
int DroppingTimer = 10;
int DroppingCounter = DroppingTimer;
int EatTime[6] = {255,255,255,255,255,255};

float currentContainerWeight;

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
      client.subscribe("Create Instant Meal");
      client.subscribe("Time choice");
      client.subscribe("Food Type");
      client.subscribe("Cleaning The Machine");
      client.subscribe("Get Container Weight");
    }
    else
    {
      Serial.println(" try again in 5 seconds");
      // delay(5000);
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
    isOpenTray = (isOpenTray) ? false : true;
  }
  if (StringEqual(topic, "Create Instant Meal"))
  {
    onFeedTime = true;
    // client.publish("",);
  }
  if (StringEqual(topic, "Time choice"))
  {
    stMessage.remove(0, 1);
    stMessage.remove(stMessage.length() - 1);
    int index = 0;
    // Split the string at each comma and convert substrings to integers
    while (stMessage.length() > 0) 
    {
      int commaIndex = stMessage.indexOf(',');
      if (commaIndex != -1) 
      {
        String intValueStr = stMessage.substring(0, commaIndex);
        EatTime[index] = intValueStr.toInt();
        stMessage.remove(0, commaIndex + 1);
      } 
      else 
      {
        EatTime[index] = stMessage.toInt();
        stMessage = ""; // Clear the string
      }
      index++;
    }
  }
  if (StringEqual(topic, "Cleaning The Machine"))
  {
    if (stMessage == "true")
      Serial.println("Cleaning the machine ......");
    else
      Serial.println("Stop cleaning the machine ......");
  }
  if (StringEqual(topic, "Get Container Weight"))
  {
    currentContainerWeight = stMessage.toFloat();
    Serial.println(currentContainerWeight);
  }
}

void callback(char* topic, byte* message, unsigned int length)
{
  Serial.println(topic);
  // Serial.print(": ");
  String stMessage;
  for (int i = 0; i < length; i++)
    stMessage += (char)message[i];
  // Serial.println(stMessage); // print the value

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
  setupDCMotor();

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
void getFeedingTime()
{
  for (int i = 0; i < 6; i++)
    if (timeClient.getHours() == EatTime[i])
      onFeedTime = true;
}
void loop() 
{
  timeClient.update();
  getFeedingTime();


  if(!client.connected()) mqttReconnect();
  client.loop();
  // // create content to be publish
  // int temp = random(50, 1000);
  // char buffer1[50], buffer2[50];
  // sprintf(buffer1, "%d", random(0, 100));
  // sprintf(buffer2, "%d", random(0, 3000));

  // you could create json content for database
  //int humidity = // use your fucking sensor here
  //sprintf(buffer,"{\"temperature\":%d,\"humidity\":%d}",temp,humidity);

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

  // client.publish("Food Left In Tray", buffer1);
  // client.publish("Food Left In Container", buffer2);

  // loop_loadCellCalibration();
  // loop_loadCell();
  // Serial.println(getCurWeight(), 1);

  loopDCMotor();

  if (onFeedTime)
  {
    if (isOpenTray) 
    {
      isOpenTray = false; // ensure that the tray is close
      return;
    }
    if (!Food_is_out)
    {
      if (!Food_is_prepared)
      {
        if (!isOpenFunnel) isOpenFunnel = true;
        if (DroppingCounter > 0)
        {
          Serial.println("Dropping food ...");
          DroppingCounter--;
        }
        else
        {
          Serial.println("Dropping done. Uploading food after dropping ..");
          DroppingCounter = DroppingTimer;

          // update food left in tray, this thing cause slow
          char buffer[50];
          sprintf(buffer,"%f", (float)(currentContainerWeight - 100.f)); // currentContainerWeight - getCurWeight()
          client.publish("Food Left In Container",buffer);

          Food_is_prepared = true;
          isOpenFunnel = false;

          delay(10); // not so quick
        }
      }
      else
      {
        Serial.println("Opening food tray ...");
        if (!isOpenTray) isOpenTray = true;
        Food_is_out = true;
      }
    }
    else // Food prepared and out to eat
    {
      if (getDistance() > 50 && CallOnce) // the only time that its called
      {
        playSound();
      }
      else if (getDistance() > 50 && !CallOnce) // if it comes and go then start waiting time till close the tray
      {
        if (away_from_food_counter > 0) 
        {
          Serial.println("waiting for pet to return ....");
          away_from_food_counter -= 1;
        }
        else 
        {
          Serial.println("Meal is finished. Upload remain food in tray. Close tray");
          char buffer[50];
          sprintf(buffer,"%d", 37); // getCurWeight()
          client.publish("Food Left In Tray",buffer);

          onFeedTime = false;
          isOpenTray = false;
          away_from_food_counter = Away_from_food_timer; // reset the counter
        }
      }
      else
      {
        stopSound();
        Serial.println("Pet is eating ...");
        // do nothing to serve the pet
        if (CallOnce) CallOnce = false;
      }
    }
  }
  else // reset all the states
  {
    Food_is_out = false;
    Food_is_prepared = false;
    CallOnce = true;
  }
}