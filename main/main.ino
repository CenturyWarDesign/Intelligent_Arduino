#include <light.h>
#include<string.h>

/*
 * serialToArry.cpp
 *
 *  Created on: 2013-10-17
 *      Author: 俊岭
 */

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
void setup()
{
  Serial.begin(9600);
  pinMode(light1.getInter(),OUTPUT);
    pinMode(light2.getInter(),OUTPUT);
    pinMode(light3.getInter(),OUTPUT);
    pinMode(light4.getInter(),OUTPUT);
    pinMode(light5.getInter(),OUTPUT);
}


void loop()
{

inputTostring();

}

void inputTostring(){
 /* 当串口有数据的时候，将数据拼接到变量comdata */
  while (Serial.available() > 0)
  {
    comdata += char(Serial.read());
    delay(2);
    mark = 1;
  }
  /* 以逗号分隔分解comdata的字符串，分解结果变成转换成数字到[]数组 */
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
//Serial.println("classType:");
//Serial.println(classType);
//Serial.println("interNum:");
//Serial.println(interNum);
//Serial.println("oprate:");
//Serial.println(oprate);
//Serial.println("data:");
//Serial.println(data);
//Serial.print(interNum);
//Serial.print(oprate);
int lightstatus;
if(classType=10){
  if(oprate==1){
    digitalWrite(lightarr[interNum-1].getInter(),HIGH);
    lightarr[interNum-1].open();
     lightstatus=lightarr[interNum-1].getStatus();
  Serial.println(lightstatus);
  }
  else if(oprate==0)
  {
    digitalWrite(lightarr[interNum-1].getInter(),LOW);
    lightarr[interNum-1].close();
     lightstatus=lightarr[interNum-1].getStatus();
  Serial.println(lightstatus);
  }
  else if(oprate==2)
  {
     lightstatus=lightarr[interNum-1].getStatus();
    Serial.println(lightstatus);
  }
  else
  {
    Serial.println("operate undefine:"+oprate);
  }
 
}



    mark = 0;
    j=0;
    comdata = String("");
    for(int i=0;i<7;i++){
      //Serial.println(numdata[i]);
     // Serial.print(0x13);
      numdata[i]=0;
    }
  }

}
