int potpin=0;//定义模拟接口0
//int ledpin=11;//定义数字接口11（PWM 输出）
int val=0;// 暂存来自传感器的变量数值
void setup()
{
//pinMode(ledpin,OUTPUT);//定义数字接口11 为输出
Serial.begin(9600);//设置波特率为9600
}
void loop()
{
val=analogRead(potpin);// 读取传感器的模拟值并赋值给val
Serial.println(val);//显示val 变量
//val++;
//if(val>255)
//{
//  val=0;
//}

//analogWrite(ledpin,val);// 打开LED 并设置亮度（PWM 输__________出最大值255）
delay(1000);//延时0.01 秒
}

