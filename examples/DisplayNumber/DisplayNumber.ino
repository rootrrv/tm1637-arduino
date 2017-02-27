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
  for (int n=-1020; n<10020; n++) {
    tm1637D4.displayNumber(n);
    //delay (50);
  }

  for (int n=-1020; n<10020; n++) {
    tm1637D4.displayNumber(n,'0'); //Fill with zeros
    //delay (50);
  }

}
