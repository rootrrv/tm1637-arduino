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
  
  tm1637D4.displayWord("---0");
}

void loop() 
{
  for (byte b=0; b<=3; b++){  
    tm1637D4.displayNumeral(3,b);
    tm1637D4.setBright(b);
    delay(1000);
  }
}
