
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
Light light1(9);
Light light2(10);
Light light3(11);
Light light4(12);
Light light5(13);
Light lightarr[5]={light1,light2,light3,light4,light5};
//20 is can use ,please send some import message
int sendPollSize=20;
String sendPoll[20];
int sendpollmin=0;
int sendpollmax=0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char hostname[] = "192.168.1.31";
int port = 8080; 

// websocket message handler: do something with command from server
void ondata(SocketIOClient client, char *data) {
    //Serial.print(data);
    comdata=data;
    mark=1;
    Serial.print("comdata:"+comdata);
    inputTostring();

}

//get current temperature
void getTemperature(){
    int n = analogRead(A0);    //读取A0口的电压值
    float vol= n * (5.0 / 1023.0*100);   //使用浮点数存储温度数据，温度数据由电压值换算得到
    char tem[5];
    dtostrf(vol,2,2,tem);
    Serial.println("gemperature");
    Serial.println(tem);
    char returnstr[]="30_1_00_00";
    returnstr[5]=char(tem[0]);
    returnstr[6]=char(tem[1]);
    returnstr[8]=char(tem[3]);
    returnstr[9]=char(tem[4]);
    Serial.println(returnstr);
    client.send(returnstr);
}

void setup() {
    Serial.begin(9600);
    Ethernet.begin(mac);
    pinMode(light1.getInter(),OUTPUT);
    pinMode(light2.getInter(),OUTPUT);
    pinMode(light3.getInter(),OUTPUT);
    pinMode(light4.getInter(),OUTPUT);
    pinMode(light5.getInter(),OUTPUT);
    getTemperature();
    client.setDataArrivedDelegate(ondata);
    client.setSec("7a941492a0dc743544ebc71c89370a61");
}

#define HELLO_INTERVAL 3000UL
#define TEMPERATURE 20000UL
unsigned long lasthello;
unsigned long lasthuoyan;
unsigned long sendtemperaturetime;

void monitorBaojing(){
    unsigned long now = millis();

    int huoyan=A3;
    int n = 0;
    if ((now - lasthuoyan) >= HELLO_INTERVAL) {
       lasthuoyan = now;
       n=analogRead(huoyan);
       Serial.println("get huoyan:");
       Serial.println(n);
       if(n<700){
        pushToSend("31_1_3_3");
        }
    
        //pushToSend("40_1_1_1");
    }

    //发送温度数据
    if ((now - sendtemperaturetime) >= TEMPERATURE) {
        sendtemperaturetime = now;
        getTemperature();
    } 

}

void loop() {
    if(!client.connected()){
        delay(1000);
        client.disconnect();
        if (!client.connect(hostname, port)) Serial.println(F("Not connected."));
    }

    //pushToSend("40_2_1_1");
    pushToSendToServer();
    getSerialValue();
    client.monitor();
    //发送一些监控数据及温度数据
    monitorBaojing();
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
        //Serial.println(classType);
        //Serial.println(interNum);
        //Serial.println(oprate);
        //Serial.println(data);
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
    if(classType=10){
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
    if (client.connected()) 
    {

        client.send(buffer);
        Serial.println("feedback:");
        Serial.println(buffer);
    }

}
