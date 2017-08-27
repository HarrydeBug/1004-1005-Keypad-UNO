//I2C Slave arduino that gets input from a matrix keyboard and reports it over I2C wenever asked by a
//an I2C Master (an other Arduino)
//the code is for  Arduino Uno board 
//
//This Code can make an Arduino take sensors (a Keyboard encoder in this case) and make them available on I2C
//This is usefull if you are out of pins on an arduino but have the use of I2C.
//To use this program you need run it on one Arduino and then connect another arduino over I2C (SDA, SCL and GND should be conencted between the two arduinos.)
//On the other Arduino call on adress "I2CSlaveAddr" and ask for 3 bytes back.
//The keys pressed since last call will be returned 
//Built upon the example I2C code "Wire Master Reader" by Nicholas Zambetti <http://www.zambetti.com>
//ZachTec 2016-2017

//   Zachteck 4x3 Keypad demo
//   Sends Keypresses on the UART as soons as they are pressed
//   and acts as a I2C Slave and sends the keypresses when asked for by
//   The I2C MAster



#include <Keypad.h>
#include <Wire.h>//Use I2C library

const byte ROWS = 4; // Five rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'7', '8', '9'},
  {'4', '5', '6'},
  {'1', '2', '3'},
  {'<', '0', 'E'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 5, 4, 3, 2 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 9, 8, 7 };

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define I2CSlaveAddr 9  //Define what I2C adress to act as a slave on

char KeyPres='\0'; //Buffer to store a a Keypres between I2C requests from the master.
boolean I2Cquery=false;

void setup()
{
  pinMode (13,OUTPUT); //Set LED Pin to output;
  KeyPres='\0';
  
  //Set up I2C as a slave and set up Interrups call for incoming data
  Wire.begin(I2CSlaveAddr);   // join I2C bus with address #I2CSlaveAddr
  Wire.onRequest(doI2CEvent); // register event to handle incoming I2C reqests from the I2C Master
  Serial.begin(9600);  //Set up UART
  delay (200);
  //Startup blink
  digitalWrite(13,HIGH); 
  delay(100);
  digitalWrite(13,LOW);
  delay(100);  
  digitalWrite(13,HIGH); 
  delay(100);
  digitalWrite(13,LOW);
}

void loop()
{
  char key = kpd.getKey(); //Check to see i a key has been pressed on the Keyboard
  if (key)
  {
    KeyPres = key; //Store Key in I2CBuffer
    Serial.println(key);
  }

  //Pulse LED if we are queried by I2C
  if (I2Cquery) {
    digitalWrite(13,HIGH); 
    delay(5);
    digitalWrite(13,LOW);
    I2Cquery=false; 
  }
  
}


// Event that executes whenever data is requested by I2C master
void doI2CEvent() {

  Wire.write(KeyPres); // respond with message of 1 byte as expected by master
  KeyPres='\0';
  I2Cquery=true;
}

