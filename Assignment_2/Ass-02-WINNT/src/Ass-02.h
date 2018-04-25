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

//const char *ADDstr = "add";
//const char *SUBstr = "sub";
//const char *PROstr = "pro";
//const char *DIVstr = "div";
//const char *SINstr = "sin";
//const char *COSstr = "cos";
//const char *TANstr = "tan";
//const char *ASINstr = "asin";
//const char *ACOSstr = "acos";
//const char *ATANstr = "atan";
//const char *POWstr = "pow";
//const char *SQRTstr = "sqrt";
//const char *LNstr = "ln";
//const char *LOGstr = "log";
//const char *EXPstr = "exp";
//const char *RADstr = "rad";
//const char *DEGstr = "deg";
//
//const char *HELPstr = "help";

// Question 1
extern void CommandLineParserInit(void);
extern void CommandLineParserProcess(void);
extern int string_parser(char *inp, char **array_of_words_p[]);
extern double add_function(char **array_of_words_p[], int word_count);
extern double sub_function(char **array_of_words_p[]);
extern double pro_function(char **array_of_words_p[], int word_count);
extern double div_function(char **array_of_words_p[]);
extern double sin_function(char **array_of_words_p[]);
extern double cos_function(char **array_of_words_p[]);
extern double tan_function(char **array_of_words_p[]);
extern double asin_function(char **array_of_words_p[]);
extern double acos_function(char **array_of_words_p[]);
extern double atan_function(char **array_of_words_p[]);
extern double pow_function(char **array_of_words_p[]);
extern double sqrt_function(char **array_of_words_p[]);
extern double ln_function(char **array_of_words_p[]);
extern double log_function(char **array_of_words_p[]);
extern double exp_function(char **array_of_words_p[]);
extern int help_function(char **array_of_words_p[], int word_count);
extern int print_help(char **option);

// Question 2
extern void CalculatorInit(void);
extern void CalculatorProcess(void);

// Library functions
#ifdef STM32F407xx
extern uint8_t BSP_TP_Init(void);
extern uint8_t BSP_TP_GetDisplayPoint(Coordinate *pDisplay);
#endif

#endif /* ASS_02_H_ */
