/*************************************************************************
Title:    Interprets NMEA messages on the UART
Author:   crasbe  <crasbe@gmail.com>  http://www.crasbe.de/
File:     gpsclock.c
Software: AVR-GCC 4.x
Hardware: HD44780 compatible LCD text display
		  GPS module sending NMEA messages over serial
		  7 free IO pins for LCD, 1 for UART
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "lcd.h"
#include "uart.h"

#define UART_BAUD_RATE      9600

#if LCD_DISP_LENGTH == 8
#define TXT_TEMPLATE "xx:xx:xx\n xx xxSa"
#define LOCK_X 1
#define LOCK_Y 1
#define SAT_X 4
#define SAT_Y 1
 
#else
#define TXT_TEMPLATE "xx:xx:xx UTC\nxx LOCK xx SAT"
#define LOCK_X 0
#define LOCK_Y 1
#define SAT_X 8
#define SAT_Y 1
#define TXT_RTC

#endif

// states:
// 0 - no command
// 1 - receiving command
// 2 - receiving GPGGA
// 3 - receiving GPGSA
#define NO_CMD		0
#define RECV_CMD 	1
#define RECV_GPGGA	2
#define RECV_GPGSA	3

void displayTime(char *utctime, bool rtc) {
	lcd_gotoxy(0, 0); // first line, first character
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


int main(void) {
	unsigned int c;
	char input;
	
	uint8_t state = NO_CMD;
	
	// comma position
	// GPGGA - time after first comma, satellite count after seventh comma
	//       - $GPGGA,hhmmss.00,,,,,,nn,,
	// GPGSA - lock after second comma
	//		 - $GPGSA,,l,,,,
	uint8_t comma = 0;
	
	// receive the command
	char command[6];
	uint8_t compos = 0; // position

	// receive the time (hhmmss.00)
	char utctime[9];
	uint8_t utctimepos = 0;
	
	// receive the satellite count
	char satcount[2];
	uint8_t satpos = 0;
	
	// receive the lock state
	uint8_t lock = 1;

	// initialize display, cursor off
	lcd_init(LCD_DISP_ON);

	// initialize UART
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	
	sei(); // enable interrupts for UART
	
	lcd_clrscr(); // clear the LCD

	//lcd_puts_P("xx:xx:xx UTC\nxx LOCK xx SAT"); // template for LCD
	lcd_puts_P(TXT_TEMPLATE);
	
	for (;;) {
		c = uart_getc(); // get a char from the buffer
		if (c & UART_NO_DATA) { // nothing received
			continue;
		} else if(	c & UART_FRAME_ERROR || c & UART_OVERRUN_ERROR || 
					c & UART_BUFFER_OVERFLOW) {
			uart_puts_P("UART Error!\n");
			continue;
		}

		input = (unsigned char) c; // convert input to unsigned char
		
		if(input == '\n') { // the party with this line is over
			state = NO_CMD;
			comma = 0;
			compos = 0;
			utctimepos = 0;
			satpos = 0;
			lock = 1;
			
			continue;
		} else if(input == '$') { // new line, new luck
			state = RECV_CMD;
			continue;
		} else if(input == ',') {
			comma += 1;
			
			// we finished receiving the command
			if(state == RECV_CMD) {
				// is our command "GPG*A"?
				if(	command[0] == 'G' && command[1] == 'P' && 
					command[2] == 'G' && command[4] == 'A') {
					if(command[3] == 'G') // GPGGA
						state = RECV_GPGGA;
					else if(command[3] == 'S') // GPGSA
						state = RECV_GPGSA;
					else // something else
						state = NO_CMD;
				} else // something else
					state = NO_CMD;
			} else if(state == RECV_GPGGA && lock == 1 && comma == 2) { // no lock yet
				displayTime(utctime, true);
				// lcd_puts_P("RTC");		// display the time from the RTC
			}
			continue;
		} 
		
		if(state == RECV_CMD) { // fill the command buffer
			command[compos] = (char) input;
			compos++;
		} else if(state == RECV_GPGGA) { // time and satellite count
			if(comma == 1) { 	// receiving time
				utctime[utctimepos] = input;
				utctimepos++;
			} else if(comma == 2) { // time received
				displayTime(utctime, false);
				//lcd_puts_P("   "); // override a "RTC"
			} else if(comma == 7) {		// receiving satellite count
				satcount[satpos] = input;
				satpos++;
			} else if(comma == 8) { // satellite count received
				//lcd_gotoxy(8, 1);
				lcd_gotoxy(SAT_X, SAT_Y);
				lcd_putc(satcount[0]);
				lcd_putc(satcount[1]);
			}
		} else if(state == RECV_GPGSA) {
			if(comma == 2) { // receive lock state
				lock = atoi(&input);
				
				//lcd_gotoxy(0, 1); // second line, first character
				lcd_gotoxy(LOCK_X, LOCK_Y);
				if(lock == 1)
					lcd_puts_P("NO");
				else if(lock == 2)
					lcd_puts_P("2D");
				else if(lock == 3)
					lcd_puts_P("3D");
			}
		}
	}
	return 0; // never reached
}
