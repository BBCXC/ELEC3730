//     $Date: 2018-05-10 10:07:15 +1000 (Thu, 10 May 2018) $
// $Revision: 1301 $
//   $Author: Peter $

// Assignment 3 include file

#ifndef ASS_03_H_
#define ASS_03_H_

// Standard includes
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"

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

int SIM_FLAG = 0;

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

#define DEFAULT_COLOUR_M KNRM
#define DEBUG_M KYEL
#define SYS_M KMAG
#define ERROR_M KRED

#define HELP_M "%-25.25s\t%-35.35s"
#define DEBUG_P                                                                                                        \
    { printf("File %s, function %s, line %i\n", __FILE__, __FUNCTION__, __LINE__); }

#define LCD_COLOR_BLUE 0x001F
#define LCD_COLOR_GREEN 0x07E0
#define LCD_COLOR_RED 0xF800
#define LCD_COLOR_CYAN 0x07FF
#define LCD_COLOR_MAGENTA 0xF81F
#define LCD_COLOR_YELLOW 0xFFE0
#define LCD_COLOR_LIGHTBLUE 0x841F
#define LCD_COLOR_LIGHTGREEN 0x87F0
#define LCD_COLOR_LIGHTRED 0xFC10
#define LCD_COLOR_LIGHTCYAN 0x87FF
#define LCD_COLOR_LIGHTMAGENTA 0xFC1F
#define LCD_COLOR_LIGHTYELLOW 0xFFF0
#define LCD_COLOR_DARKBLUE 0x0010
#define LCD_COLOR_DARKGREEN 0x0400
#define LCD_COLOR_DARKRED 0x8000
#define LCD_COLOR_DARKCYAN 0x0410
#define LCD_COLOR_DARKMAGENTA 0x8010
#define LCD_COLOR_DARKYELLOW 0x8400
#define LCD_COLOR_WHITE 0xFFFF
#define LCD_COLOR_LIGHTGRAY 0xD69A
#define LCD_COLOR_GRAY 0x8410
#define LCD_COLOR_DARKGRAY 0x4208
#define LCD_COLOR_BLACK 0x0000
#define LCD_COLOR_BROWN 0xA145
#define LCD_COLOR_ORANGE 0xFD20

extern double parseFormula(void);
extern double parseSub(void);
extern double parseSum(void);
extern double parsePro(void);
extern double parseDiv(void);
extern double parseFactor(void);
extern double parseNumber(void);
extern double parsePow(void);

#define num_buttons 12
typedef struct {
    int position[num_buttons][4];  // = {(x_min, x_max, y_min, y_max),   // Button 1
                                   // (x_min, x_max, y_min, y_max)};  // Button 2
    int item[num_buttons];

} button_t;
button_t button;

typedef struct {
    int width;        //       = 250;
    int height;       //     = 142;
    int position[4];  // = {x_min, x_max, y_min, y_max};

    int bg_colour;
    int line_colour;
    int grid_colour;

    int zoom_coeff;  // = 1;  // Number between 1 and MAX_ZOOM
    int buflen;      // Number of input values that are mapped to the
                     // window buffer

    int auto_scale;  // =  // Holds the maximum value that the window buffer has on the
                     // screen

    int next;  // = 0;  // Holds the position in the window buffer array
               // that is the latest filled

} window_t;
window_t window;
#define Max_Samples 10000
#define MAX_ZOOM 1
#define MIN_ZOOM 1

int Window_buffer[250][2];  // window.width][2];

typedef struct {
    int width;        //       = 250;
    int height;       //     = 142;
    int position[4];  // = {x_min, x_max, y_min, y_max};

    int bg_colour;
    int line_colour;
    int grid_colour;

    int zoom_coeff;  // = 1;  // Number between 1 and MAX_ZOOM
    int buflen;      // Number of input values that are mapped to the
                     // window buffer

    int auto_scale;  // =  // Holds the maximum value that the window buffer has on the
                     // screen

    int next;  // = 0;  // Holds the position in the window buffer array
               // that is the latest filled

} input_t;
input_t input;

int Input_buffer[10000];
int simulated_data[2000];

// OS handles
extern osThreadId defaultTaskHandle;
extern osThreadId myTask02Handle;
extern osThreadId myTask03Handle;
extern osTimerId myTimer01Handle;

extern osSemaphoreId myBinarySem01Handle;
extern osSemaphoreId myBinarySem02Handle;
extern osMessageQId myQueue01Handle;
extern osMutexId myMutex01Handle;  // Protect LCD
extern osMutexId myMutex02Handle;  // Protect console output

extern osMutexId inputbuf_Handle;   // Protect Input buffer
extern osMutexId windowbuf_Handle;  // Protect Window buffer

// Assignment tasks
extern void Ass_03_Task_01(void const* argument);
extern void Ass_03_Task_02(void const* argument);
extern void Ass_03_Task_03(void const* argument);

// Library functions
extern uint8_t BSP_TP_Init(void);
extern uint8_t BSP_TP_GetDisplayPoint(Coordinate* pDisplay);

// STEPIEN: Safe printf() to ensure that only one task can write to
//          the console at a time

#define safe_printf(fmt, ...)                                                                                          \
    osMutexWait(myMutex02Handle, osWaitForever);                                                                       \
    printf(fmt, ##__VA_ARGS__);                                                                                        \
    osMutexRelease(myMutex02Handle);

//
// ADD YOUR CODE
//

#endif /* ASS_03_H_ */
