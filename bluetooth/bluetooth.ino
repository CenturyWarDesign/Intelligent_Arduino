
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
Light light1(3);
Light light2(4);
Light light3(5);
Light light4(6);
Light light5(7);
Light light6(8);
Light lightarr[6]={light1,light2,light3,light4,light5,light6};
//20 is can use ,please send some import message
int sendPollSize=7;
String sendPoll[7];
int sendpollmin=0;
int sendpollmax=0;


//some server set config

int rentiopenpik=7;
int rentilasttime=5000;

int lighthightpik=4;
int lightlowpik=3;

int lighthighvalue=30;
int lightlowvalue=5;


int temhightpik=6;
int temlowpik=5;

int temthighvalue=22;
int temlowvalue=20;



//char sec[]="7a941492a0dc743544ebc71c89370a63";
int port = 8080; 
boolean needresert=false;
int pmwvalue;





void setup() {
    needresert=true;
    pmwvalue=0;
//  Serial.println("set up begin");
	Serial.begin(9600);
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
   Serial.println("set up finish");
  //    irrecv.enableIRIn(); 
//   getTem();
}




void loop() {
   getSerialValue();
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
       analogWrite(9,oprate);
    }
    else if(classType==51){
      rentiopenpik=oprate;
    }
     else if(classType==52){
       rentilasttime=oprate;
    }
     else if(classType==53){
       lighthightpik=oprate;
    }
    else if(classType==54){
      lightlowpik=oprate;
    }
     else if(classType==55){
       lighthighvalue=oprate;
    }
    else if(classType==56){
      lightlowvalue=oprate;
    }
    
        else if(classType==57){
       temhightpik=oprate;
    }
    else if(classType==58){
      temlowpik=oprate;
    }
     else if(classType==59){
       temthighvalue=oprate;
    }
    else if(classType==60){
      temlowvalue=oprate;
    }
    

    if (client.connected()&&classType!=100) 
    {
        client.send(buffer);
//        Serial.println("feedback:");
        Serial.println(buffer);
    }

}



