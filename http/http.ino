
#include "SocketIOClient.h"
#include "Ethernet.h"
#include "SPI.h"
#include <light.h>
//#include "bitlash.h"
SocketIOClient client;
#define HELLO_INTERVAL 3000UL
#define DEVICE_INTERVAL 1000UL
#define TEMPERATURE 10000UL
#define SENDRENTI 30000UL
#define ONLINETIME 1000UL
unsigned long lasthello;
unsigned long lasthuoyan;
unsigned long lastlight;
unsigned long lastrenti;
unsigned long lasthongwai;
unsigned long sendtemperaturetime;
unsigned long lastonline;

unsigned long lastSendRenti;
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
int sendPollSize=7;
String sendPoll[7];
int sendpollmin=0;
int sendpollmax=0;


//some server set config

int rentiopenpik=9;
int rentilasttime=5000;

int lightopenpik=0;
int lightlasttime=0;

int lightopenvalue=0;
int lightclosevalue=0;

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
int pmwvalue;

// websocket message handler: do something with command from server
void ondata(SocketIOClient client, char *data) {
    comdata=data;
    mark=1;
//    Serial.print("comdata:"+comdata);
    inputTostring();
}

#include <IRremote.h>                    // 引用 IRRemote 函式庫
//const int irReceiverPin = 2;             // 紅外線接收器 OUTPUT 訊號接在 pin 2
IRrecv irrecv(2);            // 定義 IRrecv 物件來接收紅外線訊號
decode_results results;  

void gethongwai(){
   unsigned long now = millis();
  if ((now - lasthongwai) >= HELLO_INTERVAL) 
  {
    lasthongwai = now;
   if (irrecv.decode(&results)) {         // 解碼成功，收到一組紅外線訊號
    String codetype="";
  if (results.decode_type == UNKNOWN) {
      codetype="Unknown";
  } 
  else if (results.decode_type == NEC) {
      codetype="NEC";
  } 
  else if (results.decode_type == SONY) {
        codetype="SNONY";
  } 
  else if (results.decode_type == RC5) {
        codetype="RC5";
  } 
  else if (results.decode_type == RC6) {
        codetype="RC6";
  }
  else if (results.decode_type == PANASONIC) {	
        codetype="PANASONIC";
//    Serial.print("Decoded PANASONIC - Address: ");
//    Serial.print(results.panasonicAddress,HEX);
//    Serial.print(" Value: ");
  }
  else if (results.decode_type == JVC) {
        codetype="JVC";
  }

//   Serial.println(sendstr);    // 紅外線編碼  
   char sendstr[30];
   sprintf(sendstr,"34_%s_%s_0",codetype,results.value,HEX);
   Serial.println(codetype);    // 紅外線編碼
   pushToSend(sendstr);
  
//    Serial.print(",  bits: ");           
//    Serial.println(results.bits);        // 紅外線編碼位元數
    irrecv.resume();                    // 繼續收下一組紅外線訊號        
  }  
  }
}


void setup() {
    needresert=true;
    pmwvalue=0;
//  Serial.println("set up begin");
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
    irrecv.enableIRIn(); 
//   getTem();
}



void monitorLight(){
  unsigned long now = millis();
  int n = 0;
  
  
  if ((now - lastlight) >= SENDRENTI) {
	lastlight = now;
        n=analogRead(A1);
        char sendstr[12];
        sprintf(sendstr,"33_1_%d_0",n);
        pushToSend(sendstr);
  }
}
void monitorBaojing(){
  unsigned long now = millis();
  int n = 0;
    n=digitalRead(A2);
    if(n>=1){
      lastrenti = now+rentilasttime;
      digitalWrite(rentiopenpik,HIGH); 
    }
    
    if(now>lastrenti){
       digitalWrite(rentiopenpik,LOW); 
    }
    
  if(n>=1){
      if((now - lastSendRenti) >= SENDRENTI) {
          Serial.println("send renti:");
	  lastSendRenti = now;
          pushToSend("32_1_3_3");   
        }          
    }


    //发送温度数据
    if ((now - sendtemperaturetime) >= TEMPERATURE) {
        sendtemperaturetime = now;
          char buffer[6],sendstr[12];
          double tems=getTem();
          dtostrf(tems,2,2,buffer);
          sprintf(sendstr,"30_1_%s_0",buffer);
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
    
   //pushToSend("40_2_1_1");
   pushToSendToServer();
   getSerialValue();
   
   client.monitor();
  //发送一些监控数据及温度数据
   monitorBaojing();
   monitorLight();
   delay(1000);
}

//改变发送策略，只有二十个条目池，把最远一个给更新掉
void pushToSend(String sendstr){
    sendPoll[sendpollmax%sendPollSize]=sendstr;
    sendpollmax++;
}

//发送一些延时发送数据，保证通路正常
void pushToSendToServer(){
    unsigned long now = millis();
    if ((now - lasthello) >= HELLO_INTERVAL) {
        lasthello = now;
        if (client.connected()&&sendpollmax!=sendpollmin) {
            int temReadytoSend=sendpollmin%sendPollSize;
            if(sendPoll[temReadytoSend].length()!=0)
            {
                char tem[sendPoll[temReadytoSend].length()];
                int i = 0;
                for(; i < sendPoll[temReadytoSend].length() ; i++){
                    tem[i]=(int)sendPoll[temReadytoSend].charAt(i);
                }
                client.send(tem);
//                Serial.println("sendpoolmin:");
//                Serial.println(sendpollmin);
//                Serial.println(tem);
//                Serial.println("sendpoolmax:");
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
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void printFreeRam() {
  //Serial.print(msg);
  Serial.print(" free RAM: ");
  Serial.println(freeRam());
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
       analogWrite(9,oprate);
    }
    else if(classType==51){
      rentiopenpik=oprate;
    }
     else if(classType==52){
       rentilasttime=oprate;
    }
     else if(classType==53){
       lightopenpik=oprate;
    }
    else if(classType==54){
      lightlasttime=oprate;
    }
     else if(classType==55){
       lightopenvalue=oprate;
    }
    else if(classType==56){
      lightclosevalue=oprate;
    }
    
    if (client.connected()&&classType!=100) 
    {
        client.send(buffer);
//        Serial.println("feedback:");
        Serial.println(buffer);
    }
     printFreeRam();
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
   delay(10);
  }
  pmwvalue=value;
  Serial.println("pmw open");
}
void pmwclose(int pik,int misocket){
  for(int i=0;i<misocket;i++){
    delay(10);
    analogWrite(pik,(double)(misocket-i)*255/misocket);
  }
  Serial.println("pmw close");
}
