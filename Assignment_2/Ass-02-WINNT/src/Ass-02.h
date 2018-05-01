/*
 * Author 	   : Mitchell Marotta C3258958
 * 				 Taylor Young C3206230
 * Date	  	   : 4 May 2018
 * Description : Assignment 2 header file
*/

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

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define CLEAR_M "\014"
#define RESET_M "\014"
#define DEBUG_M KYEL"DEBUG_INFO"
#define SYS_M KMAG
#define ERROR_M KRED "ERROR"

#define HELP_M "%-25.25s\t%-35.35s"

// Assignment main
extern void Ass_02_Main(void);

typedef struct{
	int debug;
	int system;
	int formula_mode;
	int first_time;
} 
	sys_t;
	sys_t info;

//Question 1
typedef struct{
	char *NameString; 								//Operation string
	int (*Function_p)(char **array_of_words_p[],	//Array pointer
					  int word_count, 				//Function pointer
	   				  double *result); 				//Result Pointer
	char *HelpString; 								//Help information
	char *DescriptionString;						//Description
} 
	command_s;

// Question 1
extern void CommandLineParserInit(void);
extern void CommandLineParserProcess(void);

extern int StringProcess(char *command_line, int i);
extern int string_parser(char *inp, char **array_of_words_p[]);
extern int help_parser(char **array_of_words_p[], int word_count, int debugsys);
extern int command_parser(char **array_of_words_p[], int word_count, int debugsys, double *prev_ans);

extern int add_function(char **array_of_words_p[], int word_count, double *result);
extern int sub_function(char **array_of_words_p[], int word_count, double *result);
extern int mul_function(char **array_of_words_p[], int word_count, double *result);
extern int div_function(char **array_of_words_p[], int word_count, double *result);
extern int sin_function(char **array_of_words_p[], int word_count, double *result);
extern int cos_function(char **array_of_words_p[], int word_count, double *result);
extern int tan_function(char **array_of_words_p[], int word_count, double *result);
extern int asin_function(char **array_of_words_p[], int word_count, double *result);
extern int acos_function(char **array_of_words_p[], int word_count, double *result);
extern int atan_function(char **array_of_words_p[], int word_count, double *result);
extern int pow_function(char **array_of_words_p[], int word_count, double *result);
extern int sqrt_function(char **array_of_words_p[], int word_count, double *result);
extern int ln_function(char **array_of_words_p[], int word_count, double *result);
extern int log_function(char **array_of_words_p[], int word_count, double *result);
extern int exp_function(char **array_of_words_p[], int word_count, double *result);
extern int formula_function(char **array_of_words_p[], int word_count, double *result);
extern int debug_function(char **array_of_words_p[], int word_count, double *result);
extern int system_function(char **array_of_words_p[], int word_count, double *result);
extern int clear_function(char **array_of_words_p[], int word_count, double *result);
extern int reset_function(char **array_of_words_p[], int word_count, double *result);
extern int help_function(char **array_of_words_p[], int word_count, double *result);


//Question 2
typedef struct{
	int Area[25][5];
	char **items;
}	
	grid_struct;
	grid_struct grid_space_p;

typedef struct{
	int size;
	char **input;
	int pos;
}
	str_mem;
	str_mem equation;

typedef struct{
	char *formula;
	double result;
	double prev_ans;
}
	result_mem;
	result_mem output;

// Question 2
extern void CalculatorInit(void);
extern int title_animation(void);
extern int calculator_layout(void);
extern int draw_numpad(void);
extern int draw_sym(void);
extern int draw_item(int cell_number, int offset, int text_colour, int cell_colour);
extern int LCD_Cell_Colour(int x_min, int x_max, int y_min, int y_max, int text_colour, int cell_colour);

extern void CalculatorProcess(void);
extern int get_touch_pos(int display_x, int display_y);
extern int Input_append(char *item);
extern int LCD_Cell_Highlight(int status, int cell_number, int offset);
extern int allocate_memory(void);
extern int reallocate_memory(void);
extern int clear_equation(void);
extern int draw_equation(void);
extern int draw_result(int status);

//Recursive decent parser
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
