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

typedef struct{
	char *NameString; 								//Operation string
	int (*Function_p)(char **array_of_words_p[],	//Array pointer
					  int word_count, 			//Function pointer
	   				  double *result); 			//Result Pointer
	char *HelpString; 								//Help information
	char *DescriptionString;						//Description
} command_s;

typedef struct{
	char *formula;
	double result;
	double prev_ans;
}
	result_mem;
	result_mem output;

// Question 1
extern void CommandLineParserInit(void);
extern void CommandLineParserProcess(void);
extern int string_parser(char *inp, char **array_of_words_p[]);
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
extern int help_function(char **array_of_words_p[], int word_count, double *result);

extern double parseFormula(void);
extern double parseSub(void);
extern double parseSum(void);
extern double parsePro(void);
extern double parseDiv(void);
extern double parseFactor(void);
extern double parseNumber(void);
extern double parsePow(void);

// Question 2
extern void CalculatorInit(void);
extern void CalculatorProcess(void);

// Library functions
#ifdef STM32F407xx
extern uint8_t BSP_TP_Init(void);
extern uint8_t BSP_TP_GetDisplayPoint(Coordinate *pDisplay);
#endif

#endif /* ASS_02_H_ */
