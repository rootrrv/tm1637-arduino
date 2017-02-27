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
  Serial.begin(9600);
}

void loop() {

  for (byte t=0;t<123;t++){
	  Serial.print(t);
	  Serial.print(" ");
	  Serial.println(char(t));
	  
	  tm1637D4.displayAscii(1,char(t));
          
          delay(1500);
  }

}
