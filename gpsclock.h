// Header for gpsclock.c

#ifndef GPSCLOCK_H_
#define GPSCLOCK_H_

/*****************************
 * Pin definitions
 *****************************/

// LED Output
#define LED_NOLOCK		PD4
#define LED_NOLOCK_DDR	DDRD
#define LED_NOLOCK_PORT PORTD
#define LED_2DLOCK		PD3
#define LED_2DLOCK_DDR	DDRD
#define LED_2DLOCK_PORT PORTD
#define LED_3DLOCK 		PD2
#define LED_3DLOCK_DDR  DDRD
#define LED_3DLOCK_PORT PORTD

/*****************************
 * Macros
 *****************************/

// UART baud rate. most GPS modules use 9600 baud
#define UART_BAUD_RATE      9600


/*****************************
 * Auxiliary Macros
 * DO NOT CHANGE!
 *****************************/

// to change the LCD properties please edit lcd_definitions.h

#define TIME_X 0
#define TIME_Y 0

// some 16x1 think they're 8x2 displays
#if (LCD_DISP_LENGTH == 8) && (LCD_LINES == 2)
#define TXT_TEMPLATE "xx:xx:xx\n xx xxSa"
#define LOCK_X 1
#define LOCK_Y 1
#define SAT_X 4
#define SAT_Y 1

// 16x2
#elif (LCD_DISP_LENGTH == 16) && (LCD_LINES == 2)
#define TXT_TEMPLATE "xx:xx:xx UTC\nxx LOCK xx SAT"
#define LOCK_X 0
#define LOCK_Y 1
#define SAT_X 8
#define SAT_Y 1
#define TXT_RTC		/* Macro to activate the "RTC" text*/

#endif

// Receiver states:
// 0 - no command
// 1 - receiving command
// 2 - receiving GPGGA
// 3 - receiving GPGSA
#define NO_CMD		0
#define RECV_CMD 	1
#define RECV_GPGGA	2
#define RECV_GPGSA	3

/*****************************
 * Function Prototypes
 ****************************/

void displayTime(char *utctime, bool rtc) {
	lcd_gotoxy(TIME_X, TIME_Y); // first line, first character
	lcd_putc(utctime[0]);
	lcd_putc(utctime[1]);
	lcd_putc(':');
	lcd_putc(utctime[2]);
	lcd_putc(utctime[3]);
	lcd_putc(':');
	lcd_putc(utctime[4]);
	lcd_putc(utctime[5]);
#ifdef TXT_RTC
	lcd_gotoxy(13, 0);
	if(rtc == true) {
		lcd_puts_P("RTC");
	} else {
		lcd_puts_P("   ");
	}
#endif
}

#endif // GPSCLOCK_H_
