// Header for gpsclock.c

#ifndef GPSCLOCK_H_
#define GPSCLOCK_H_

#define UART_BAUD_RATE      9600

// to change the LCD properties please edit lcd_definitions.h

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

#endif // GPSCLOCK_H_
