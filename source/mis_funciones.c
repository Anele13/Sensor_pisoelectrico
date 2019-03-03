#include "p33FJ256GP710.h"
#include "config.h"
#include "estructura.h"

void encenderCamara(void){
    PORTAbits.RA0 = 1;
}

void apagarCamara(void){
    PORTAbits.RA0 = 0;
}

void grabarBitacora(void){
    bitacora[indicadorBitacora].hora = hora;
    bitacora[indicadorBitacora].minutos = minutos;
    bitacora[indicadorBitacora].segundos = segundos;
    bitacora[indicadorBitacora].velocidad = velocidad;
    bitacora[indicadorBitacora].cant_ejes = contadorCableA; //o el contadorCableB. Si paso el test puede ser cualquiera de los 2
    indicadorBitacora++;
}


void transmitir(){
    U2TXREG = protocoloEnvio[0];
}

void reset(void){
    int x;
    for(x=0; x < TOPE_BITACORA; x++){
        bitacora[x].hora =0;
        bitacora[x].minutos =0;
        bitacora[x].segundos =0;
        bitacora[x].velocidad =0;
        bitacora[x].cant_ejes =0;
    }        
    contadorAutos=0;
    indicadorBitacora=0;
}

int calcularVehiculosMas2Ejes(void){
    int x,cantidad = 0;
    for(x=0; x < TOPE_BITACORA; x++){
        if(bitacora[x].cant_ejes >2)
            cantidad++;
    }
    return cantidad;
}



void calcularVehiculosEnHora(int hora){
    int x = 0;
    int puntero=6;
    for(x=0; x < TOPE_BITACORA; x++){
        if(bitacora[x].hora == hora){
            protocoloEnvio[puntero] = bitacora[x].hora;
            protocoloEnvio[puntero++] = bitacora[x].minutos;
            protocoloEnvio[puntero++] = bitacora[x].segundos;
            protocoloEnvio[puntero++] = bitacora[x].velocidad;
            protocoloEnvio[puntero++] = bitacora[x].cant_ejes;
        }            
    }
    if(puntero != 6)
        protocoloEnvio[1]= 8 + (puntero - 6);
}

void calcularChecksum(){
    int TopeNuevo = protocoloEnvio[1];
    int t,sumaTotal, suma, Low_byte, High_byte = 0;
    while (t<(TopeNuevo - 2))
       {
           if ((t + 1) == (TopeNuevo - 2))
              {
               suma = protocoloEnvio[t];
               suma = suma<<8;
               suma = suma + 0;
              }
           else
              {
               suma = protocoloEnvio[t];
               suma = suma<<8;
               t++;
               suma = suma + protocoloEnvio[t];
              }

       sumaTotal = suma + sumaTotal;
       suma = 0;
       t++;
     }

     Low_byte = sumaTotal;
     High_byte = sumaTotal>>8;
     Low_byte = Low_byte<<8;
     Low_byte = Low_byte>>8;

     protocoloEnvio[TopeNuevo-2] = High_byte;
     protocoloEnvio[TopeNuevo-1] = Low_byte;
}


void enviarCantidadVehiculos(void){
    protocoloEnvio[1] = 9;
    protocoloEnvio[5] = CANT_VEHIC;
    protocoloEnvio[6] = contadorAutos;
    calcularChecksum();
    transmitir();
}

void resetBitacora(void){
    protocoloEnvio[1] = 8;
    protocoloEnvio[5] = RESET;
    reset();
    calcularChecksum();
    transmitir();
}

void enviarVehiculosEjes(void){
    protocoloEnvio[1] = 9;
    protocoloEnvio[5] = EJES_VEHIC;
    protocoloEnvio[6] = calcularVehiculosMas2Ejes();
    calcularChecksum();
    transmitir();
}

void enviarVehiculosEnHora(int hora){
    protocoloEnvio[1] = 8;
    protocoloEnvio[5] = HORA_VEHIC;
    calcularVehiculosEnHora(hora);
    calcularChecksum();
    transmitir();
}

void onCamara(void){
    protocoloEnvio[1] = 8;
    protocoloEnvio[5] = ENC_CAMARA;
    encenderCamara();
    calcularChecksum();
    transmitir();
}

void errorComandos(void){
    protocoloEnvio[1] = 8;
    protocoloEnvio[5] = NAK;
    calcularChecksum();
    transmitir();
}

void errorChecksum(void){
    errorComandos();
}

void verificarCheksumm (void){
    paqueteOK = FALSE;
    int sumaTotal,suma,h,check=0;
    int TOPE = protocoloRecepcion[1];
    
    if (protocoloRecepcion[2] == DIRECCION_LOCAL){
        while (h<(TOPE - 2)){
            if ((h + 1) == (TOPE - 2))              {
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

void enviarDatos(void){
    int aux = indicadorBitacora;
    if(aux)
        aux--;
    else
        aux=0;
    Update_Time(bitacora[aux].hora, bitacora[aux].minutos, bitacora[aux].segundos, bitacora[aux].velocidad);
}