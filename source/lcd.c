/**********************************************************************
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Richard Fischer   07/14/05  Explorer 16 board LCD support
* Priyabrata Sinha  01/27/06  Ported to non-prototype devices
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
*
**********************************************************************/
#include "common.h"
#include "config.h"
#include "p33FJ256GP710.h"
#include "lcd.h"
#include "delay.h"


void hexdec( unsigned char count );
volatile unsigned char hunds;
volatile unsigned char tens;
volatile unsigned char ones;

// Control signal data pins 
#define  RW  LATDbits.LATD5       // LCD R/W signal
#define  RS  LATBbits.LATB15      // LCD RS signal
#define  E   LATDbits.LATD4       // LCD E signal 
//#define  E   LATFbits.LATF1       // LCD E signal

// Control signal pin direction 
#define  RW_TRIS	TRISDbits.TRISD5 
#define  RS_TRIS	TRISBbits.TRISB15
#define  E_TRIS		TRISDbits.TRISD4
//#define  E_TRIS		TRISFbits.TRISF1

// Data signals and pin direction
#define  DATA      LATE           // Port for LCD data
#define  DATAPORT  PORTE
#define  TRISDATA  TRISE          // I/O setup for data Port

const char segunda_linea[] = "HH:MM:SS VEL  ";
const char puntos[] = ":";
const char time_msg[] = "HORA: ";

/****************************************************************************/
/*****	LCD SUBROUTINE  *****/
void config_inicial(void){
        //Manejo del Xtal de la aplicación
	CLKDIV = 0;
	PLLFBD = 38;
	__builtin_write_OSCCONH(0x03);		/*	Initiate Clock Switch to Primary Oscillator with PLL (NOSC=0b011)*/
	__builtin_write_OSCCONL(0x01);
	while (OSCCONbits.COSC != 0b011);	/*	Wait for Clock switch to occur	*/
	while(!OSCCONbits.LOCK);
}

void Init_LCD( void )             // initialize LCD display
{
	// 15mS delay after Vdd reaches nnVdc before proceeding with LCD initialization
	// not always required and is based on system Vdd rise rate
	Delay(Delay_15mS_Cnt);                  // 15ms delay
			
	/* set initial states for the data and control pins */
	LATE &= 0xFF00;	
    RW = 0;                       // R/W state set low
	RS = 0;                       // RS state set low
	E = 0;                        // E state set low

	/* set data and control pins to outputs */
	TRISE &= 0xFF00;
 	RW_TRIS = 0;                  // RW pin set as output
	RS_TRIS = 0;                  // RS pin set as output
	E_TRIS = 0;                   // E pin set as output

	/* 1st LCD initialization sequence */
	DATA &= 0xFF00;
    DATA |= 0x0038;
    E = 1;	
    Nop();
    Nop();
    Nop();
    E = 0;                        // toggle E signal
   	Delay(Delay_5mS_Cnt);         // 5ms delay
      
	/* 2nd LCD initialization sequence */
	DATA &= 0xFF00;
    DATA |= 0x0038;
    E = 1;	
    Nop();
    Nop();
    Nop();	
    E = 0;                        // toggle E signal
    Delay_Us( Delay200uS_count ); // 200uS delay

	/* 3rd LCD initialization sequence */
	DATA &= 0xFF00;
    DATA |= 0x0038;
    E = 1;		
    Nop();
    Nop();
    Nop();	
    E = 0;                        // toggle E signal
    Delay_Us( Delay200uS_count ); // 200uS delay

    lcd_cmd( 0x38 );              // function set
    lcd_cmd( 0x0C );              // Display on/off control, cursor blink off (0x0C)
    lcd_cmd( 0x06 );			  // entry mode set (0x06)
}

/*---------------------------------------------------------------------
  Function Name: Update_Time
  Description:   Update LCD for real-time clock data
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/


void lcd_cmd( char cmd )          // subroutiune for lcd commands
{
//	TRISD &= 0xFF00;              // ensure RD0 - RD7 are outputs
	DATA &= 0xFF00;               // prepare RD0 - RD7
    DATA |= cmd;                  // command byte to lcd
	RW = 0;                       // ensure RW is 0
    RS = 0;
    E = 1;                        // toggle E line
    Nop();
    Nop();
    Nop();
    E = 0;
   	Delay(Delay_5mS_Cnt);         // 5ms delay
}


void lcd_data( char data )        // subroutine for lcd data
{
//	TRISD &= 0xFF00;              // ensure RD0 - RD7 are outputs
	RW = 0;       				 // ensure RW is 0
    RS = 1;                       // assert register select to 1
	DATA &= 0xFF00;               // prepare RD0 - RD7
    DATA |= data;                 // data byte to lcd
    E = 1;				
 	Nop();
    Nop();
    Nop();
    E = 0;                       // toggle E signal
    RS = 0;                      // negate register select to 0
    Delay_Us( Delay200uS_count ); // 200uS delay
    Delay_Us( Delay200uS_count ); // 200uS delay
}


void puts_lcd( unsigned char *data, unsigned char count ) 
{
  	while ( count )
	{
		lcd_data( *data++ );
		count --;
	}	
}

void hexdec( unsigned char count )
{
 hunds = 0;						//initialize hundred
 tens  = 0;						//initialize tens
 ones = 0;						//initialzise ones
 
	while ( count >= 10 )
	{

		if ( count >= 200 )		//check two hundreds
		{
		  count -= 200;		    //subtract 200
		  hunds = 0x02;			//set for 2 hundred
		}
	
		if (count >= 100)		//check hundreds
		{
		  count -= 100;		    //subract 100
		  hunds++;				//increment hundred register
		}
	
		if (count >= 10 )		//check tens
		{
		  count -= 10;		    //subtract 10
		  tens++;				//increment tens
		}
	}

	ones = count;				//remaining count equals ones
}

void Update_Time(unsigned char hora_aux, unsigned char minutos_aux, unsigned char seg_aux, unsigned char velocidad)
{
    //"TP3      AC 2017"
   	/* position LCD cursor at column, row */
  	home_it();
    puts_lcd( (unsigned char*) &time_msg[0], sizeof(time_msg) -1 );
	hexdec( hora );
	lcd_data(tens + 0x30);
	lcd_data(ones + 0x30);
    
    puts_lcd( (unsigned char*) &puntos[0], sizeof(puntos) -1 );
    hexdec( minutos );
    /* position LCD cursor at column, row */
    //cursor_right();
	lcd_data(tens + 0x30);
	lcd_data(ones + 0x30);
	
    puts_lcd( (unsigned char*) &puntos[0], sizeof(puntos) -1 );
  	hexdec( segundos );
	/* position LCD cursor at column, row */
    //cursor_right();
	lcd_data(tens + 0x30);
	lcd_data(ones + 0x30);
    
    /***** segunda linea *****/
    line_2();
    hexdec( hora_aux );
	lcd_data(tens + 0x30);
	lcd_data(ones + 0x30);
    puts_lcd( (unsigned char*) &puntos[0], sizeof(puntos) -1 );
    
    hexdec( minutos_aux );
	lcd_data(tens + 0x30);
	lcd_data(ones + 0x30);
    puts_lcd( (unsigned char*) &puntos[0], sizeof(puntos) -1 );
    
    hexdec( seg_aux );
	lcd_data(tens + 0x30);
	lcd_data(ones + 0x30);
    cursor_right();
    
    //Velocidad
    hexdec( velocidad );
    lcd_data(hunds + 0x30);
	lcd_data(tens + 0x30);
	lcd_data(ones + 0x30);
    cursor_right();
    
    //Cantidad de autos
    hexdec( contadorAutos );
	lcd_data(tens + 0x30);
	lcd_data(ones + 0x30);
}