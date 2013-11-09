/*
  Receives Test Events from your phone.
  After it gets a test message the led 13 will blink
  for one second.
*/
 
#include <MeetAndroid.h>

MeetAndroid meetAndroid;
int onboardLed = 5;

void setup()  
{
  // use the baud rate your bluetooth module is configured to 
  // not all baud rates are working well, i.e. ATMEGA168 works best with 57600
  Serial.begin(9600); 
  
  // register callback functions, which will be called when an associated event occurs.
  // - the first parameter is the name of your function (see below)
  // - match the second parameter ('A', 'B', 'a', etc...) with the flag on your Android application
  meetAndroid.registerFunction(testEvent, 'A');  
  Serial.print("BEGIN");
  pinMode(onboardLed, OUTPUT);
  digitalWrite(onboardLed, HIGH);

}

void loop()
{
  meetAndroid.receive(); // you need to keep this in your loop() to receive events
}

/*
 * This method is called constantly.
 * note: flag is in this case 'A' and numOfValues is 0 (since test event doesn't send any data)
 */
void testEvent(byte flag, byte numOfValues)
{
  Serial.println("flag");
  flushLed(300);
  flushLed(300);
}

void flushLed(int time)
{
  digitalWrite(onboardLed, LOW);
  delay(time);
  digitalWrite(onboardLed, HIGH);
  delay(time);
}

