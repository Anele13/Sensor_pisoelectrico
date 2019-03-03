#include "p33FJ256GP710.h"
#include "config.h"

void restablecerValores(void){
    velocidad=0;    
    cantidadEjes=0;
    contadorCableA=0;
    contadorCableB=0;
    contadorTMR6 = 0;
    bloqueado=FALSE;
}

void calcularVelocidad(void){
    float respuesta = 10.0 / 1000.0;   //
    float respuesta2 = respuesta / 3600.0;
    float aux = 0.0003/respuesta2;
    velocidad = aux;
    if(velocidad>0x3C)
        encenderCamara();
}

int primerCable(void){
    return !PORTDbits.RD6;
}

int segundoCable(void){
    return !PORTDbits.RD7;
}
int hayUnAuto(void){
    return !PORTDbits.RD13;
}

void reiniciarTimer(void){
    if(!T6CONbits.TON && !bloqueado){
        T6CONbits.TON = 1;
    }
}

void apagarTimer(void){
    if(T6CONbits.TON){
        T6CONbits.TON = 0;
        TMR6 = 0x0000;
        bloqueado = TRUE;
        calcularVelocidad();
    }    
}

int pasoElTest(void){
    if(contadorCableA == contadorCableB){
       if(contadorCableA >= 2)
           return TRUE;       
       else
           return FALSE;
    }
    return FALSE;
}

void __attribute__((interrupt, auto_psv)) _CNInterrupt( void ) {
    IFS1bits.CNIF = 0;
    
    if(hayUnAuto()){
        if(primerCable() && !segundoCable()){
            reiniciarTimer();
            contadorCableA++;
        }            
        if (segundoCable() && !primerCable()){
            apagarTimer();
            contadorCableB++;
        }
    }
    else{
        if(pasoElTest()){
            grabarBitacora();
            contadorAutos++;
            hayNuevosDatos=TRUE;
        }            
        restablecerValores();
    }
}

void __attribute__((interrupt, auto_psv)) _T6Interrupt( void ){
    IFS2bits.T6IF = 0;
    contadorTMR6++;
}

void __attribute__((interrupt, auto_psv)) _T4Interrupt( void )
{
	 IFS1bits.T4IF = 0;
     counterTog++;
     if (counterTog > 3){
        counterTog =0;
        segundos++;
        if(segundos==60){
            segundos=0;
            minutos++;
            if(minutos==60){
                minutos=0;
                hora++;
                if(hora==24){
                    hora=0;
                }
            }
        }
        enviarDatos();
    }
}

void initTimerGP(void){
/* ensure Timer 6 is in reset state */
    T6CON = 0;
    T6CONbits.TCKPS = 1; //Prescaler 8

    /* reset Timer 6 interrupt flag */
    IFS2bits.T6IF = 0;

    /* set Timer interrupt priority level */
    IPC11bits.T6IP = 5;

    /* enable Timer interrupt */
    IEC2bits.T6IE = 1;

    /* set Timer period register */
    PR6 = ValPR6;
}

void initTimerRTC(void)
{
	T4CON = 0;
	T4CONbits.TCKPS = 3; //Prescaler 256
 	IFS1bits.T4IF = 0;
	IPC6bits.T4IP = 5;
 	IEC1bits.T4IE = 1;
	PR4 = ValPR4;
    TMR4 = 0;
    T4CONbits.TON=1;
}