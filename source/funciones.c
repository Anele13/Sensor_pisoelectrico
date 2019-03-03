#include "p33FJ256GP710.h"
#include "config.h"

void transmitir(unsigned char arreglo[]){
    puntero = arreglo;
    IFS1bits.U2TXIF = 1;
}

void reset(void){
    int x;
    for(x=0; x < TOPE_BITACORA; x++)
        bitacora[x]=0;  
    contadorAutos=0;
}

int calcularVehiculosMas2Ejes(void){
    int x,cantidad = 0;
    for(x; x < TOPE_BITACORA; x+=4){
        if(bitacora[x]>2)
            cantidad++;
    }
    return cantidad;
}

int calcularVehiculosEnHora(int hora){
    int x,cantidad = 0;
    for(x; x < TOPE_BITACORA; x+=5){
        if(bitacora[x]== hora)
            cantidad++;
    }
    return cantidad;
}

void calcularChecksum (unsigned int nuevo_arreglo[]){
    int TopeNuevo = nuevo_arreglo[1];
    int t,sumaTotal, suma, Low_byte, High_byte = 0;
    while (t<(TopeNuevo - 2))
       {
           if ((t + 1) == (TopeNuevo - 2))
              {
               suma = nuevo_arreglo[t];
               suma = suma<<8;
               suma = suma + 0;
              }
           else
              {
               suma = nuevo_arreglo[t];
               suma = suma<<8;
               t++;
               suma = suma + nuevo_arreglo[t];
              }

       sumaTotal = suma + sumaTotal;
       suma = 0;
       t++;
     }

     Low_byte = sumaTotal;
     High_byte = sumaTotal>>8;
     Low_byte = Low_byte<<8;
     Low_byte = Low_byte>>8;

     nuevo_arreglo[TopeNuevo-2] = High_byte;
     nuevo_arreglo[TopeNuevo-1] = Low_byte;
}


void errorChecksum(void){
    transmitir(protocoloNak);
    paqueteOK = FALSE;
}


void verificarCheksumm (void){
    int sumaTotal,suma,h,check=0;
    paqueteOK = FALSE;
    TOPE = protocoloRecepcion[1];
    
    if (protocoloRecepcion[2]==2){
        while (h<(TOPE-2)){
            if ((h + 1) == (TOPE - 2)){
                suma = protocoloRecepcion[h];
                suma = suma<<8;
                suma = suma + 0;
            }
            else{
                suma = protocoloRecepcion[h];
                suma = suma<<8;
                h++;
                suma = suma + protocoloRecepcion[h];
            }
            sumaTotal = suma + sumaTotal;
            suma = 0;
            h++;
        }
        check = protocoloRecepcion[TOPE-2];
        check = check<<8;
        check = check + protocoloRecepcion[TOPE-1];
        
        if (check == sumaTotal)
            paqueteOK = TRUE;
        else
            errorChecksum();
    }
}

void enviarCantidadVehiculos(void){
    protocoloEnvio[5] = CANT_VEHIC;
    protocoloEnvio[6] = contadorAutos;
    calcularChecksum(protocoloEnvio);
    transmitir(protocoloEnvio);
}

void resetBitacora(void){
    protocoloEnvio[5] = RESET;
    reset();
    calcularChecksum(protocoloEnvio);
    transmitir(protocoloEnvio);
}

void enviarVehiculosEjes(void){
    protocoloEnvio[5] = EJES_VEHIC;
    protocoloEnvio[6] = calcularVehiculosMas2Ejes();
    calcularChecksum(protocoloEnvio);
    transmitir(protocoloEnvio);
}

void enviarVehiculosEnHora(int hora){
    protocoloEnvio[5] = HORA_VEHIC;
    calcularVehiculosEnHora(hora);
    calcularChecksum(protocoloEnvio);
    transmitir(protocoloEnvio);
}

void onCamara(void){
    protocoloEnvio[5] = ENC_CAMARA;
    encenderCamara();
    calcularChecksum(protocoloEnvio);
    transmitir(protocoloEnvio);
}

void errorComandos(void){
    protocoloEnvio[5] = NAK;
    calcularChecksum(protocoloEnvio);
    transmitir(protocoloEnvio);
}


void verificarComandos(int comando, int hora){
    switch (comando){
        case CANT_VEHIC:  enviarCantidadVehiculos();
                          break;

        case RESET:       resetBitacora();
                          break;

        case EJES_VEHIC:  enviarVehiculosEjes();
                          break;

        case HORA_VEHIC:  enviarVehiculosEnHora(hora);
                          break;

        case ENC_CAMARA:  onCamara();
                          break;

        default: errorComandos();
    }
}

