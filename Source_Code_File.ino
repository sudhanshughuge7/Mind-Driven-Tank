#define LED 13
#define BAUDRATE 115200
#define DEBUGOUTPUT 0
byte generatedChecksum = 0;
byte checksum = 0; 
int payloadLength = 0;
byte payloadData[64] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;
long lastReceivedPacket = 0;
boolean bigPacket = false;
#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 

int b=3000;
void setup() 
{
  pinMode(LED, OUTPUT);
  pinMode (4,OUTPUT);
  pinMode(5, OUTPUT);
  pinMode (6,OUTPUT);
  pinMode(7, OUTPUT);  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object   
  Serial.begin(57600);
}

byte ReadOneByte()
{
  int ByteRead;
  while(!Serial.available());
  ByteRead = Serial.read();

#if DEBUGOUTPUT  
  Serial.print((char)ByteRead);
#endif
  return ByteRead;
}
void loop()
{
  if(ReadOneByte() == 170) {
    if(ReadOneByte() == 170) {
      payloadLength = ReadOneByte();
      if(payloadLength > 169)  
          return;

      generatedChecksum = 0;        
      for(int i = 0; i < payloadLength; i++)
      {  
        payloadData[i] = ReadOneByte(); 
        generatedChecksum += payloadData[i];
      }   

      checksum = ReadOneByte();         
      generatedChecksum = 255 - generatedChecksum; 

        if(checksum == generatedChecksum)
        {    
        poorQuality = 200;
        attention = 0;
        meditation = 0;

        for(int i = 0; i < payloadLength; i++) 
        {  
          switch (payloadData[i]) {
          case 2:
            i++;            
            poorQuality = payloadData[i];
            bigPacket = true;            
            break;
          case 4:
            i++;
            attention = payloadData[i];                        
            break;
          case 5:
            i++;
            meditation = payloadData[i];
            break;
          case 0x80:
            i = i + 3;
            break;
          case 0x83:
            i = i + 25;      
            break;
          default:
            break;
          } 
        } 

#if !DEBUGOUTPUT
        if(bigPacket)
        {
          
          Serial.println(attention, DEC);
         
          if(attention>=0 && attention < 21){
            digitalWrite(4,LOW);
            digitalWrite(5,LOW);
            digitalWrite(6,LOW);
            digitalWrite(7,LOW);
            myservo.write(90); 
            delay(b);
             
        }
        //FORWARD
          if(attention>20 && attention < 41){
            digitalWrite(4,HIGH);
            digitalWrite(5,LOW);
            digitalWrite(7,HIGH);
            digitalWrite(6,LOW);
            myservo.write(180);                   
            delay(b);
        }
        //RIGHT
          if(attention>40 && attention < 61){
            digitalWrite(4,HIGH);
            digitalWrite(5,LOW);
            digitalWrite(7,LOW);
            digitalWrite(6,HIGH);
            myservo.write(0);                  
            delay(b);
        }
        //LEFT
          if(attention>60 && attention < 81){
            digitalWrite(4,LOW);
            digitalWrite(5,HIGH);
            digitalWrite(7,HIGH);
            digitalWrite(6,LOW);
            myservo.write(180);                
            delay(b);
        }
        //BACKWARD
          if(attention>80){
            digitalWrite(4,LOW);
            digitalWrite(5,HIGH);
            digitalWrite(7,LOW);
            digitalWrite(6,HIGH);
            myservo.write(90);                  
            delay(b);
        }
        }
        
#endif        
        bigPacket = false;        
      }
      else {
        // Checksum Error
      }  
    } 
  } 
}
