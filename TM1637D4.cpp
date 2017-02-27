/*
 * https://github.com/rootrrv/tm1637-arduino
 *
 * Arduino library for controlling a TM1637 based 7-segment display module.
 * These modules are sold under various names by various suppliers. For example:
 * https://ru.aliexpress.com/wholesale?SearchText=TM1637
 * http://www.dx.com/s/TM1637
 *
 */
/*******************************************************************************/
#include "TM1637D4.h"
#include <Arduino.h>

static int8_t ASCIITab[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    //ASCII 0-122
							0x00,0x63,0x00,0x00,0x00,0x00,0x00,0x00,
							0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,
							0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,
							0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
							0x7f,0x6f,0x00,0x00,0x00,0x00,0x00,0x00,
							0x00,0x77,0x7f,0x39,0x5e,0x79,0x71,0x3d,
							0x76,0x06,0x0E,0x00,0x38,0x00,0x00,0x3f,
							0x73,0x67,0x50,0x6d,0x78,0x3e,0x00,0x00,
							0x00,0x6e,0x00,0x39,0x00,0x0f,0x00,0x08,
							0x02,0x77,0x7c,0x58,0x5e,0x79,0x71,0x6f,
							0x74,0x04,0x0e,0x00,0x38,0x00,0x54,0x5c,
							0x73,0x67,0x50,0x6d,0x78,0x1c,0x00,0x00,
							0x00,0x6e,0x00};

TM1637D4::TM1637D4(uint8_t Clk, uint8_t Data)
{
  Clkpin = Clk;
  Datapin = Data;
  pinMode(Clkpin,OUTPUT);
  pinMode(Datapin,OUTPUT);
}

void TM1637D4::init()
{
  BlankingFlag = 0;
  DecPoint = 3;
  clearDisplay();
}

void TM1637D4::writeByte(int8_t wr_data)
{
  noInterrupts();
  uint8_t i,count1;   
  for(i=0;i<8;i++)        //sent 8bit data
  {
    digitalWrite(Clkpin,LOW);      
    if(wr_data & 0x01)digitalWrite(Datapin,HIGH);//LSB first
    else digitalWrite(Datapin,LOW);
	delayMicroseconds(3);
    wr_data >>= 1;      
    digitalWrite(Clkpin,HIGH);
	delayMicroseconds(3);
      
  }  
  digitalWrite(Clkpin,LOW); //wait for the ACK
  digitalWrite(Datapin,HIGH);
  digitalWrite(Clkpin,HIGH);     
  pinMode(Datapin,INPUT);
  while(digitalRead(Datapin))    
  { 
    count1 +=1;
    if(count1 == 200)//
    {
     pinMode(Datapin,OUTPUT);
     digitalWrite(Datapin,LOW);
     count1 =0;
    }
    pinMode(Datapin,INPUT);
  }
  pinMode(Datapin,OUTPUT);
  interrupts();
}
//send start signal to TM1637
void TM1637D4::start(void)
{
  digitalWrite(Clkpin,HIGH);//send start signal to TM1637
  digitalWrite(Datapin,HIGH);
 // delayMicroseconds(2);
  digitalWrite(Datapin,LOW); 
  digitalWrite(Clkpin,LOW); 
} 
//End of transmission
void TM1637D4::stop(void)
{
  digitalWrite(Clkpin,LOW);
 // delayMicroseconds(2);
  digitalWrite(Datapin,LOW);
//  delayMicroseconds(2);
  digitalWrite(Clkpin,HIGH);
 // delayMicroseconds(2);
  digitalWrite(Datapin,HIGH); 
}

void TM1637D4::display(uint8_t BitAddr,int8_t DispData)
{
  if (BitAddr>=0 and BitAddr<=3) {
  	if (BitAddr == 1 and BlankingFlag) {DispData = DispData|0x80;}
  	start();
  	writeByte(ADDR_FIXED);
  	stop();
  	start();
  	writeByte(BitAddr|0xc0);
  	writeByte(DispData);
  	stop();
  	start();
  	writeByte(Cmd_DispCtrl);
  	stop();
  	_segment[BitAddr]=DispData;
  }
}

void TM1637D4::clearDisplay()
{
  display(0x00,0x00);
  display(0x01,0x00);
  display(0x02,0x00);
  display(0x03,0x00);
}

void TM1637D4::displayAscii(uint8_t BitAddr,char DispChar)
{
	uint8_t asciiCode=DispChar;
	if (asciiCode>=0 and asciiCode<=122) display(BitAddr, ASCIITab[asciiCode]);
}


void TM1637D4::displayNumeral(uint8_t BitAddr,uint8_t DispNumeral)
{
	if (DispNumeral>=0 and DispNumeral<=9) display(BitAddr, ASCIITab[DispNumeral+48]);
}

void TM1637D4::displayWord(String Str){
	int len=Str.length();
	if (len>0){
		char buff[]={' ',' ',' ',' ',' '}; //5 т.к. Str.toCharArray(Bt, 5); возвращает последним символом 0 закрытие подстроки всегда
		Str.toCharArray(buff, len+1); //и поэтому что бы получить 4 сивола строки надо взять выборку 5 символов
		for (int t=0; t<4; t++){
			displayAscii(t, buff[t]);
		}
	} else {clearDisplay();}
}

void TM1637D4::displayTemp(int Temp)
{
	if (Temp > 999) {Temp=999; display(3,0x6b);
	} else if (Temp < -99) {Temp=-99; display(3,0x6b);} 
	else {display(3,0x63);}
	
	if (Temp > 99) {
		 displayAscii(0,char(Temp/100+48));
		 Temp %=100;
		 displayAscii(1,char(Temp/10+48));
		 displayAscii(2,char(Temp%10+48));
	} else if (Temp <-9 or Temp >9){
		if ( Temp <0) {displayAscii(0,'-'); Temp=-Temp;
		} else {display(0, 0x00);}
		 displayAscii(1,char(Temp/10+48));
		 displayAscii(2,char(Temp%10+48));
	} else {
		display(0, 0x00);
		if ( Temp <0) {displayAscii(1,'-'); displayAscii(2,char(-Temp+48));
		} else {display(1, 0x00); displayAscii(2,char(Temp+48));}
	}
}

void TM1637D4::displayNumber(int Num, char Filling)
{
	boolean Sign=false;
	if (Num > 9999) {Num=9999; BlankingFlag=true;
	} else if (Num < -999) {Num=-999; BlankingFlag=true;
	} else {BlankingFlag=false;}
	
	if (Num<0) {
		Sign=true;
		Num=-Num;
	}
	if (Num > 999) {
		displayAscii(0,char(Num/1000+48));
		Num %=1000;
		displayAscii(1,char(Num/100+48));
		Num %=100;
		displayAscii(2,char(Num/10+48));
		displayAscii(3,char(Num%10+48));
	} else if (Num > 99) {
		if (Sign) {displayAscii(0,'-');}
		else{displayAscii(0,Filling);}
		displayAscii(1,char(Num/100+48));
		Num %=100;
		displayAscii(2,char(Num/10+48));
		displayAscii(3,char(Num%10+48));
	} else if (Num >9){
		if (Sign) {displayAscii(0,'-');}
		else{displayAscii(0,Filling);}
		displayAscii(1,Filling);
		displayAscii(2,char(Num/10+48));
		displayAscii(3,char(Num%10+48));
	} else {
		if (Sign) {displayAscii(0,'-');}
		else{displayAscii(0,Filling);}
		displayAscii(1,Filling);
		displayAscii(2,Filling);
		displayAscii(3,char(Num+48));
	}
}

void TM1637D4::displayClock(byte Hours, byte Minutes, boolean Colon)
{
	if (Hours > 23 or Minutes > 59) {displayWord("tErr"); //Display the four minus (tErr)
	}else{
		if (Colon) {BlankingFlag=!BlankingFlag;}
		displayAscii(0,char(Hours/10+48));
		displayAscii(1,char(Hours%10+48));
		displayAscii(2,char(Minutes/10+48));
		displayAscii(3,char(Minutes%10+48));		
	}
}

void TM1637D4::displayStopwatch(byte Minutes, byte Seconds, boolean Colon)
{
	if (Minutes > 59 or Seconds > 59) {displayWord("tErr"); //Display the four minus (tErr)
	}else{
		if (Colon) {BlankingFlag=!BlankingFlag;}
		displayAscii(0,char(Minutes/10+48));
		displayAscii(1,char(Minutes%10+48));
		displayAscii(2,char(Seconds/10+48));
		displayAscii(3,char(Seconds%10+48));		
	}
}


void TM1637D4::setColon(boolean colonFlag)
{
  if (colonFlag) {BlankingFlag=true; _segment[1]=_segment[1]|0x80; display(1,_segment[1]);}
  else {BlankingFlag=false; _segment[1]=_segment[1]&0x7F; display(1,_segment[1]);}
}

void TM1637D4::changeColon()
{
  _segment[1]=_segment[1]^0x80;
  display(1,_segment[1]);
  BlankingFlag=!BlankingFlag;
}

void TM1637D4::setBright(uint8_t sBright)
{
	if (sBright > 3) sBright=3;
	Cmd_DispCtrl = 0x88 + sBright;
	display(0,_segment[0]);
}




