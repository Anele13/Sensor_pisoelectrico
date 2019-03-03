#define TOPE_BITACORA 30

typedef struct{
	unsigned char hora;
	unsigned char minutos;
    unsigned char segundos;
    unsigned char velocidad;
    unsigned char cant_ejes;
}tipo_registro;
tipo_registro bitacora[TOPE_BITACORA];

