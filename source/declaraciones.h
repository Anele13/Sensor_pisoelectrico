unsigned char counterTog;
unsigned char espera;


unsigned int TOPE,i;
unsigned char contadorAutos=0;

unsigned char segundos=0;
unsigned char minutos=0;
unsigned char hora=0;
unsigned int indicadorBitacora=0;


unsigned char velocidad=0;
unsigned int cantidadEjes=0;
unsigned int contadorCableA=0;
unsigned int contadorCableB=0;
unsigned int hayNuevosDatos=FALSE;
unsigned int hayPaqueteNuevo = FALSE;
unsigned int contadorTMR6=0;


//para UART
unsigned int k=0;
unsigned int ReceivedChar;
unsigned int iniciar=TRUE;
unsigned int bloqueado= FALSE;
unsigned int paqueteOK;



unsigned char protocoloRecepcion[9];
unsigned char protocoloEnvio[20];
unsigned char protocoloNak[8];

//Transmision de datos
unsigned int *puntero;
unsigned int t=1;