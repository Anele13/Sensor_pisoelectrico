#include "common.h"
#include <xc.h>
#include "confbits.h"
#include "p33FJ256GP710.h"
#include "lcd.h"
#include "config.h"
#include "declaraciones.h"


void enviarLCD(void){
    enviarDatos();
}

int main(void){
    home_clr();
    config();
    while(TRUE){  
        if(hayNuevosDatos){
            hayNuevosDatos = FALSE;
            enviarLCD();         
        }
        if(hayPaqueteNuevo){
            verificarComandos(protocoloRecepcion[5], protocoloRecepcion[6]);
            hayPaqueteNuevo=FALSE;
        }
    }
}