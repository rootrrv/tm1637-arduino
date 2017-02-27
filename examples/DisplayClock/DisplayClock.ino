/*
 * https://github.com/rootrrv/tm1637-arduino
 */

#include <TM1637D4.h>

#define CLK 6
#define DIO 7
TM1637D4 tm1637D4(CLK, DIO);

void setup() 
{
  tm1637D4.init();
  tm1637D4.clearDisplay();
}

void loop() 
{
    for (byte h=0; h<24; h++){
    for (byte m=0; m<60; m++){
      tm1637D4.displayClock(h,m);
    }
  }
  
  for (byte h=0; h<24; h++){
    for (byte m=0; m<60; m++){
    	    for (byte s=0; s<60; s++){
		      tm1637D4.displayClock(h,m,true); //If the third parameter is "true" then the ":" character is constantly changing
      		  delay(1000);
      		}
    }
  }
}
