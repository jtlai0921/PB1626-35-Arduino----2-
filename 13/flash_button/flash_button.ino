// A simple web server control with Macromedia Flash by Thomas 2012.4.11

#include "etherShield.h"
#include "ETHER_28J60.h"

int outputPin = 6;

static uint8_t mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};   // this just needs to be unique for your network, 
                                                                // so unless you have more than one of these boards
                                                                // connected, you should be fine with this value.
                                                           
static uint8_t ip[4] = {192, 168, 1, 15};                       // the IP address for your board. Check your home hub
                                                                // to find an IP address not in use and pick that
                                                                // this or 10.0.0.15 are likely formats for an address
                                                                // that will work.

static uint16_t port = 80;                                      // Use port 80 - the standard for HTTP

ETHER_28J60 e;

void setup()
{ 
  e.setup(mac, ip, port);
  pinMode(outputPin, OUTPUT);
}

void loop()
{
  char* params;
  if (params = e.serviceRequest())
  {
    e.print("A0read=");
     e.print(analogRead(1));//print A0 data
    if (strcmp(params, "?cmd=on") == 0)
    {
    digitalWrite(outputPin, HIGH);
    e.print("&ButtonStatus=1"); //print buttonStatus data
    }
    else if (strcmp(params, "?cmd=off") == 0)
    {
      digitalWrite(outputPin, LOW);
    e.print("&ButtonStatus=0");//print buttonStatus data
    }
    e.respond();
  }
}

