//     $Date: 2018-05-22 06:24:02 +1000 (Tue, 22 May 2018) $
// $Revision: 1330 $
//   $Author: Peter $

// Assignment 3 include file

#ifndef ASS_03_H_
#define ASS_03_H_

// Standard includes
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"

#include "adc.h"
#include "fatfs.h"
#include "openx07v_c_lcd.h"
#include "stm32f4xx_hal.h"
#include "touch_panel.h"
#include "usart.h"

#include <malloc.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// OS handles
extern osThreadId defaultTaskHandle;
extern osThreadId myTask02Handle;
extern osThreadId myTask03Handle;
extern osThreadId myTask04Handle;
extern osTimerId myTimer01Handle;

extern osSemaphoreId myBinarySem01Handle;
extern osSemaphoreId myBinarySem02Handle;
extern osSemaphoreId myBinarySem03Handle;
extern osSemaphoreId myBinarySem04Handle;
extern osSemaphoreId myBinarySem05Handle;
extern osSemaphoreId myBinarySem06Handle;
extern osMessageQId myQueue01Handle;
extern osMutexId myMutex01Handle;  // Protect LCD
extern osMutexId myMutex02Handle;  // Protect console output

// Assignment tasks
extern void Ass_03_Task_01(void const* argument);
extern void Ass_03_Task_02(void const* argument);
extern void Ass_03_Task_03(void const* argument);
extern void Ass_03_Task_04(void const* argument);

// Library functions
extern uint8_t BSP_TP_Init(void);
extern uint8_t BSP_TP_GetDisplayPoint(Coordinate* pDisplay);

// Front panel input
extern uint8_t getfp(Coordinate* display);

// STEPIEN: Safe printf() to ensure that only one task can write to
//          the console at a time

#define safe_printf(fmt, ...)                                                                                          \
    osMutexWait(myMutex02Handle, osWaitForever);                                                                       \
    printf(fmt, ##__VA_ARGS__);                                                                                        \
    osMutexRelease(myMutex02Handle);

//
// ADD YOUR CODE
//

extern osMessageQId myQueue02Handle;  // State message

#include "Ass-03-Button.h"
#include "Ass-03-Debug.h"
#include "Ass-03-Popup.h"
#include "Ass-03-Window.h"
#include "Ass-03-Equation.h"
#include "Ass-03-Input.h"
#include "Ass-03-File-w.h"

extern osMutexId windowbuf_Handle;  // Protect Window buffer
extern osMutexId button_Handle;     // Protect button struct
extern osMutexId popup_Handle;      // Protect popup struct
extern osMutexId debug_Handle;      // Protect debug struct
extern osMutexId file_Handle;      // Protect file struct

#define KNRM "\e[0m"
#define KRED "\e[31m"  //"\x1B[31m"
#define KGRN "\e[32m"
#define KYEL "\e[33m"
#define KBLU "\e[34m"
#define KMAG "\e[35m"
#define KCYN "\e[36m"
#define KWHT "\e[37m"

#define CLEAR_M "\014"
#define RESET_M "\033[3J"

#define MemExpand 10

#define DEFAULT_COLOUR_M KNRM
#define DEBUG_M KYEL
#define SYS_M KMAG
#define ERROR_M KRED

#define HELP_M "%-25.25s\t%-35.35s"
#define DEBUG_P                                                                                                        \
    { safe_printf("File %s, function %s, line %i\n", __FILE__, __FUNCTION__, __LINE__); }

#define XOFF 68
#define YOFF 2
#define XSIZE 250
#define YSIZE 142

void Task_4_Init(void);

extern int Save_Popup();
extern int Load_File();
extern int Save_New();
extern int Save_Overwrite();
extern int Clear_Popup();

extern void CommandLineParserProcess(void);

extern int StringProcess(char* command_line, int i);
extern int string_parser(char* inp, char** array_of_words_p[], char delim);
extern int help_parser(char** array_of_words_p[], int word_count);
extern int command_parser(char** array_of_words_p[], int word_count, char** path_p[], int path_count);

// Recursive decent parser
extern double parseFormula(void);
extern double parseSub(void);
extern double parseSum(void);
extern double parsePro(void);
extern double parseDiv(void);
extern double parseFactor(void);
extern double parseNumber(void);
extern double parsePow(void);

extern int analog_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count);
extern int ls_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count);
extern int cd_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count);
extern int mkdir_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count);
extern int cp_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count);
extern int rm_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count);
extern int expr_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count);
extern int debug_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count);
extern int system_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count);
extern int clear_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count);
extern int reset_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count);
extern int help_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count);


// Question 1
typedef struct {
    char* NameString;                                                                               // Operation string
    int (*Function_p)(char** array_of_words_p[], int word_count, char** path_p[], int path_count);  // Result Pointer
    char* HelpString;                                                                               // Help information
    char* DescriptionString;                                                                        // Description
} command_s;


#endif /* ASS_03_H_ */
