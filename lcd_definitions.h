// Definitions for LCD library from Peter Fleury

#ifndef LCD_DEFINITIONS_H_
#define LCD_DEFINITIONS_H_


/**
 * @name  Definition for LCD controller type
 * Use 0 for HD44780 controller, change to 1 for displays with KS0073 controller.
 */
#ifndef LCD_CONTROLLER_KS0073 
#define LCD_CONTROLLER_KS0073 0  /**< Use 0 for HD44780 controller, 1 for KS0073 controller */
#endif

/** 
 * @name  Definitions for Display Size 
 * Change these definitions to adapt setting to your display
 *
 * These definitions can be defined in a separate include file \b lcd_definitions.h instead modifying this file by 
 * adding -D_LCD_DEFINITIONS_FILE to the CDEFS section in the Makefile.
 * All definitions added to the file lcd_definitions.h will override the default definitions from lcd.h
 *
 */
#ifndef LCD_LINES
#define LCD_LINES           2     /**< number of visible lines of the display */
#endif
#ifndef LCD_DISP_LENGTH
#define LCD_DISP_LENGTH    16     /**< visibles characters per line of the display */
#endif
#ifndef LCD_LINE_LENGTH
#define LCD_LINE_LENGTH  0x40     /**< internal line length of the display    */
#endif
#ifndef LCD_START_LINE1
#define LCD_START_LINE1  0x00     /**< DDRAM address of first char of line 1 */
#endif
#ifndef LCD_START_LINE2
#define LCD_START_LINE2  0x40     /**< DDRAM address of first char of line 2 */
#endif
#ifndef LCD_START_LINE3
#define LCD_START_LINE3  0x14     /**< DDRAM address of first char of line 3 */
#endif
#ifndef LCD_START_LINE4
#define LCD_START_LINE4  0x54     /**< DDRAM address of first char of line 4 */
#endif
#ifndef LCD_WRAP_LINES
#define LCD_WRAP_LINES      0     /**< 0: no wrap, 1: wrap at end of visibile line */
#endif


/**
 * @name Definitions for 4-bit IO mode
 *
 * The four LCD data lines and the three control lines RS, RW, E can be on the 
 * same port or on different ports. 
 * Change LCD_RS_PORT, LCD_RW_PORT, LCD_E_PORT if you want the control lines on
 * different ports. 
 *
 * Normally the four data lines should be mapped to bit 0..3 on one port, but it
 * is possible to connect these data lines in different order or even on different
 * ports by adapting the LCD_DATAx_PORT and LCD_DATAx_PIN definitions.
 *
 * Adjust these definitions to your target.\n 
 * These definitions can be defined in a separate include file \b lcd_definitions.h instead modifying this file by 
 * adding \b -D_LCD_DEFINITIONS_FILE to the \b CDEFS section in the Makefile.
 * All definitions added to the file lcd_definitions.h will override the default definitions from lcd.h
 *  
 */
#ifndef LCD_PORT
#define LCD_PORT         PORTB        /**< port for the LCD lines   */
#endif
#ifndef LCD_DATA0_PORT
#define LCD_DATA0_PORT   LCD_PORT     /**< port for 4bit data bit 0 */
#endif
#ifndef LCD_DATA1_PORT
#define LCD_DATA1_PORT   LCD_PORT     /**< port for 4bit data bit 1 */
#endif
#ifndef LCD_DATA2_PORT
#define LCD_DATA2_PORT   LCD_PORT     /**< port for 4bit data bit 2 */
#endif
#ifndef LCD_DATA3_PORT
#define LCD_DATA3_PORT   PORTD     /**< port for 4bit data bit 3 */
#endif
#ifndef LCD_DATA0_PIN
#define LCD_DATA0_PIN    2            /**< pin for 4bit data bit 0  */
#endif
#ifndef LCD_DATA1_PIN
#define LCD_DATA1_PIN    1            /**< pin for 4bit data bit 1  */
#endif
#ifndef LCD_DATA2_PIN
#define LCD_DATA2_PIN    0            /**< pin for 4bit data bit 2  */
#endif
#ifndef LCD_DATA3_PIN
#define LCD_DATA3_PIN    6            /**< pin for 4bit data bit 3  */
#endif
#ifndef LCD_RS_PORT
#define LCD_RS_PORT      PORTB     /**< port for RS line         */
#endif
#ifndef LCD_RS_PIN
#define LCD_RS_PIN       4            /**< pin  for RS line         */
#endif
#ifndef LCD_RW_PORT
#define LCD_RW_PORT      PORTB     /**< port for RW line         */
#endif
#ifndef LCD_RW_PIN
#define LCD_RW_PIN       3            /**< pin  for RW line         */
#endif
#ifndef LCD_E_PORT
#define LCD_E_PORT       PORTD     /**< port for Enable line     */
#endif
#ifndef LCD_E_PIN
#define LCD_E_PIN        5            /**< pin  for Enable line     */
#endif


#endif // LCD_DEFINITIONS_H_
