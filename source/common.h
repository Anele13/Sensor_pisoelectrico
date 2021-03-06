
#define USAR_LCD

/* variables used in Timer 1 ISR */
extern volatile unsigned char hours;
extern volatile unsigned char minutes;
extern volatile unsigned char seconds;

/* variables used in raw ADC data to hex */
extern void advolt( unsigned int adc_conv_data );
extern volatile unsigned char adones;
extern volatile unsigned char adtens;
extern volatile unsigned char adhunds;
extern volatile unsigned int temp1;
extern volatile unsigned char adc_lcd_update;


#ifdef USAR_LCD
/*****	LCD COMMAND FUCNTION PROTOTYPES  *****/
#define cursor_right()  lcd_cmd( 0x14 )
#define cursor_left()   lcd_cmd( 0x10 )
#define display_shift() lcd_cmd( 0x1C )
#define home_clr()      lcd_cmd( 0x01 ) 
#define home_it()       lcd_cmd( 0x02 ) 
#define line_2()        lcd_cmd( 0xC0 ) // (0xC0)

#endif // USAR_LCD

