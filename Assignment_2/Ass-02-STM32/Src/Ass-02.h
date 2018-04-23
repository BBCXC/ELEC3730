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

// Assignment main
extern void Ass_02_Main(void);

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE 
//
typedef struct{
  int Area[25][5];
  static char *items[42] = {"=", "+", "ANS", ".", "0", ">", "-", "3", "2", "1",
                     		"<", "/", "6", "5", "4", "AC", "x", "9", "8", "7", "DEL",
                     		"=", "(", "ANS", "sqrt", "^", ">", ")", "atan", "acos", "asin",
                     		"<", "pi", "tan", "cos", "sin", "AC", "", "log", "ln", "exp", "DEL"};
  char **input;

  char *formula;
  char *result;
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
extern int draw_item(int cell_number);
extern int LCD_ClearCell(int x_min, int x_max, int y_min, int y_max);
extern void CalculatorProcess(void);
extern int get_touch_pos(voidint display_x, int display_y);


extern double parseFormula();
extern double parseSub();
extern double parseSum();
extern double parsePro();
extern double parseDiv();
extern double parseFactor();
extern double parseNumber();


// Library functions
#ifdef STM32F407xx
extern uint8_t BSP_TP_Init(void);
extern uint8_t BSP_TP_GetDisplayPoint(Coordinate *pDisplay);
#endif

#endif /* ASS_02_H_ */
