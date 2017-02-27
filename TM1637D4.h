//-----------------------------------------------
/*
 * https://github.com/rootrrv/tm1637-arduino
 *
 * Arduino library for controlling a TM1637 based 7-segment display module.
 * These modules are sold under various names by various suppliers. For example:
 * https://ru.aliexpress.com/wholesale?SearchText=TM1637
 * http://www.dx.com/s/TM1637
 *
 *       **0**
 *      *     *
 *      5     1  *
 *      *     *
 *       **6**   7
 *      *     *
 *      4     2  *
 *      *     *
 *       **3**
 *
 * byte b = B00000000;
 *         //--------
 *    //bits 76543210
 *
 *  //The eighth bit of the first address shows the symbol ":"
 *
 */
	
#ifndef TM1637D4_h
#define TM1637D4_h

//************definitions for TM1637*********************
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44
#define STARTADDR  0xc0

#include <inttypes.h>
#include <Arduino.h>

	class TM1637D4
	{
	  public:
		uint8_t Cmd_DispCtrl;
		TM1637D4(uint8_t, uint8_t);
		void init(); //Initializes the display
		void writeByte(int8_t wr_data);//write 8bit data to tm1637
		void start(void);//send start bits
		void stop(void); //send stop bits
		void display(uint8_t BitAddr,int8_t DispData); //displays the symbol
		void displayAscii(uint8_t BitAddr,char DispChar); //displays the character
		void displayNumeral(uint8_t BitAddr,uint8_t DispNumeral); //displays the Numeral
		void displayWord(String Str); //display the word of 4 characters
		void displayTemp(int Temp); //display  the temperature in Celsius (is -99 to 999 )
		void displayNumber(int Num, char Filling=' '); //display number (is -999 to 9999 )
		void displayClock(byte Hours, byte Minutes, boolean Colon=false); //display Clock (HOURS:MINUTES). Colon - changes the sign ":"
		void displayStopwatch(byte Minutes, byte Seconds, boolean Colon=false); //display Stopwatch (MINUTES:SECONDS). Colon - changes the sign ":"
		void clearDisplay(); //Clear display
		void setColon(boolean colonFlag);//enables or disables the sign ":"
		void changeColon(); //changes the sign ":" (enables or disables)
		void setBright(uint8_t sBright=2); //Set bright (0..3)
	  private:
		uint8_t Clkpin;
		uint8_t Datapin;
		uint8_t DecPoint;
		boolean BlankingFlag;
		uint8_t _segment[4];
	};
#endif
