//Generales
#define TRUE 1
#define FALSE 0
#define SOF 0xFE
#define DISTANCIA 30
#define DIRECCION_LOCAL 2
#define DIRECCION_DESTINO   3

//UART
#define VALOR_INICIO 0x00FE
#define FCY 40000000
#define BAUDRATE 9600
#define BRGVAL ((FCY / BAUDRATE) / 16) -1

//Timers
#define ValPR4	39062
#define ValPR6  625     //para interrumpir cada 1000 us o sea 1 milisegundo

//Bitacora
#define HORA base
#define MIN  base + 1
#define SEG  base + 2
#define VEL  base + 3
#define EJES base + 4

//protocolo
#define CANT_VEHIC 0x0041
#define RESET      0x0042
#define EJES_VEHIC 0x0043
#define HORA_VEHIC 0x0044
#define ENC_CAMARA 0x0045
#define NAK        0x0046  


//generales
extern unsigned char contadorAutos;

//Variables
extern unsigned char counterTog;
extern unsigned char espera;
extern unsigned char minutos,segundos, hora;
extern unsigned char protocoloEnvio[];
extern unsigned char protocoloRecepcion[];
extern unsigned char velocidad;
extern unsigned int cantidadEjes;
extern unsigned int indicadorBitacora;

//UART
extern unsigned int k;
extern unsigned int ReceivedChar;
extern unsigned int iniciar;
extern unsigned int paqueteOK;
extern unsigned int hayPaqueteNuevo;
extern unsigned int hayNuevosDatos;


//Transmision de datos
extern unsigned int t;


//TIMERS
extern unsigned int contadorTMR6;
extern unsigned int bloqueado;
extern unsigned int contadorCableA;
extern unsigned int contadorCableB;
extern unsigned int hayNuevosDato;


//Prototipos de Funciones
void config( void );
void Update_Time(unsigned char, unsigned char, unsigned char, unsigned char );
void InitUART2 ( void );
void initTimerGP(void );
void initTimerRTC( void );
void initPuertos(void);
void verificarComandos(int comando, int valor);
void verificarCheksumm(void);
void encenderCamara(void);
void grabarBitacora(void);

void enviarDatos(void);


