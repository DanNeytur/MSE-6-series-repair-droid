//Receiver Code
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//CE - PIN9
//CSN  - PIN10
//SCK - PIN13
//MOSI - PIN11
//MISO - PIN12
RF24 radio(0, 10); // CE, CSN
const byte address[6] = "00001";

//Left Motors
const int LeftMotorsForward = 3;    // IN1-D3
const int LeftMotorsBackward = 5;   // IN2-D5
//RIght Motors
const int RightMotorsForward = 6;     // IN3-D6
const int RightMotorsBackward = 9;    // IN4-D9

//sound module
const int IO0=8; //IO0-D8
const int IO1=7; //IO1-D7
const int IO2=4; //IO2-D4
const int IO3=2; //IO3-D2
int cnt=0;
int IOX=0;
bool sound_flag=false;
int sound_busy;

//////////////////////////////////////////////////////////////////////////
void setup() 
{
  //disables pins D0 D1 UART0 module
  UCSR0B &= ~(1<<3);
  UCSR0B &= ~(1<<4);
  
  Serial.begin(9600);
  pinMode(RightMotorsForward, OUTPUT);
  pinMode(LeftMotorsForward, OUTPUT);
  pinMode(RightMotorsBackward, OUTPUT);
  pinMode(LeftMotorsBackward, OUTPUT);

  pinMode(IO0, OUTPUT);
  pinMode(IO1, OUTPUT);
  pinMode(IO2, OUTPUT);
  pinMode(IO3, OUTPUT);

  digitalWrite(IO0, HIGH);
  digitalWrite(IO1, HIGH);
  digitalWrite(IO2, HIGH);
  digitalWrite(IO3, HIGH);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

}

////////////////////////////////////////////////////////////////////
void loop() 
{
  if (radio.available()) 
  {
    int data[4];
    radio.read(data, sizeof(data));
    
    //data[0]=LeftJoyX;
    //data[1]=LeftJoyY;
    //data[2]=RightJoyX;
    //data[3]=RightJoyY;
    Serial.print("Left Joystick Y: "); Serial.println(data[1]);
   
    //data Left Joystick Y
    int pwm_val_y;
    if(data[1]>550)
      pwm_val_y=map(data[1],0,1023,0,255);  //map(value, fromLow, fromHigh, toLow, toHigh)
    else pwm_val_y=0;
    
    //data Right Joystick X-FORWARD
    if (data[2] > 550) 
    {
      analogWrite(LeftMotorsBackward,pwm_val_y);
      digitalWrite(LeftMotorsForward, LOW);
      analogWrite(RightMotorsBackward,pwm_val_y);
      digitalWrite(RightMotorsForward, LOW);
    }

    //data Right Joystick X-BACKWARD
    if (data[2] < 400) 
    {
      digitalWrite(LeftMotorsBackward, LOW);
      analogWrite(LeftMotorsForward,pwm_val_y);
      digitalWrite(RightMotorsBackward, LOW);
      analogWrite(RightMotorsForward,pwm_val_y);
    }

    //data Right Joystick Y-TURN RIGHT
    if (data[3] > 550 ) {
      digitalWrite(LeftMotorsBackward, LOW);
      analogWrite(LeftMotorsForward,pwm_val_y);
      analogWrite(RightMotorsBackward,pwm_val_y);
      digitalWrite(RightMotorsForward, LOW);
    }

    //data Right Joystick Y-TURN LEFT
    if (data[3] < 400 ) 
    {
      analogWrite(LeftMotorsBackward,pwm_val_y);
      digitalWrite(LeftMotorsForward, LOW);
      digitalWrite(RightMotorsBackward, LOW);
      analogWrite(RightMotorsForward,pwm_val_y);
    }

      //Right Joystick in center-STOP
     if (data[2] < 550 && data[2] > 400 && data[3] < 550 && data[3] > 400) 
     {
      digitalWrite(RightMotorsForward, LOW);
      digitalWrite(RightMotorsBackward, LOW);
      digitalWrite(LeftMotorsForward, LOW);
      digitalWrite(LeftMotorsBackward, LOW);
    }
    
  }

  sound_busy=analogRead(A0);
//  Serial.print("IOX=");Serial.println(IOX);
//  Serial.print("sound_busy=");Serial.println(sound_busy);
//  Serial.print("cnt=");Serial.println(cnt);
  
  cnt++;
  
  if(sound_busy>600 and cnt>=10)//no sound playing
  {
    cnt=0;
    sound_flag=true;
  }

  if(sound_busy<600 and cnt==10)//sound is played
  {
    sound_flag=false;
    digitalWrite(IO0, HIGH);
    digitalWrite(IO1, HIGH);
    digitalWrite(IO2, HIGH);
    digitalWrite(IO3, HIGH);    
    IOX++;
    if(IOX==4) IOX=0;
  }
    
  if(sound_flag==true)
  {
   switch(IOX)
    {
      case 0: //play audio 00001
        digitalWrite(IO0, LOW);
        digitalWrite(IO1, HIGH);
        digitalWrite(IO2, HIGH);
        digitalWrite(IO3, HIGH);
        break;
       case 1://play audio 00002
        digitalWrite(IO0, HIGH);
        digitalWrite(IO1, LOW);
        digitalWrite(IO2, HIGH);
        digitalWrite(IO3, HIGH);
        break;
       case 2://play audio 00003
        digitalWrite(IO0, HIGH);
        digitalWrite(IO1, HIGH);
        digitalWrite(IO2, LOW);
        digitalWrite(IO3, HIGH);
        break;
       case 3://play audio 00004
        digitalWrite(IO0, HIGH);
        digitalWrite(IO1, HIGH);
        digitalWrite(IO2, HIGH);
        digitalWrite(IO3, LOW);
        break;
    }
  }  
 }
