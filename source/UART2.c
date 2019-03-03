#include "p33FJ256GP710.h"
#include "config.h"


void chequearInicioPaquete(void){
    k=0;
    if(ReceivedChar == VALOR_INICIO){
        protocoloRecepcion[k] = ReceivedChar;
        k++;
        iniciar = FALSE;
    }
}

void armarPaquete(void){
    protocoloRecepcion[k] = ReceivedChar;
    k++;
    if (k == protocoloRecepcion[1]){
        verificarCheksumm();
         //paqueteOK=TRUE;
        if (paqueteOK){
            encenderCamara();
            hayPaqueteNuevo = TRUE;
        }
        iniciar = TRUE;
    }
}

void __attribute__((interrupt, auto_psv)) _U2RXInterrupt( void )
{
    encenderCamara();
    IFS1bits.U2RXIF = 0;
    ReceivedChar = U2RXREG;
    
    if(iniciar)
        chequearInicioPaquete();
    else
        armarPaquete();
}


void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void){
    IFS1bits.U2TXIF = 0;
	if (t<protocoloEnvio[1]){
        U2TXREG = protocoloEnvio[t];
        t++;
    }
    else{
        t=1;
    }
}

void InitUART2(void)
{
	U2MODEbits.STSEL = 0; // 1-stop bit
	U2MODEbits.PDSEL = 0; // No Parity, 8-data bits
	U2MODEbits.ABAUD = 0; // Auto-Baud Disabled
	U2MODEbits.BRGH = 0; // Low Speed mode
	U2BRG = BRGVAL; //
    
    U2STAbits.URXISEL = 0;
	
    IFS1bits.U2RXIF = 0;	// Clear the Recieve Interrupt Flag
	IEC1bits.U2RXIE = 1;	// Enable Recieve Interrupts
	
    U2STAbits.UTXEN = 1;	// Empieza a transmitir. Se dispara el Flag TXIF
    
    IFS1bits.U2TXIF = 0;	// Clear the Transmit Interrupt Flag
	IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts
    
    U2MODEbits.UARTEN = 1; // Enable UART
}
