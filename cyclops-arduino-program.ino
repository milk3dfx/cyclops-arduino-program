#include <SoftwareServo.h>

#define MIN_POS 1500
#define MAX_POS 1900

#define LEFT 1
#define RIGHT 2

// Servo motors
#define HORIZONTAL 1
#define VERTICAL 2

#define SERIAL_BUFFER_SIZE 128

//Main headers(0-19)
#define SERIAL_NO 0
#define SERIAL_YES 1
//DC Motors headers(20-29)
#define SERIAL_SETUP_DC_MOTOR 20
#define SERIAL_SET_DC_MOTOR 22
#define SERIAL_DIRECTION_DC_MOTOR 23
#define SERIAL_SPEED_DC_MOTOR 24

//Servo motor headers(30-39)
#define SERIAL_ATTACH_SERVO 30
#define SERIAL_DETACH_SERVO 31
#define SERIAL_SET_SERVO 32

int header[2];
byte body[128];

//DC arrays
int mSpeedPin[128];
int mDirectPin[10];
//Servo array

SoftwareServo mServo[2];

void sendHeader(byte type, byte length){
  Serial.write(type);
  Serial.write(length);
}

void setup(){
  delay(500);
  Serial.begin(9600);
}
void loop(){
  while(true){
    SoftwareServo::refresh();
    if(Serial.available() == 2){
      header[0]=Serial.read();
      header[1]=Serial.read();
      while(true){
        if(Serial.available() == header[1]){
          for(int i=0;i<header[1];i++){
            body[i]=Serial.read();
          }
          break;
        }
      }
      switch(header[0]){
      case SERIAL_SETUP_DC_MOTOR:
            mDirectPin[body[0]]=body[1];
            mSpeedPin[body[0]]=body[2];

            pinMode(body[1], OUTPUT);
            pinMode(body[2], OUTPUT);
        break;
        case SERIAL_SET_DC_MOTOR:
            if(body[1]==LEFT){
              digitalWrite(mDirectPin[body[0]], HIGH);
            }else{
              digitalWrite(mDirectPin[body[0]], LOW);
            }
            analogWrite(mSpeedPin[body[0]], body[2]);
        break;
        case SERIAL_DIRECTION_DC_MOTOR:
            if(body[1]==LEFT)
              digitalWrite(mDirectPin[body[0]], HIGH);
            else
              digitalWrite(mDirectPin[body[0]], LOW);
        break;
        case SERIAL_SPEED_DC_MOTOR:
            analogWrite(mSpeedPin[body[0]], body[1]);
        break;
        //Servo motor
        case SERIAL_ATTACH_SERVO:
           mServo[body[0]].attach(body[1]);
        break;
        case SERIAL_DETACH_SERVO:
           mServo[body[0]].detach();
        break;
        case SERIAL_SET_SERVO:
           mServo[body[0]].write(body[1]);
        break;
      }
    }
  }
}
