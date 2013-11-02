
#include "SocketIOClient.h"
#include "Ethernet.h"
#include "SPI.h"
#include <light.h>
//#include "bitlash.h"
SocketIOClient client;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char hostname[] = "192.168.1.110";
int port = 8080;
String comdata = "";
int numdata[7] ={0};
int classType;
int interNum;
int oprate;
int data;
int j=0;
int mark=0;
Light light1(9);
Light light2(10);
Light light3(11);
Light light4(12);
Light light5(13);
Light lightarr[5]={light1,light2,light3,light4,light5};
String sendtoserver="";
// websocket message handler: do something with command from server
void ondata(SocketIOClient client, char *data) {
        comdata=data;
        mark=1;
	//Serial.print("comdata:"+comdata);
        inputTostring();
}
void setup() {
	Serial.begin(9600);
	Ethernet.begin(mac);
	client.setDataArrivedDelegate(ondata);
        client.setSec("7a941492a0dc743544ebc71c89370a61");
        pinMode(light1.getInter(),OUTPUT);
        pinMode(light2.getInter(),OUTPUT);
        pinMode(light3.getInter(),OUTPUT);
        pinMode(light4.getInter(),OUTPUT);
        pinMode(light5.getInter(),OUTPUT);
}

#define HELLO_INTERVAL 3000UL
unsigned long lasthello;

void loop() {
    if(!client.connected()){
      delay(1000);
      client.disconnect();
      if (!client.connect(hostname, port)) Serial.println(F("Not connected."));
    }
    getSerialValue();
    client.monitor();
    unsigned long now = millis();
    if ((now - lasthello) >= HELLO_INTERVAL) {
      lasthello = now;
      if (client.connected()) client.send("hello word!");
    }
    if(sendtoserver.length()>0){
      char tem[50];
      Serial.print("will send to server");
      Serial.print(comdata);
      for(int i = 0; i < comdata.length() ; i++)
      {
        Serial.print(i);
        tem[i]=comdata[i];
      }
      client.send(tem);
      sendtoserver="";
    }
}
void getSerialValue(){
 /* 当串口有数据的时候，将数据拼接到变量comdata */
  while (Serial.available() > 0)
  {
    comdata += char(Serial.read());
    delay(2);
    mark = 1;
  }
  inputTostring();
}
void inputTostring(){
  if(mark == 1)
  {
    Serial.print("You inputed : ");
    Serial.println(comdata);
    for(int i = 0; i < comdata.length() ; i++)
    {
      if(comdata[i] == '_' || comdata[i] == 0x10 || comdata[i] == 0x13)
      {
        j++;
      }
      else
      {
        numdata[j] = numdata[j] * 10 + (comdata[i] - '0');
      }
    }
    classType=numdata[0];interNum=numdata[1];oprate=numdata[2];data=numdata[3];
    
    commandcontrol();
    mark = 0;
    j=0;
    comdata = String("");
    for(int i=0;i<7;i++){
      numdata[i]=0;
    }
  }
}
void commandcontrol(){
 int lightstatus;
if(classType=10){
  if(oprate==1){
    digitalWrite(lightarr[interNum-1].getInter(),HIGH);
    lightarr[interNum-1].open();
     lightstatus=lightarr[interNum-1].getStatus();
    Serial.println(lightstatus);
    sendtoserver="open it";
  }
  else if(oprate==0)
  {
    digitalWrite(lightarr[interNum-1].getInter(),LOW);
      lightarr[interNum-1].close();
     lightstatus=lightarr[interNum-1].getStatus();
        Serial.println(lightstatus);
       sendtoserver="close it";
  }
  else if(oprate==2)
  {
     lightstatus=lightarr[interNum-1].getStatus();
    Serial.println(lightstatus);
    sendtoserver="status it";
    //if (client.connected()) client.send("status it");
  }
  else
  {
    Serial.println("operate undefine:"+oprate);
  }
 
} 
}
