int potpin=0;//定义模拟接口0
int ledpin=11;//定义数字接口11（PWM 输出）
int val=0;// 暂存来自传感器的变量数值
void setup()
{
pinMode(ledpin,OUTPUT);//定义数字接口11 为输出
//pinMode(potpin,INPUT);//定义数字接口11 为输出
Serial.begin(9600);//设置波特率为9600
//注意：模拟接口自动设置为输入
}
void loop()
{
  
 val= analogRead(potpin);
Serial.println(val);//显示val 变量
//analogWrite(ledpin,val);// 打开LED 并设置亮度（PWM 输__________出最大值255）
delay(100);//延时0.01 秒
}

