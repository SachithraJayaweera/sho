#define F_CPU 8000000UL   /* Define frequency here its 8MHz */
#include<avr/io.h>
#include<util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#define IRDDR DDRA 
#define IRPORT PORTA
#define MOTORDDR DDRC
#define MOTORPORT PORTC
#define PATHDDR DDRC
#define PATHPORT PORTD


#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


void initIrSensor(void);
void initMotor(void);

int readSensor(void);
	
int rightSensor(void);
int rightStopSensor(void);
int leftStoptSensor(void);
int leftSensor(void);

void initPWM(void);
void motorDrive(int m1, int m2);
void turnRight(void);
void stop(void);

void positionOne(void);
void positionTwo(void);
void positionThree(void);
void positionFour(void);

void positonOne_back(void);

unsigned char selectPosition(void);


//long count;
int lastValue;


unsigned char position;
int crosses_2 = 0;
int crosess_3 = 0;


int main(void){

	initIrSensor();
	initMotor();


	
	while(1){
	
	position = selectPosition();

	if (position=='x'){	
	  stop();	
	 
	}else if(position == 'a'){	
	  positionOne();
	
	}else if(position == 'b'){
	  positonOne_back();
	  
	}else if(position == 'f'){
	  stop();
	}
	
 
 }	
	
 return 0;
 
}




void initIrSensor(void){
	IRDDR = 0x00;  //defined portA as input 
	DDRB |= 0X00;  //defined portB as input
	
}

void initMotor(void){

	MOTORDDR |= 0b11000011;
	DDRD |= 0xf0; //defined portD as input for positons
	//PORTD |= 0x0f;// make pull up
	
}


unsigned char selectPosition(void){

unsigned int pinD0, pinD1, pinD2;
unsigned char c;
 

	pinD0 = PIND&(1<<0);
	pinD1 = PIND&(1<<1);
	pinD2 = PIND&(1<<2);
	

	if(pinD0){
	
	c = 'a';
	
	}else if(pinD1){
	
	c = 'b';
	
	}else if(pinD2){
	c = 'c';
	
	}else if(pinD0 && pinD1){
	
	c = 'd';
	
	}else if(pinD0 && pinD2){
	
	c = 'e';
	
	}else if(pinD0 && pinD1 && pinD2){
	
	c = 'f';
	
	}
	
	
	return c;
	
}



int readSensor(void){

unsigned int pin0, pin1, pin2, pin3, pin4, pin5, pin6, pin7;
int x0, x1, x2, x3, x4, x5, x6, x7;

int diff;
	pin0 = 	PINA&(1<<0);
	pin1 = 	PINA&(1<<1);
	pin2 = 	PINA&(1<<2);
	pin3 = 	PINA&(1<<3);
	pin4 = 	PINA&(1<<4);
	pin5 = 	PINA&(1<<5);
	pin6 = 	PINA&(1<<6);
	pin7 = 	PINA&(1<<7);
	
	
	
	if(pin0){x0 = 50;}else{x0 = 0;}
	if(pin1){x1 = 30;}else{x1 = 0;}
	if(pin2){x2 = 15;}else{x2 = 0;}
	if(pin3){x3 = 10;}else{x3 = 0;}
	if(pin4){x4 = -10;}else{x4 = 0;}
	if(pin5){x5 = -15;}else{x5 = 0;}
	if(pin6){x6 = -30;}else{x6 = 0;}
	if(pin7){x7 = -50;}else{x7 = 0;}
	
 int y = x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7;
 
 diff = y+(y-lastValue)*0.5;
 
 lastValue = y;
 
 return diff;

}



void initPWM(void){
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
	DDRB|=(1<<PB3);
	
	TCCR2 = (1<<WGM20) | (1<<WGM21) | (1<<COM21) | (1<<CS20);
	DDRD|=(1<<PD7);

}




void motorDrive(int m1, int m2){

	initPWM();
	
	if(m1>0){
	
		if(m1>255){
			m1=255;
		}		
		MOTORPORT |= (1<<0);
		MOTORPORT &=  ~(1<<1);
		OCR0 = m1;
	
	}else{
		if(m1<-255){
			m1= -255;
		} 
		
		MOTORPORT |=  (1<<1);
		MOTORPORT &=  ~(1<<0);
		OCR0 = m1*-1;
	
	}	
	
	if(m2>0){
	
		if(m2>255){
			m2=255;
		}
		
		MOTORPORT |= (1<<6);
		MOTORPORT &=  ~(1<<7);
		OCR2 = m2;
	
	}else{
		if(m2<-255){
			m2= -255;
		}		
		MOTORPORT |= (1<<7);
		MOTORPORT &=  ~(1<<6);
		OCR2 = m2*-1;	
	}

}




int rightSensor(void){

	unsigned int Rpin;	
	Rpin = 	PINB&(1<<0);
	
	return Rpin;
	
}





int leftSensor(void){
	unsigned int Lpin;
	Lpin = 	PINB&(1<<6);
	
	return Lpin;
}




int rightStopSensor(void){

	unsigned int RStopPin;
	unsigned int Right_Stop = 0;
	
	RStopPin = 	PINB&(1<<1);
	
		if(RStopPin){
	
		Right_Stop = 1;
	
		}else{
		
		Right_Stop = 0;
	
		}
	
	
	return Right_Stop;


}




int leftStoptSensor(void){
	unsigned int LStopPin;
	unsigned int left_Stop = 0;
	
	LStopPin = 	PINB&(1<<4);
	
		if(LStopPin){
	
		left_Stop = 1;
	
		}else{
		
		left_Stop = 0;
	
		}
	
	
	return left_Stop;


}




void stop(void){
	MOTORPORT &= ~(0b11000011);
}




void positionOne(void){

	
	unsigned int leftStop = leftStoptSensor();
	unsigned int left = leftSensor();
	unsigned int rightStop = rightStopSensor();
	unsigned int right = rightSensor();
	
	int err = readSensor();
	
	
	
	if(left){
	
		motorDrive(150, -150);
		
		_delay_ms(50);
	}
	
	
	/*
	else if(right){
	
		motorDrive(-255, 255);
		
		_delay_ms(50);
	}
	*/
	
	else if(leftStop == 0){
		stop();
		
	}
	
	else if(rightStop ==0 && leftStop ==0){
	
	motorDrive(190+err, 190-err);
	
	}
	/*
	else if(rightStop == 0){
		stop();
		
	}
	*/
	
	
	else {
	
	motorDrive(190+err, 190-err);
	
	}
	
}



void positonOne_back(void){

	unsigned int leftStop = leftStoptSensor();
	  unsigned int left = leftSensor();
 	unsigned int rightStop = rightStopSensor();
	unsigned int right = rightSensor();
	
	int err = readSensor();
	
	if(leftStop == 0){
	
		for(int i =0; i<10; i++){
			motorDrive(-150, 150);
			_delay_ms(35);
		}

		
		
	}else if(right){
	
		motorDrive(-150, 150);
		_delay_ms(40);
		
		
	}else if(rightStop == 0){
	
	   stop();
	   
	}else{
	
	   motorDrive(190+err, 190-err);
	   
	}

}




void positionTwo(void){


	
	unsigned int leftStop = leftStoptSensor();
	unsigned int left = leftSensor();
	int err = readSensor();
	
	if(crosses_2 == 0 && left && leftStop){
	
		motorDrive(150, 150);
		_delay_ms(40);
		crosses_2 = crosses_2 + 1;
		
	}
	
	else if(crosses_2 > 0 && left){
	
		motorDrive(250, -250);
		_delay_ms(60);
		
	}
	
	else if(leftStop){
		stop();
		
	}
	
	else{
	
	motorDrive(150+err, 150-err);
	
	}

}




void positionThree(void){


	unsigned int leftStop = leftStoptSensor();
	unsigned int left = leftSensor();
	int err = readSensor();
	
	
	if(crosess_3 == 0 && left && leftStop){
	
		motorDrive(150, 150);
		_delay_ms(40);
		crosess_3 = crosess_3 +1;
		
	}
	
	
	else if(crosess_3 == 1 && left && leftStop){
	
		motorDrive(150, 150);
		_delay_ms(40);
		crosess_3 = crosess_3 +1;
		
	}
	
	
	else if(crosess_3 == 2 && left && err>= -80){
	
		motorDrive(-250, 250);
		_delay_ms(40);
		
	}
	
	else if(leftStop){
		stop();
		
	}
	
	else{
	
	motorDrive(150+err, 150-err);
	
	}

}






void positionFour(void){

int crosess_4 = 0;

	unsigned int leftStop = leftStoptSensor();
	unsigned int left = leftSensor();
	int err = readSensor();
	
	
	
	if(crosess_4 == 0 && left && err<= -80){
	
		motorDrive(150, 150);
		_delay_ms(40);
		crosess_4 = crosess_4+1;
		
	}
	
	
	else if(crosess_4 == 1 && left && err>= -80){
	
		motorDrive(150, 150);
		_delay_ms(40);
		crosess_4 = crosess_4+1;
		
	}
	
	
	else if(crosess_4 == 2 && left && err>= -80){
	
		motorDrive(150, 150);
		_delay_ms(40);
		crosess_4 = crosess_4+1;
		
	}
	
	else if(crosess_4 == 3 && left && err>= -80){
	
		motorDrive(-250, 250);
		_delay_ms(40);
		
	}
	
	else if(leftStop){
		stop();
		
	}else{
	
	motorDrive(150+err, 150-err);
	
	}
	
}


