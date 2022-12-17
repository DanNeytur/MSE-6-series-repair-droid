//Transmitter code
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <AFMotor.h>

//CE - PIN9
//CSN  - PIN10
//SCK - PIN13
//MOSI - PIN11
//MISO - PIN12

//Left joystick-speed
#define joyLeftY A0 //left joystick Y-A0
#define joyLeftX A1 //left joystick X-A1

//Right joystick-direction
#define joyRightY A2 //right joystick Y-A2
#define joyRightX A3 //right joystick X-A3

int data[4];

RF24 radio(9, 10); // CE, CSN

const byte address[6] = "00001";

void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() 
{
  int LeftJoyX=analogRead(joyLeftX);
  int LeftJoyY=analogRead(joyLeftY);
  int RightJoyX=analogRead(joyRightX);  
  int RightJoyY=analogRead(joyRightY);
  
  data[0]=LeftJoyX;
  data[1]=LeftJoyY;
  data[2]=RightJoyX;
  data[3]=RightJoyY;
  
  Serial.print("      Left Joystick X: "); Serial.print(LeftJoyX);
  Serial.print(" Y: "); Serial.println(LeftJoyY);
  Serial.print(" Right Joystick X: "); Serial.print(RightJoyX);
  Serial.print(" Y: "); Serial.print(RightJoyY);
  radio.write(data, sizeof(data)); 
 
}
