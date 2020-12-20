#include <FirebaseArduino.h>
#include <FirebaseHttpClient.h>
#include <FirebaseCloudMessaging.h>
#include <Firebase.h>
#include <FirebaseError.h>
#include <FirebaseObject.h>

#include <ESP8266wifi.h>

 
#include <ESP8266WiFi.h>
#include "ESP_MICRO.h" //Include the micro library 
#include <MQ2.h>
#include <Wire.h>

int Analog_Input = A0;
MQ2 mq2(Analog_Input);
int n = 0;

int lpg, co, smoke;

#define FIREBASE_HOST "pms-demo-b8c47.firebaseio.com"
#define FIREBASE_AUTH "2JJwlljSbXcB9muXZu6aF7WQvb3gfsOEG0mU1Lxr"
                      
#define WIFI_SSID "jayu"
#define WIFI_PASSWORD "12345678jjj"

void setup() 
{
   
  Serial.begin(9600);
  mq2.begin();
  Serial.println("Gas sensor warming up!");
  delay(20000);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


void loop() 
{
  float* values= mq2.read(true); 
  if (Firebase.failed()) 
  {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // set string value
  //lpg = values[0];
  lpg = mq2.readLPG();
  //co = values[1];
  co = mq2.readCO();
  //smoke = values[2];
  smoke = mq2.readSmoke();
  
  Serial.print("LPG: ");
  Serial.println(lpg);
  Firebase.setInt("LPG", lpg);
  
  Serial.print("CO: ");
  Serial.println(co);
  Firebase.setInt("CO", co);
  
  Serial.print("SMOKE: ");
  Serial.println(smoke);
  Firebase.setInt("SMOKE", smoke);
  Serial.println(); 

  delay(1000);
  // handle error
  if (Firebase.failed()) 
  {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  }
