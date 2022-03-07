#include	<pic.h>
#include	<htc.h>
#include	"delay.h"
__CONFIG(0x3F32);

#define ON RC0
#define STDBY RC3

void inicializar(void);
void enviarCanal(int canal);
void usartTx(char dato);
char usartRx(void);
void main(void)
{
	inicializar();
	ON=1;
	while(1)
	{
		STDBY=1;
	}
}

void inicializar(void)
{
	INTCON=0b01000000;
	RCSTA=0b10010000;
	OPTION=0;
	TRISA=0b00111111;
	TRISB=0b11111111;
	TRISC=0b10000000;
	TRISD=0b11110000;
	TRISE=0b00000111;
	PIE1=0x20;
	PIE2=0;
	TXSTA=0b00100100;
	SPBRG=10;
	PR2=255;
	CMCON=0x07;
	CVRCON=0;
	ADCON1=128;
	PORTA=0;
	PORTC=0;
	PORTD=0;
	CCPR1L=0;
	CCPR2L=0;
	T2CON=0b00000100;
	CCP1CON=0b00001100;
	CCP2CON=0b00001100;
	ei();
}

void interrupt rsi(void)
{
	char instruccion, arg1, arg2;
	instruccion=RCREG;
	STDBY=0;
	switch(instruccion)
	{
		case 3:
			usartTx(PORTB);
			//usartTx(10);
			break;
		case 4:
			arg1=usartRx();
			PORTC=(PORTC&0b11001111)|(arg1&0b00110000);
			break;
		case 10:
			enviarCanal(0);
			enviarCanal(1);
			//usartTx(10);
			break;
		case 11:
			enviarCanal(1);
			enviarCanal(2);
			//usartTx(10);
			break;
		case 12:
			enviarCanal(2);
			enviarCanal(3);
			//usartTx(10);
			break;
		case 13:
			enviarCanal(3);
			enviarCanal(4);
			//usartTx(10);
			break;			
		case 14:
			enviarCanal(4);
			enviarCanal(5);
			//usartTx(10);
			break;			
		case 15:
			enviarCanal(5);
			enviarCanal(6);
			//usartTx(10);
			break;
		case 16:
			enviarCanal(6);
			enviarCanal(7);
			//usartTx(10);
			break;
		case 20:
			enviarCanal(0);
			//usartTx(10);
			break;
		case 21:
			enviarCanal(1);
			//usartTx(10);
			break;
		case 22:
			enviarCanal(2);
			//usartTx(10);
			break;
		case 23:
			enviarCanal(3);
			//usartTx(10);
			break;
		case 24:
			enviarCanal(4);
			//usartTx(10);
			break;
		case 25:
			enviarCanal(5);
			//usartTx(10);
			break;
		case 26:
			enviarCanal(6);
			//usartTx(10);
			break;
		case 27:
			enviarCanal(7);
			//usartTx(10);
			break;
		case 30:
			usartTx(RB0);
			//usartTx(10);
			break;
		case 31:
			usartTx(RB1);
			//usartTx(10);
			break;
		case 32:
			usartTx(RB2);
			//usartTx(10);
			break;
		case 33:
			usartTx(RB3);
			//usartTx(10);
			break;
		case 34:
			usartTx(RB4);
			//usartTx(10);
			break;
		case 35:			
			usartTx(RB5);
			//usartTx(10);
			break;
		case 36:
			usartTx(RB6);
			//usartTx(10);
			break;
		case 37:
			usartTx(RB7);
			//usartTx(10);
			break;
		case 41:
			arg1=usartRx();
			arg2=usartRx();
			CCPR2L=arg1;
			CCP2CON=(CCP2CON & 0b11001111)|((arg2>>2)& 0b00110000);
			break;
		case 42:
			arg1=usartRx();
			arg2=usartRx();
			CCPR1L=arg1;
			CCP1CON=(CCP1CON & 0b11001111)|((arg2>>2)& 0b00110000);
			break;
		case 44:
			RC4=(usartRx() & 1);
			break;
		case 45:
			RC5=(usartRx() & 1);
			break;
		case 50:
			usartTx(PORTD & 0b11110000);
			//usartTx(10);
			break;
		case 51:
			arg1=usartRx();
			PORTD=(PORTD & 0b11110000)|(arg1 & 0b00001111);
			break;
		case 60:
			RD0=(usartRx() & 1);
			break;
		case 61:
			RD1=(usartRx() & 1);
			break;
		case 62:
			RD2=(usartRx() & 1);
			break;
		case 63:
			RD3=(usartRx() & 1);
			break;
		case 64:
			usartTx(RD4);
			//usartTx(10);
			break;
		case 65:
			usartTx(RD5);
			//usartTx(10);
			break;
		case 66:
			usartTx(RD6);
			//usartTx(10);
			break;
		case 67:
			usartTx(RD7);
			//usartTx(10);
			break;
		case 101:
			usartTx(99);
			//usartTx(10);
			break;
	}
}

void enviarCanal(int canal)
{
	if(canal<8)
	{
		ADCON0=(0b10000001|(canal<<3));
		DelayUs(20);
		ADGO=1;
		while(ADGO){}
		usartTx(ADRESH);
		usartTx(ADRESL);
	}
}

void usartTx(char dato)
{
	while(!TXIF){}
	TXREG=dato;
}

char usartRx(void)
{
	int i=0;
	while(!RCIF && i<31250)
	{
		i++;
	}
	return RCREG;	
}