/*
 * https://github.com/rootrrv/tm1637-arduino
 */

/*
 *       **0**
 *      *     *
 *      5     1 *
 *      *     *
 *       **6**  7
 *      *     *
 *      4     2 *
 *      *     *
 *       **3**
 *
 * byte b = B00000000;
 *         //--------
 *         //76543210
 */
 
 /*
  *		void init();
		void writeByte(int8_t wr_data);//write 8bit data to tm1637
		void start(void);//send start bits
		void stop(void); //send stop bits
		void display(uint8_t BitAddr,int8_t DispData); //displays the symbol
		void displayAscii(uint8_t BitAddr,char DispChar); //displays the character
		void displayNumeral(uint8_t BitAddr,uint8_t DispNumeral); //displays the Numeral
		void displayWord(String Str); //display the word of 4 characters
		void displayTemp(int Temp); //display  the temperature in Celsius (is -99 to 999 )
		void displayNumber(int Num, char Filling=' '); //display number (is -999 to 9999 )
		void displayClock(byte hours, byte minutes, boolean Colon=false); //display Clock (HOURS:MINUTES). Colon - changes the sign ":"
		void displayStopwatch(byte Minutes, byte Seconds, boolean Colon=false); //display Stopwatch (MINUTES:SECONDS). Colon - changes the sign ":"
		void clearDisplay();
		void setColon(boolean colonFlag);//enables or disables the sign ":"
		void changeColon(); //changes the sign ":" (enables or disables)
		void setBright(uint8_t sBright=2);
  */
  
 
 

#include <TM1637D4.h>

#define CLK 6
#define DIO 7
TM1637D4 tm1637D4(CLK, DIO);

byte b0 = B01110110; //H
byte b1 = B01111001; //E   Стагший бит первого байта отвечает за ":"
byte b2 = B00111000; //L
byte b3 = B00111111; //O

void setup() 
{
  tm1637D4.init();
  tm1637D4.setBright(3);
  tm1637D4.clearDisplay();
  
    //Вывод Hello
  tm1637D4.display(0, b0);
  tm1637D4.display(1, b1);
  tm1637D4.display(2, b2);
  tm1637D4.display(3, b3);
  delay(2000);
}

void loop() 
{
  tm1637D4.displayAscii(0, 'E');
  tm1637D4.displayAscii(1, 'r');
  tm1637D4.displayAscii(2, 'r');
  tm1637D4.displayNumeral(3, 0);
  delay(2000);

  tm1637D4.displayWord("disp");
  delay(2000);
  
  for (int t=-110; t<1020; t++){ //The method shows a temperature of -99 to 999 degrees Celsius
    tm1637D4.displayTemp(t);
    if (t<-50)delay(100);
  }
  
  for (int n=-1020; n<10020; n++){ //The method shows a number from -999 to 9999
    tm1637D4.displayNumber(n);
    if (n<-950)delay(100);
    if (n>0)n=n+21;
  }
  
  byte h=5;
    for (byte m=0; m<60; m++){
      tm1637D4.displayClock(h,m,true); //Если третьим параметром true значит будут менятся двоеточее (загоратся или тухнуть)
	  if (m<15)delay(500);
    }

  h=13;
    for (byte m=0; m<60; m++){
      tm1637D4.displayClock(h,m); //Без изменения двоеточия
	  if (m<15)delay(500);
    }

  byte m=0;
    for (int s=15; s>0; s--){
      tm1637D4.displayStopwatch(m,s,true); //Если третьим параметром true значит будут менятся двоеточее (загоратся или тухнуть)
	  delay(500);
	}
  
  
  tm1637D4.displayWord("----");
  
  tm1637D4.setColon(1);
  delay(500);
  tm1637D4.setColon(0);
  delay(500);
  tm1637D4.setColon(true);
  delay(500);
  tm1637D4.setColon(false);
  delay(1000);
  tm1637D4.changeColon();
  delay(500);
  tm1637D4.changeColon();
  
  tm1637D4.displayWord("br-0");
  for (byte b=0; b<=3; b++){  
    tm1637D4.displayNumeral(3,b);
    tm1637D4.setBright(b);
    delay(1000);
  }
}
