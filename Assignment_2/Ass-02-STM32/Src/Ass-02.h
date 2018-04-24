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

// Assignment main
extern void Ass_02_Main(void);

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE
//
typedef struct{
  int Area[25][5];
  char **items;
  char **input;
  int num_char;

  char *formula;
  char *equation;
  double result;
  double prev_ans;
}
  grid_struct;
  grid_struct grid_space_p;

// Question 1
extern void CommandLineParserInit(void);
extern void CommandLineParserProcess(void);

// Question 2
extern void CalculatorInit(void);
extern int calculator_layout(void);
extern int draw_numpad(void);
extern int draw_sym(void);
extern int draw_item(int cell_number, int offset);
extern int LCD_Cell_Colour(int x_min, int x_max, int y_min, int y_max, int text_colour, int cell_colour);
extern void CalculatorProcess(void);
extern int get_touch_pos(int display_x, int display_y);
extern int Input_append(char *item);
extern int LCD_Cell_Highlight(int status, int cell_number);

extern double parseFormula(void);
extern double parseSub(void);
extern double parseSum(void);
extern double parsePro(void);
extern double parseDiv(void);
extern double parseFactor(void);
extern double parseNumber(void);
extern double parsePow(void);


// Library functions
#ifdef STM32F407xx
extern uint8_t BSP_TP_Init(void);
extern uint8_t BSP_TP_GetDisplayPoint(Coordinate *pDisplay);
#endif

#endif /* ASS_02_H_ */
