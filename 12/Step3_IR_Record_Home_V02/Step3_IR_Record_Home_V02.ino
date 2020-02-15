//IR remote for TV
//By Leo 2013.02.19
//http://weibo.com/leolite

//Type
//LG 液晶電視 AKB72915211
//Hisense 機頂盒 SD-16127

#include <IRremote.h>
// #include <EEPROM.h>

#include "IR_Receive.h"
#include "IR_Send.h"
#include "IR_Value.h"
#include "Keyboard.h"
#include "Mapping.h"

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);
IRsend irsend;

decode_results results;
unsigned long date;

int addr = 0; //for EEPROM
int index = 0; //for array
int mapto = 0; //fir mapping
int led = 13;

void setup()
{
  Serial.begin(9600);
  // make pin 2 an input and turn on the 
  // pullup resistor so it goes high unless
  // connected to ground:
  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);   
  pinMode(led, OUTPUT);
  digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(3000);               // wait for a second
  digitalWrite(3, LOW);    // turn the LED off by making the voltage LOW


    //  Keyboard.begin();
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  irrecv.enableIRIn(); // Start the receiver
  light(500);
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
      mapto = mapping[index];
      Serial.print("SendIR: ");
      Serial.println(ir_code_s[mapto], HEX);
      delay(100); // Wait
      irsend.sendNEC(ir_code_s[mapto], 32); //NEC 32 bits
      flash(200,3);
    }

    //Block 1 end

    irrecv.resume(); // Receive the next value
  }

}

void flash(int n,int m){
  for(int i=0;i<m;i++){
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(n);               // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(n); 
  }
}
void light(int n){
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(n);               // wait for a second
}




