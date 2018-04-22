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
}
  grid_struct;

// Question 1
extern void CommandLineParserInit(void);
extern void CommandLineParserProcess(void);

// Question 2
extern void CalculatorInit(grid_struct *grid_space_p, char **input[]);
extern int calculator_layout(grid_struct *grid_space_p, int scaled_width, int scaled_height);
extern int draw_numpad(grid_struct *grid_space_p);
extern int draw_sym(grid_struct *grid_space_p);
extern int draw_item(grid_struct *grid_space_p, char* item, int cell_number);
extern int LCD_ClearCell(int x_min, int x_max, int y_min, int y_max);
extern void CalculatorProcess(grid_struct *grid_space_p, char **input[]);
extern int get_touch_pos(grid_struct *grid_space_p, int display_x, int display_y);

// Library functions
#ifdef STM32F407xx
extern uint8_t BSP_TP_Init(void);
extern uint8_t BSP_TP_GetDisplayPoint(Coordinate *pDisplay);
#endif

#endif /* ASS_02_H_ */
