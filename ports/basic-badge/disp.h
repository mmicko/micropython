#ifndef	__DISP_H
#define	__DISP_H


void TFT_24_7789_Init(void);
void TFT_24_7789_Write_Command(uint16_t command);
void tft_fill_area (uint16_t x, uint16_t y, uint16_t xlen, uint16_t ylen, uint32_t back);
void tft_disp_buffer_refresh(uint8_t * buff, uint8_t * color_buff);
void tft_disp_buffer_refresh_part(uint8_t * buff, uint8_t * color_buff);
void disp_tasks (void);
void set_cursor_state (uint8_t state);


#define	EGA_BLACK	0
#define	EGA_BLUE	1
#define	EGA_GREEN	2
#define	EGA_CYAN	3
#define	EGA_RED		4
#define	EGA_MAGENTA	5
#define	EGA_BROWN	6
#define	EGA_LGRAY	7
#define	EGA_DGRAY	8
#define	EGA_BBLUE	9
#define	EGA_BBGREEN	10
#define	EGA_BCYAN	11
#define	EGA_BRED	12
#define	EGA_BMAGENTA	13
#define	EGA_YELLOW	14
#define	EGA_WHITE	15

#endif
