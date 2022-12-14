#include "CTBot.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

CTBot myBot;

String ssid = "Teras";     // REPLACE mySSID WITH YOUR WIFI SSID
String pass = "tanyacendi"; // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = "5231184432:AAExY9_Cg4JNC-WM_Nvl2HOsUfXKNFie1RY";   // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

TBMessage msg;
const long utcOffsetInSeconds = 25200;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
unsigned long int j=0, m=0, s=0;

#define relay1 16
#define relay2 5
#define relay3 4
#define relay4 0
#define relay5 2
#define relay6 14
#define relay7 12
#define relay8 13
bool pagerstat=0, garasistat=0, terasstat=0;

void setup(){
  Serial.begin(115200);
  Serial.println("Starting TelegramBot...");

  WiFi.begin(ssid, pass);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  myBot.setTelegramToken(token);

  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection NOK");
    
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  pinMode(relay7, OUTPUT);
  pinMode(relay8, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);
  digitalWrite(relay7, HIGH);
  digitalWrite(relay8, HIGH);
  timeClient.begin();
  timeClient.update();
}

void loop(){
 
  
  j = timeClient.getHours();
  m = timeClient.getMinutes();
  s = timeClient.getSeconds();
  s++;
  
  if (s==60){
    s=0;
    m+=1;
  }
  
  if (m==60){
    m=0;
    j+=1;
  }
  
  if (j==24){
    j=0;
  }
  if (myBot.getNewMessage(msg)) {
    if (msg.text.equalsIgnoreCase("/start")) {       
      String welcome = "Badhe dhawuh menapa?\n";
        welcome+= "/Bukapager\n";
        welcome+= "/Tutuppager\n";
        welcome+= "/LamputerasON\n";
        welcome+= "/LamputerasOFF\n";
        welcome+= "/LampugarasiON\n";
        welcome+= "/LampugarasiOFF\n";
      myBot.sendMessage(msg.sender.id, welcome);
    }
    if (msg.text.equalsIgnoreCase("/Bukapager")) {
      if (pagerstat) myBot.sendMessage(msg.sender.id, "Pager sudah terbuka");
      else {
         digitalWrite (relay3, LOW);
        delay(2000);
        digitalWrite (relay3, HIGH);
        digitalWrite (relay1, LOW);
        delay(5000);
        digitalWrite (relay1, HIGH);      
        myBot.sendMessage(msg.sender.id, "Pager terbuka");
        pagerstat= !pagerstat;
      }
  }
  if (msg.text.equalsIgnoreCase("/Tutuppager")) {
      if (pagerstat){
        digitalWrite (relay2, LOW);
        delay(5000);
        digitalWrite (relay2, HIGH);
        digitalWrite (relay4, LOW);
        delay(2000);
        digitalWrite (relay4, HIGH);            
        myBot.sendMessage(msg.sender.id, "Pager tertutup dan terkunci");
        pagerstat= !pagerstat;
      }
      else {
        myBot.sendMessage(msg.sender.id, "Pager sudah tertutup");
      }
  }
  if (msg.text.equalsIgnoreCase("/LamputerasON")) {
    if (terasstat) myBot.sendMessage(msg.sender.id, "Lampu teras sudah aktif");
    else {
    digitalWrite (relay5, LOW);
    myBot.sendMessage(msg.sender.id, "Lampu teras aktif");
    terasstat= !terasstat;
    }
  }
  if (msg.text.equalsIgnoreCase("/LamputerasOFF")) {
    if (terasstat){
      digitalWrite (relay5, HIGH);
      myBot.sendMessage(msg.sender.id, "Lampu teras mati");
      terasstat= !terasstat;
    }
    else {
      myBot.sendMessage(msg.sender.id, "Lampu teras sudah mati");
    }
  }
  if (msg.text.equalsIgnoreCase("/LampugarasiON")) {
    if (garasistat) myBot.sendMessage(msg.sender.id, "Lampu garasi sudah aktif");
    else {
    digitalWrite (relay6, LOW);
    myBot.sendMessage(msg.sender.id, "Lampu garasi aktif");
    garasistat= !garasistat;
    }
  }
  if (msg.text.equalsIgnoreCase("/LampugarasiOFF")) {
    if (garasistat){
      digitalWrite (relay6, HIGH);
      myBot.sendMessage(msg.sender.id, "Lampu garasi mati");
      garasistat= !garasistat;
    }
    else {
      myBot.sendMessage(msg.sender.id, "Lampu garasi sudah mati");
    }
  }
 }
    if (j == 22 && m == 0 && s == 0){
      if (terasstat){
       digitalWrite (relay5, HIGH);
       terasstat= !terasstat;
      }
      else Serial.print("aman");
    }
    if (j == 17 && m == 0 && s == 0) {
      if (terasstat){
        if (garasistat){
          Serial.print("aman");
        }
        else {
          digitalWrite (relay6, LOW);
          garasistat= !garasistat;
        }
      }
      else {
        digitalWrite (relay5, LOW);
        terasstat= !terasstat;
        if (garasistat){
          Serial.print("aman");
        }
        else {
          digitalWrite (relay6, LOW);
          garasistat= !garasistat;
        }
      }
    }
    if (j == 5 && m == 30 && s == 0) {
      if (garasistat){
        digitalWrite (relay6, HIGH);
        garasistat= !garasistat;
      }
      else Serial.print("aman");
    }
}
