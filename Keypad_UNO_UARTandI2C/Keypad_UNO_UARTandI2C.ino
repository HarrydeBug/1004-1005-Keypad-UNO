//Software for Zachtek "Customizable Keyboard shield with long stroke keys" and "Customizable Keyboard shield with clicky keys"
//This software is intended to run on a Arduino UNO with one of the above shields attaced to it.

//If a key is pressed, its symbol will be sent out on the Serial port.
//It will aslo be stored waiting for an I2C Query that will trigger an event and report it on I2C to the master

//I2C Portions built upon the example I2C code "Wire Master Reader" by Nicholas Zambetti <http://www.zambetti.com>
//Keyboad decoding portion built upon the example code by Alexander Brevig alexanderbrevig@gmail.com
//ZachTec 2016-2017

//The Version of this software is stored in the constant "softwareversion" and is displayed on the Serialport att startup
//For Arduino Pro Mini.

#include <Keypad.h>
#include <Wire.h>//Use I2C library

const char softwareversion[] = "1.0" ; //Version of this program, sent to serialport at startup

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

  Serial.print("Zachtek Keypad for UNO Software, Version: ");
  Serial.println(softwareversion);
  
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

