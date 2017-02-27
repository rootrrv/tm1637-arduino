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
  tm1637D4.set(6);
  tm1637D4.clearDisplay();
}

void loop() 
{
  for (int n=-120; n<1020; n++) {
    tm1637D4.displayTemp(n);
    delay (100);
  }
}
