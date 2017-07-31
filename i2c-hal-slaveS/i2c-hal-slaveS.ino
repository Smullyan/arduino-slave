// Written by Aulia Widyaputra
// 28 July 2017

#include <Wire.h>

float new_percentage;
float last_percentage;

const byte MY_ADDRESS = 42;

// various commands we might get
enum {
    CMD_ID = 1,
    CMD_READ_A0  = 2,
    };

char command;

void setup() 
  {
  // for debugging
  //Serial.begin(9600);
    
  command = 0;
  
  pinMode (A0, INPUT);
  digitalWrite (A0, LOW);  // disable pull-up

  Wire.begin (MY_ADDRESS);
  Wire.onReceive (receiveEvent);  // interrupt handler for incoming messages
  Wire.onRequest (requestEvent);  // interrupt handler for when data is wanted

  }  // end of setup


void loop() 
  {
  // all done by interrupts
  }  // end of loop

void receiveEvent (int howMany)
  {
  command = Wire.read ();  // remember command for when we get request
  } // end of receiveEvent



// TODO try sending only one byte of data.
// That way we can use battery_remaining to display percentage.
void sendSensor (const byte which)
  {
  int val = analogRead (which);
  // 56 and 966 are the min-max values of float
  float percentage = map(val, 56, 966, 100, 0);

  // We choose a slow transient time to prevent violent changes as a result of vibration
  new_percentage = 0.96 * new_percentage + 0.02 * percentage + 0.02 * last_percentage;
  last_percentage = percentage;

  int sent_percentage = round(new_percentage);
  
  byte buf[0];

  buf[0] = sent_percentage;

  //Serial.println(*buf);
  Wire.write (buf, 1);
  
  /*
  byte buf [2];
  
    buf [0] = val >> 8;
    buf [1] = val & 0xFF;
    Wire.write (buf, 2); // 2 byte data code
  */
  }  // end of sendSensor

void requestEvent ()
  {
  switch (command)
    {
     case CMD_ID:      Wire.write (0x55); break;   // send our ID 
     case CMD_READ_A0: sendSensor (A0); break;  // send A0 value
    
     }  // end of switch
  
  }  // end of requestEvent
