//     $Date: 2018-05-21 09:11:26 +1000 (Mon, 21 May 2018) $
// $Revision: 1327 $
//   $Author: Peter $

#include "Ass-03.h"

// This is the console task. Can use your code from the previous assignment
// and expand.
//
// The example code is very basic, showing access to the SD card. Note that
// if the SD card is access from both this task and the pulse rate monitor
// task, a MUTEX will be required.
//
// *** MAKE UPDATES TO THE CODE AS REQUIRED ***
//

uint8_t myReadFile();

FIL MyFile;
FIL MyFile2, MyFile3;
FRESULT Status;

void Ass_03_Task_01(void const * argument)
{

  uint32_t loop=0;
  uint8_t ts[100];
  uint16_t i;
  Coordinate display;
  char c;

  safe_printf("Hello from Task 1 (Initialise LCD and TP first)\n");

  // STEPIEN: Initialize and turn on LCD and calibrate the touch panel
  BSP_LCD_Init();
  BSP_LCD_DisplayOn();
  BSP_TP_Init();
  //
  // Signal other tasks to start
  osSignalSet(myTask02Handle, 1);
  osSignalSet(myTask03Handle, 1);
  osSignalSet(myTask04Handle, 1);

  // Display welcome message
  osMutexWait(myMutex01Handle, osWaitForever);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(5, 5, (uint8_t*)"ELEC3730 Assignment 3 (v02 $Rev: 1327 $)",LEFT_MODE);
  BSP_LCD_DisplayStringAt(5,20, (uint8_t*)"This is some demonstration code",LEFT_MODE);
  BSP_LCD_DisplayStringAt(5,35, (uint8_t*)"that can be used as a starting point",LEFT_MODE);
  BSP_LCD_DisplayStringAt(5,50, (uint8_t*)"for the assignment.",LEFT_MODE);
  osMutexRelease(myMutex01Handle);

  while (1)
  {
	  c = getchar();
	  safe_printf("Task 1: %d (got '%c')",loop,c);
      loop++;
      myReadFile();
  }

}

uint8_t myReadFile()
{
#define BUFF_SIZE 256
	uint8_t rtext[BUFF_SIZE];
	FRESULT res;
	uint32_t bytesread;

	// Open file Hello.txt
	if((res = f_open(&MyFile, "Hello.txt", FA_READ)) != FR_OK)
	{
		safe_printf("ERROR: Opening 'Hello.txt'\n");
		return 1;
	}
	safe_printf("Task 1: Opened file 'Hello.txt'\n");

	// Read data from file
	if ((res = f_read(&MyFile, rtext, BUFF_SIZE-1, &bytesread)) != FR_OK)
	{
		safe_printf("ERROR: Reading 'Hello.txt'\n");
		f_close(&MyFile);
		return 1;
	}
	rtext[bytesread] = '\0';
	safe_printf("Task 1: Read: %s\n", rtext);

	// Close file
	f_close(&MyFile);

	return 0;
}

