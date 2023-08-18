#include <WiFi.h>
#include <NTPClient.h>
#include "PubSubClient.h"

#include "loadCell.h"
#include "distanceSensor.h"
#include "dcMotor.h"
#include "buzzer_button.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* host = "maker.ifttt.com";
const char* request_FoodContainer = "/trigger/out_of_food/with/key/dbWqciIC3LLpyxiZtdYBpB";
const char* request_Feeding = "/trigger/reminder/with/key/dbWqciIC3LLpyxiZtdYBpB";
const int port_http = 80;

const char* mqttServer = "test.mosquitto.org";
const int port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

WiFiUDP ntpUD;
NTPClient timeClient(ntpUD);

bool avoid_empty_food_at_first = true;
bool onFeedTime = false;
bool Food_is_out = false;
bool Food_is_prepared = false;
bool CallOnce = true;
int Away_from_food_timer = 100; //900000; // around 15 mins if pet away from eating food
int away_from_food_counter = Away_from_food_timer;
int DroppingTimer = 100;
int DroppingCounter = DroppingTimer;

int FeedingHours[6] = {255,255,255,255,255,255};
int FeedingMins[6] = {255,255,255,255,255,255};
int eat_index = 0;

float currentContainerWeight;
bool hadSentAlarmOfFood = false;

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
    
  }
  if (StringEqual(topic, "Time choice"))
  {
    // Serial.println(stMessage);
    stMessage.remove(0, 1);
    stMessage.remove(stMessage.length() - 1);
    // Serial.println(stMessage); // after remove bracket
    int index = 0;
    // Split the string at each comma and convert substrings to integers
    while (stMessage.length() > 0) 
    {
      int commaIndex = stMessage.indexOf(',');
      if (commaIndex != -1) 
      {
        String timeValue = stMessage.substring(0, commaIndex);
        int colonIndex = timeValue.indexOf(':');
        int colonIndex2 = timeValue.lastIndexOf(":");
        if (colonIndex != -1) 
        {
          String intValueHour = timeValue.substring(1, colonIndex);
          String intValueMin = timeValue.substring(colonIndex + 1, colonIndex2);
          FeedingHours[index] = intValueHour.toInt();
          FeedingMins[index] = intValueMin.toInt();
        }
        stMessage.remove(0, commaIndex + 1);
        // Serial.println(stMessage); // Debug
      } 
      else 
      {
        int colonIndex = stMessage.indexOf(':');
        if (colonIndex != -1) 
        {
          String intValueHour = stMessage.substring(1, colonIndex);
          stMessage.remove(0, colonIndex + 1);
          int colonIndex2 = stMessage.indexOf(':');
          if (colonIndex2 != -1) 
          {
            String intValueMin = stMessage.substring(0, colonIndex2);
            FeedingHours[index] = intValueHour.toInt();
            FeedingMins[index] = intValueMin.toInt();
          }
        }
        stMessage = ""; // Clear the string
      }
      index++;
    }

    // for (int i = 0; i < 6; i++)
    // {
    //   if (FeedingHours[i] == 255) continue;
    //   Serial.print(FeedingHours[i]);
    //   Serial.print(":");
    //   Serial.println(FeedingMins[i]);
    // }
  }
  if (StringEqual(topic, "Cleaning The Machine"))
  {
    if (stMessage == "true")
      Serial.println("Cleaning the machine ......");
    else
      Serial.println("Stop cleaning the machine ......");
  }
  if (StringEqual(topic, "Get Container Weight")) // received every 100ms
  {
    currentContainerWeight = stMessage.toFloat();
    if (avoid_empty_food_at_first) avoid_empty_food_at_first = false;
  }
}

void callback(char* topic, byte* message, unsigned int length)
{
  // Serial.println(topic);
  // Serial.print(": ");
  String stMessage;
  for (int i = 0; i < length; i++)
    stMessage += (char)message[i];
  // Serial.println(stMessage); // print the value

  ExecuteUIorder(topic ,stMessage);
}

void sendRequest(const char* request)
{
  Serial.println("Connectinng to ");
  Serial.println(host);
  Serial.println(": ");
  Serial.println(port_http);

  WiFiClient client;
  while (!client.connect(host, port_http))
  {
    Serial.println("connection fail");
    delay(1000);
  }

  client.print("GET " + String(request) + " HTTP/1.1\r\n" + 
                "Host: " + host + "\r\n" +
                "Connection: close\r\n\r\n");
  delay(500);

  while(client.available())
  {
    String line = client.readStringUntil('\R');
    Serial.println(line);
  }
  Serial.println();
}

String getFormattedDate()
{
  String months[12]={"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  String currentMonthName = months[currentMonth-1];
  int currentYear = ptm->tm_year+1900;
  //Print complete date:
  String currentDate = String(monthDay) + "/" + String(currentMonth) + "/" + String(currentYear);
  return currentDate;
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // hardwares first
  // setup_loadCellCalibration();
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
  Serial.println("Done MQTT set up");
  // sendRequest();

  timeClient.begin();
  timeClient.setTimeOffset(7*3600);
}

void getFeedingTime()
{
  if (timeClient.getHours() == FeedingHours[eat_index] && 
      timeClient.getMinutes() == FeedingMins[eat_index])
  {
    onFeedTime = true;
    // publish bla bla, to the cloud ye ye
  }
  if (eat_index < 6) eat_index++;
  else eat_index = 0;
}
void remind_the_user_aboutFeedTime()
{
  if (timeClient.getFormattedTime() == "08:00:00" || 
      timeClient.getFormattedTime() == "18:00:00"||
      timeClient.getFormattedTime() == "22:00:00")
      sendRequest(request_Feeding);
}

void loop() 
{
  timeClient.update();
  getFeedingTime();
  remind_the_user_aboutFeedTime();

  if(!client.connected()) mqttReconnect();
  client.loop();
  // create content to be publish

  // loop_loadCellCalibration();
  // loop_loadCell();
  // Serial.println(getCurWeight(), 1);

  loopDCMotor();

  // Notification operation
  if (currentContainerWeight < 300 && !hadSentAlarmOfFood && !avoid_empty_food_at_first) // lower than 300 gram and havent sent alarm
  {
    sendRequest(request_FoodContainer); // to Ifttt
    hadSentAlarmOfFood = true;
  }
  if (hadSentAlarmOfFood && currentContainerWeight > 300) 
    hadSentAlarmOfFood = false;

  // Feeding operation
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
          Serial.println("Dropping done.Food is prepared ...");
          DroppingCounter = DroppingTimer;

          // update food left in tray, this thing cause slow
          char buffer[50];
          sprintf(buffer,"%f", (float)(currentContainerWeight - 99.5)); // currentContainerWeight - getCurWeight()
          client.publish("Food Left In Container",buffer);

          sprintf(buffer,"{\"date\":%s,\"time\":%s}",getFormattedDate().c_str(),timeClient.getFormattedTime().c_str());
          client.publish("Data Logging", buffer);

          Food_is_prepared = true;
          isOpenFunnel = false;
          delay(10);
        }
      }
      else
      {
        Serial.println("Opening food tray ...");
        if (!isOpenTray) isOpenTray = true;
        Food_is_out = true;
        Serial.println("Food is out to eat");
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
          Serial.println("Meal is finished. Close tray");

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

  char remainFood[50];
  sprintf(remainFood,"%f", 37.5); // getCurWeight()
  client.publish("Food Left In Tray",remainFood);
}