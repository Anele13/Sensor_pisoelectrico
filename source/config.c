#include "p33FJ256GP710.h"
#include "common.h"
#include "lcd.h"
#include "config.h"

void initPuertos(void){
    AD1PCFGL = 0xffff;
    AD1PCFGH= 0xffff;
    
    TRISA = 0xFFFE;
    TRISD = 0xFFFF;
    
    CNEN1bits.CN15IE = 1;
    CNEN2bits.CN16IE = 1;
    CNEN2bits.CN19IE = 1;
    IEC1bits.CNIE = 1;
}

void initProtocolos(void){
    protocoloEnvio[0] = SOF;
    protocoloEnvio[1] = 0x08;   //puede cambiar segun lo que envie pero minimo siempre es 8
    protocoloEnvio[2] = DIRECCION_DESTINO;
    protocoloEnvio[3] = DIRECCION_LOCAL;
    protocoloEnvio[4] = 0x40;
}

void config( void )
{
    //Init protocolos
    initProtocolos();
    
    //Puertos necesarios
    initPuertos();
			
	//UART2
	InitUART2();

	// Timer de uso general
	initTimerGP();
    
    //Reloj
    initTimerRTC();
    
	// LCD
	Init_LCD();    
}
