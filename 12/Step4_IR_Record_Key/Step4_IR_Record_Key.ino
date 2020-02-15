//IR remote for PC
//http://weibo.com/leolite
//By Leo 2013.02.19


#include <IRremote.h>
// #include <EEPROM.h>

#include "IR_Receive.h"
#include "IR_Send.h"
#include "IR_Value.h"
#include "Keyboard.h"
#include "Mapping.h"

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;
unsigned long date;

int addr = 0; //for EEPROM
int index = 0; //for array
int mapto = 0; //for mapping

void setup()
{
  Serial.begin(9600);
  // make pin 2 an input and turn on the 
  // pullup resistor so it goes high unless
  // connected to ground:
  pinMode(2, INPUT_PULLUP);
  Keyboard.begin();
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {


    //Block 1 start
    //search from IR_Reveive.h and return index
    if(results.value != ir_code_r[0]){
      for(int i=0;i<256;i++){
        if( results.value== ir_code_r[i]){
          index = i;
          break;
        } 
      }
    }

    //Block 1 end

    //Bolck 2 start
    //test function,look up IR_Value.h
    //        Serial.println(results.value, HEX);
    //       Serial.println(index);
    //      Serial.println(ir_code_v[index]);
    //Block 2 end

    //block 3 mapping
    mapto = mapping[index];
   // Serial.println(key_define[mapto],HEX);
    //block 3 end

    //Send the message
    Keyboard.write(key_define[mapto]);

    irrecv.resume(); // Receive the next value
  }
}














