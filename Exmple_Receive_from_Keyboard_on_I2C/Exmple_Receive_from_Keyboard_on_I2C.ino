//This is a I2C Master software that receives I2C data from a Keyboard
//The Keyboard Shield sits on a UNO and  acts as a I2C slave and waits for this program to query it.
//This Software runs on a separate Arduino with the two connected over I2C (SDA, SCL and GND should be conencted between the two arduinos.)
//The Keyboard Arduino awaits queries on adress "I2CSlaveAddr" and reports back the last Key pressed.
//Built upon the example I2C code "Wire Master Reader" by Nicholas Zambetti <http://www.zambetti.com>
//ZachTec 2016-2017

#include <Wire.h>
char KeyIn='\0';                               // Char to hold received Keyboard input
boolean KeyBoardConnected=false;
boolean LastConnectState=false;

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output

  Serial.println("Zachtek I2C Master that Queries a Keypad on adress 9");
  Serial.println("");
  
  //Startup blink
  digitalWrite(13,HIGH); 
  delay(100);
  digitalWrite(13,LOW);
  delay(100);  
  digitalWrite(13,HIGH); 
  delay(100);
  digitalWrite(13,LOW);
}


  
void loop() {
unsigned long I2CTimeOut; 
boolean Notreceived;
  
   //Check Keyboard
   KeyIn='\0';
   Notreceived=true;
   Wire.requestFrom(9, 1);    // request 1 bytes from slave device #9 (Keyboard)
   I2CTimeOut=millis()+100;  //Timeout if not all bytes are received in 100mS
  while (Notreceived && millis()<I2CTimeOut ) {//until all data is received or TimeOut is reached.
    while (Wire.available()) { // call may return with less than requested so keep on filling buffer
      KeyIn = Wire.read(); // receive a byte as character  
      Notreceived=false;
    }
  }
  if (millis ()>=I2CTimeOut) 
  {
    KeyBoardConnected=false;
    //Serial.println("Keyboard I2C TIME OUT");
  }
  else
    {//We recevied data from I2C slave
     KeyBoardConnected=true;
    if (KeyIn != '\0') 
    {    
      Serial.print(KeyIn); //Print the received data
    }  
  }
  delay (50); //Query 20 times per second
  if (KeyBoardConnected!=LastConnectState)
  {
    //if the Keyboard was pluged or unplugged then write Status Message
    LastConnectState=KeyBoardConnected;
    if (KeyBoardConnected) 
    {
      Serial.println("Keyboard connected"); 
    }
    else
    {
     Serial.println(""); 
     Serial.println("Keyboard not connected"); 
    }
  }
}


