/*************************************************************************
Title:    Interprets NMEA messages on the UART
Author:   crasbe  <crasbe@gmail.com>  http://www.crasbe.de/
File:     gpsclock.c
Software: AVR-GCC 4.x
Hardware: HD44780 compatible LCD text display
		  GPS module sending NMEA messages over serial
		  7 free IO pins for LCD, 1 for UART
**************************************************************************/
#include <stdbool.h>

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include <util/delay.h>

#include "lcd.h"
#include "uart.h"
#include "gpsclock.h"


int main(void) {
	unsigned int c; 		// received char from the UART
	unsigned char input;	// the same, converted to char
	
	uint8_t state = NO_CMD; // state of the receiving process
	
	// comma position
	// GPGGA - time after first comma, satellite count after seventh comma
	//       - $GPGGA,hhmmss.00,,,,,,nn,,
	// GPGSA - lock after second comma
	//		 - $GPGSA,,l,,,,
	uint8_t comma = 0;
	
	// buffer for receiving command, time, satellite count or lock state
	char buffer[9];
	uint8_t bufpos = 0;
	
	// receive the lock state
	uint8_t lock = 1;
	
	// initialize the LED port
	LED_NOLOCK_DDR |= (1 << LED_NOLOCK);	// outputs
	LED_2DLOCK_DDR |= (1 << LED_2DLOCK);
	LED_3DLOCK_DDR |= (1 << LED_3DLOCK);
	LED_NOLOCK_PORT |= (1 << LED_NOLOCK);	// no lock at startup
	LED_2DLOCK_PORT &= ~(1 << LED_2DLOCK);
	LED_3DLOCK_PORT &= ~(1 << LED_3DLOCK);
	
	
	// initialize UART and enable interrupts
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	sei();
	
	lcd_init(LCD_DISP_ON); 		// initialize display, cursor off
	lcd_clrscr(); 				// clear the LCD
	
	lcd_puts_P("GPS-Clock by\ncrasbe, R1 2017");
	
	_delay_ms(3000);
		
	lcd_clrscr();
	lcd_puts_P(TXT_TEMPLATE);	// write a template to the screen
	
	for (;;) {
		set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();
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
			lock = 1;
			bufpos = 0;
			
			continue;
		} else if(input == '$') { // new line, new luck
			state = RECV_CMD;
			continue;
		} else if(input == ',') {
			comma += 1;
			bufpos = 0;
			
			// we finished receiving the command
			if(state == RECV_CMD) {
				// is our command "GPG*A"?
				if(	buffer[0] == 'G' && buffer[1] == 'P' && 
					buffer[2] == 'G' && buffer[4] == 'A') {
					if(buffer[3] == 'G') // GPGGA
						state = RECV_GPGGA;
					else if(buffer[3] == 'S') // GPGSA
						state = RECV_GPGSA;
					else // something else
						state = NO_CMD;
				} else // something else
					state = NO_CMD;
			} else if(	state == RECV_GPGGA && lock == 1 && 
						comma == 2 && buffer[0] != 'G') { // no lock yet
				// on a cold start the module does not even output the
				// RTC time, so there is no time in buffer
				displayTime(buffer, true);
			}
			continue;
		} 
		
		if(state == RECV_CMD || (state == RECV_GPGGA && (comma == 1 || comma == 7))) { // fill the command buffer
			buffer[bufpos] = input;
			bufpos++;
		} else if(state == RECV_GPGGA) { // time and satellite count
			if(comma == 2) {
				displayTime(buffer, false);
			} else if(comma == 8) { // satellite count received
				lcd_gotoxy(SAT_X, SAT_Y);
				lcd_putc(buffer[0]);
				lcd_putc(buffer[1]);
			}
		} else if(state == RECV_GPGSA) {
			if(comma == 2) { // receive lock state
				lock = (uint8_t) (input-0x30); // better than atoi
				
				lcd_gotoxy(LOCK_X, LOCK_Y);
				if(lock == 1) {
					lcd_puts_P("NO");
					LED_NOLOCK_PORT |= (1 << LED_NOLOCK);
					LED_2DLOCK_PORT &= ~(1 << LED_2DLOCK);
					LED_3DLOCK_PORT &= ~(1 << LED_3DLOCK);
				} else if(lock == 2) {
					lcd_puts_P("2D");
					LED_NOLOCK_PORT &= ~(1 << LED_NOLOCK);
					LED_2DLOCK_PORT |= (1 << LED_2DLOCK);
					LED_3DLOCK_PORT &= ~(1 << LED_3DLOCK);
				} else if(lock == 3) {
					lcd_puts_P("3D");
					LED_NOLOCK_PORT &= ~(1 << LED_NOLOCK);
					LED_2DLOCK_PORT &= ~(1 << LED_2DLOCK);
					LED_3DLOCK_PORT |= (1 << LED_3DLOCK);
				}
			}
		}
	}
	return 0; // never reached
}
