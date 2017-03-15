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
 * u-blox 7 configuration
 * for Timepulse
 *****************************/

// this configuration changes the timepulse output of the u-blox
// NEO-7M module to 10kHz. be aware that this might not be the
// configuration you desire!
// Please refer to the u-blox receiver description!
#define UBLOX_CONF

#ifdef UBLOX_CONF
const unsigned char configuration[40] PROGMEM = { 
	0xB5, 0x62, 0x06, 0x31, 0x20, 0x00, 
	0x00, 0x01, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
	0x00, 0x00, 0x00, 0x00, 0xEF, 0x00, 0x00, 0x00,
	0x31, 0xDB };
#endif

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
#define RTC_TXT		/* Macro to activate the "RTC" text*/
#ifdef RTC_TXT
#define RTC_X 13
#define RTC_Y 0
#endif // RTC_TXT

#endif // 16x2

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

void displayTime(char *utctime) {
	lcd_gotoxy(TIME_X, TIME_Y); // first line, first character
	lcd_putc(utctime[0]);
	lcd_putc(utctime[1]);
	lcd_putc(':');
	lcd_putc(utctime[2]);
	lcd_putc(utctime[3]);
	lcd_putc(':');
	lcd_putc(utctime[4]);
	lcd_putc(utctime[5]);
}

#endif // GPSCLOCK_H_
