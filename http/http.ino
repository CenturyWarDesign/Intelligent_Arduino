
#include "SocketIOClient.h"
#include "Ethernet.h"
#include "SPI.h"
#include <light.h>
//#include "bitlash.h"
SocketIOClient client;
String comdata = "";
int numdata[7] ={0};
int classType;
int interNum;
int oprate;
int data;
int j=0;
int mark=0;
Light light1(4);
Light light2(5);
Light light3(6);
Light light4(7);
Light light5(8);
Light light6(9);
Light lightarr[6]={light1,light2,light3,light4,light5,light6};
//20 is can use ,please send some import message
int sendPollSize=20;
String sendPoll[20];
int sendpollmin=0;
int sendpollmax=0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xCD };
//IPAddress ip(192,168,1,199);
char hostname[] = "192.168.1.31";
//char hostname[] = "42.121.123.185";
//char hostname[] = "192.168.1.107";
//char sec[]="7g941492a0dc743544ebc71c89370a62";
//lijinpeng
char sec[]="7b941492a0dc743544ebc71c89370a61";
//char sec[]="7a941492a0dc743544ebc71c89370a63";

int port = 8080; 

boolean needresert=false;

//rember pmw value
int pmwvalue;

// websocket message handler: do something with command from server
void ondata(SocketIOClient client, char *data) {
    //Serial.print(data);
    comdata=data;
    mark=1;
    Serial.print("comdata:"+comdata);
    inputTostring();

}

void setup() {
  needresert=true;
  pmwvalue=0;
  Serial.println("set up begin");
	Serial.begin(9600);
	Ethernet.begin(mac);
        pinMode(light1.getInter(),OUTPUT);
//        digitalWrite(light1.getInter(),LOW);
        pinMode(light2.getInter(),OUTPUT);
//         digitalWrite(light2.getInter(),LOW);
        pinMode(light3.getInter(),OUTPUT);
//                digitalWrite(light3.getInter(),LOW);
        pinMode(light4.getInter(),OUTPUT);
//                digitalWrite(light4.getInter(),LOW);
        pinMode(light5.getInter(),OUTPUT);
//                digitalWrite(light5.getInter(),LOW);
        pinMode(light6.getInter(),OUTPUT);
//                digitalWrite(light6.getInter(),LOW);
	client.setDataArrivedDelegate(ondata);
        client.setSec(sec);
   Serial.println("set up finish");
   getTem();
}

#define HELLO_INTERVAL 3000UL
#define TEMPERATURE 60000UL
#define SENDRENTI 30000UL
#define ONLINETIME 1000UL
unsigned long lasthello;
unsigned long lasthuoyan;
unsigned long lastlight;
unsigned long lastrenti;
unsigned long sendtemperaturetime;
unsigned long lastonline;

unsigned long lastSendRenti;

void monitorLight(){
  unsigned long now = millis();
  int light=A1;
  int n = 0;
  if ((now - lastlight) >= HELLO_INTERVAL) {
		lastlight = now;
        n=analogRead(light);
        Serial.println("get light:");
        Serial.println(n);
  }
}
void monitorBaojing(){
  unsigned long now = millis();
  int renti=A2;
  int n = 0;
  if ((now - lastrenti) >= HELLO_INTERVAL) {
		lastrenti = now;
        n=digitalRead(renti);
        Serial.println("get renti:");
        Serial.println(n);
        if(n==1){
          if((now - lastSendRenti) >= SENDRENTI) {
		lastSendRenti = now;
                 pushToSend("32_1_3_3");
          }
        }
    }

    //发送温度数据
    if ((now - sendtemperaturetime) >= TEMPERATURE) {
        sendtemperaturetime = now;
        //double temputer=getTem();
          char buffer[6],sendstr[12];
          double tems=getTem();
          dtostrf(tems,2,2,buffer);
          sprintf(sendstr,"30_1_%s_0",buffer);
           Serial.println(sendstr);
          pushToSend(sendstr);   
    } 

}

void loop() {
//   Serial.println(millis());
    if(!client.connected()){
      unsigned long now = millis();
      delay(500);
      Serial.println("try on line");
        client.disconnect();
       if (!client.connect(hostname, port)) Serial.println(F("Not connected."));
        Serial.println("try on line finish");
    }
    
    if(needresert==true){
       pushToSend("00_00_00_00");
       needresert=false;
    }
//  
   //pushToSend("40_2_1_1");
   pushToSendToServer();
   getSerialValue();
  delay(300);
   client.monitor();
  //发送一些监控数据及温度数据
   monitorBaojing();
   monitorLight();

}

//改变发送策略，只有二十个条目池，把最远一个给更新掉
void pushToSend(String sendstr){
    sendPoll[sendpollmax%20]=sendstr;
    sendpollmax++;
}

//发送一些延时发送数据，保证通路正常
void pushToSendToServer(){
    unsigned long now = millis();
    if ((now - lasthello) >= HELLO_INTERVAL) {
        lasthello = now;
        if (client.connected()&&sendpollmax!=sendpollmin) {
            int temReadytoSend=sendpollmin%20;
            if(sendPoll[temReadytoSend].length()!=0)
            {
                char tem[sendPoll[temReadytoSend].length()];
                int i = 0;
                for(; i < sendPoll[temReadytoSend].length() ; i++){
                    tem[i]=(int)sendPoll[temReadytoSend].charAt(i);
                }
                client.send(tem);
                Serial.println("sendpoolmin:");
                Serial.println(sendpollmin);
                Serial.println(tem);
                Serial.println("sendpoolmax:");
                Serial.println(sendpollmax);
                sendPoll[temReadytoSend]="";
            }
            sendpollmin++;
        }
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
        Serial.println(classType);
        Serial.println(interNum);
        Serial.println(oprate);
        Serial.println(data);
        if(classType>0)
        {
            commandcontrol();
        }
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
    char buffer[12];
    sprintf(buffer,"r_%d_%d_%d_%d",classType,interNum,oprate,data); 
    if(classType==10){
        if(oprate==1){
            digitalWrite(lightarr[interNum-1].getInter(),HIGH);
            lightarr[interNum-1].open();
            lightstatus=lightarr[interNum-1].getStatus();
            Serial.println(lightstatus);
        }else if(oprate==0)
        {
            digitalWrite(lightarr[interNum-1].getInter(),LOW);
            lightarr[interNum-1].close();
            lightstatus=lightarr[interNum-1].getStatus();
            Serial.println(lightstatus);
        }else if(oprate==2)
        {
           lightstatus=lightarr[interNum-1].getStatus();
           Serial.println(lightstatus);
        }else
        {
            Serial.println("operate undefine:"+oprate);
        }
      }
      else if(classType==20){
       if(oprate>0){
             pmwopen(interNum,min(500,max(10,data)),oprate);
        }else if(oprate==0){
            pmwclose(interNum,min(500,max(10,data)));
        }
    }
    if (client.connected()) 
    {
        client.send(buffer);
        Serial.println("feedback:");
        Serial.println(buffer);
    }
}


#include <OneWire.h>
 
 
OneWire  ds(A0);  // 连接arduino10引脚
 
double getTem(void) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  ds.search(addr);
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
 
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();

  }
  unsigned int raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // count remain gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw << 3;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw << 2; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw << 1; // 11 bit res, 375 ms
    // default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  return celsius;
}

void pmwopen(int pik,int misocket,int value){
  for(int i=0;i<misocket;i++){
      
      analogWrite(pik,(double)i*(value-pmwvalue)/misocket+pmwvalue);
//      Serial.println((double)i*(value-pmwvalue)/misocket+pmwvalue);
//      if((double)i*(value-pmwvalue)/misocket+pmwvalue<value-3)
//      {
        delayMicroseconds(1000);
//      }
  }
  pmwvalue=value;
  Serial.println("pmw open");
}
void pmwclose(int pik,int misocket){
  for(int i=0;i<misocket;i++){
//    if((double)(misocket-i)*255/misocket>0)
//     {
    delayMicroseconds(1000);
//     }
    analogWrite(pik,(double)(misocket-i)*255/misocket);
    
  }
  Serial.println("pmw close");
}
