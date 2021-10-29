#include <TFT_eSPI.h> 
#include <SPI.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

TFT_eSPI tft = TFT_eSPI();  

#include "7seg20.h"
#include "ani.h"
#include "Orbitron_Medium_16.h"
#define grey 0x65DB

const char* ssid     = "xxxxx";
const char* password = "xxxxxxxx";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 5;


int c=1;
int press3=0;
int press2=0;
bool inv=0;
int frame=0;
int bright[]={12,30,60,100,160};

void setup() {
  pinMode(23,INPUT_PULLUP);
  pinMode(21,INPUT_PULLUP);
 
  tft.init();
  tft.setRotation(3);
  tft.invertDisplay(inv);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);

  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(5, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, bright[c]);

  connectToWifi();
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
   timeClient.setTimeOffset(7200);
   delay(100);
   tft.setTextColor(0xD340,TFT_BLACK);
   
  tft.setFreeFont(&Orbitron_Medium_16);
  tft.drawString("Clock",6,1);
  tft.setTextFont(1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawString("VolosProjects",64,6,1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
 
}


String formattedDate;
String dayStamp;
String timeStamp;
String se="";
String time2="";
String date2="";
String year2="";


void loop() {


while(!timeClient.update()) {
    timeClient.forceUpdate();
  }

 formattedDate = timeClient.getFormattedDate();  
 int splitT = formattedDate.indexOf("T");
 dayStamp = formattedDate.substring(0, splitT); 
 timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  
  tft.setCursor(0, 15);
  tft.setFreeFont(&DSEG7_Classic_Bold_30);
 
if(timeStamp.substring(0,5)!=time2){
 time2=timeStamp.substring(0,5);
 tft.drawString(time2,2,46);
}

 
  tft.setTextColor(grey,TFT_BLACK);
  tft.setFreeFont(&Orbitron_Medium_16);
   if(date2!=dayStamp.substring(5,10))
{
   date2=dayStamp.substring(5,10); 
   tft.drawString(date2,6,22);
}  

   tft.setTextFont(1);

   if(year2!=dayStamp.substring(0,4)){
   year2=dayStamp.substring(0,4);
   tft.drawString(year2,66,26,1);
   }
   tft.setTextColor(TFT_WHITE,TFT_BLACK);

  
 for(int i=0;i<c+1;i++)
 tft.fillRect(146,77-(i*4),8,2, 0x0614); 


      tft.setFreeFont(&Orbitron_Medium_16);
      
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
     if(se!=timeStamp.substring(6,8)){
      se=timeStamp.substring(6,8);
      tft.fillRect(112,61,30,34,TFT_BLACK);
     
     tft.drawString(se,114,62);
     tft.setTextColor(TFT_WHITE,TFT_BLACK);
     }
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.pushImage(112,19,40,40,ani[frame]);
  frame++;
  if(frame==151)
  frame=0;
  delay(20);

  if(digitalRead(23)==0)
    {
    if(press2==0)
      {
        press2=1;
        inv=!inv;
        tft.invertDisplay(inv);
      }
    }else {press2=0;}

      if(digitalRead(21)==0)
    {
    if(press3==0)
      { 
        press3=1;
        c++;
        if(c>=5){
        c=0;
        tft.fillRect(146,60,8,20, TFT_BLACK);
        }
        ledcWrite(pwmLedChannelTFT, bright[c]);
      }
    }else {press3=0;}
  

 
}

void connectToWifi()
  {
    tft.setTextFont(2);
    tft.print("Connecting..");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    tft.print(".");
  }
  // Print local IP address and start web server
  tft.println("");
  tft.println("  WiFi connected.");
  tft.println("  IP address:     ");
  tft.println(WiFi.localIP());
  delay(3200);
  tft.fillScreen(TFT_BLACK);
  }
