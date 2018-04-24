//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

// Assignment 2 include file

#ifndef ASS_02_H_
#define ASS_02_H_

// Standard includes
#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#include "openx07v_c_lcd.h"
#include "touch_panel.h"
#else
#include <windows.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

#define debugsys 0
#define LCDResultlen 20

// Assignment main
extern void Ass_02_Main(void);

typedef struct{
  int Area[25][5];
  char **items;
  char **input;
  int num_char;

  char *formula;
  double result;
}
  calculator_struct;
  calculator_struct cal_p;

// Question 1
extern void CommandLineParserInit(void);
extern void CommandLineParserProcess(void);

// Question 2
extern void CalculatorInit(void);
extern int title_animation(void);
extern int calculator_layout(void);
extern int draw_numpad(void);
extern int draw_sym(void);
extern int draw_item(int cell_number, int offset);
extern int draw_result(void);
extern int draw_equation(void);
extern int clear_equation(void);
extern int LCD_Cell_Colour(int x_min, int x_max, int y_min, int y_max, int text_colour, int cell_colour);
extern void CalculatorProcess(void);
extern int get_touch_pos(int display_x, int display_y);
extern int Input_append(char *item);
extern int LCD_Cell_Highlight(int status, int cell_number);

extern double parseFormula();
extern double parseSub();
extern double parseSum();
extern double parsePro();
extern double parseDiv();
extern double parsePow();
extern double parseFactor();
extern double parseNumber();


// Library functions
#ifdef STM32F407xx
extern uint8_t BSP_TP_Init(void);
extern uint8_t BSP_TP_GetDisplayPoint(Coordinate *pDisplay);
#endif

#endif /* ASS_02_H_ */

/*
LCD_COLOR_BLUE          0x001F
LCD_COLOR_GREEN         0x07E0
LCD_COLOR_RED           0xF800
LCD_COLOR_CYAN          0x07FF
LCD_COLOR_MAGENTA       0xF81F
LCD_COLOR_YELLOW        0xFFE0
LCD_COLOR_LIGHTBLUE     0x841F
LCD_COLOR_LIGHTGREEN    0x87F0
LCD_COLOR_LIGHTRED      0xFC10
LCD_COLOR_LIGHTCYAN     0x87FF
LCD_COLOR_LIGHTMAGENTA  0xFC1F
LCD_COLOR_LIGHTYELLOW   0xFFF0
LCD_COLOR_DARKBLUE      0x0010
LCD_COLOR_DARKGREEN     0x0400
LCD_COLOR_DARKRED       0x8000
LCD_COLOR_DARKCYAN      0x0410
LCD_COLOR_DARKMAGENTA   0x8010
LCD_COLOR_DARKYELLOW    0x8400
LCD_COLOR_WHITE         0xFFFF
LCD_COLOR_LIGHTGRAY     0xD69A
LCD_COLOR_GRAY          0x8410
LCD_COLOR_DARKGRAY      0x4208
LCD_COLOR_BLACK         0x0000
LCD_COLOR_BROWN         0xA145
LCD_COLOR_ORANGE        0xFD20
*/
