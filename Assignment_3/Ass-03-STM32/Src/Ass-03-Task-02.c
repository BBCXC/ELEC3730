//     $Date: 2018-05-10 10:07:15 +1000 (Thu, 10 May 2018) $
// $Revision: 1301 $
//   $Author: Peter $

#include "Ass-03.h"

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE
//

void Ass_03_Task_02(void const * argument)
{
  uint32_t loop=0;
  uint16_t i;
  uint8_t s[100];
  osEvent event;
  int32_t State_Thread1;
  static char* str = "ERROR";
  static int Prev_State = 0;

  safe_printf("Hello from Task 2\n");

  	  // TODO Move to an initilisation function
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  while (1)
  {
	  //osSignalWait(1, osWaitForever);
      event = osMessageGet(myQueue01Handle, osWaitForever);
      if (event.status == osEventMessage)
      {
    	  State_Thread1 = event.value.v;
    	  //TODO Remove this
    	  // TODO Just check the state the the message has sent
    	  // TODO On that state, perform appropriate action on the screen
		  if(State_Thread1 != Prev_State){
			  Prev_State = State_Thread1;
			  if(State_Thread1 == State_STOP){
				  str = "Stop";
			  }
			  else if(State_Thread1 == State_PLAY){
				  str = "Play";
			  }
			  else{
				  str = "ERROR";
			  }

			  PrintState(State_Thread1);
			  sprintf(s,"Task 2: %d (got %s)\n", loop, str);
			  safe_printf("%s", s);
		  }
		  else if(State_Thread1 == State_PLAY){
			  GraphProcess();
		  }

      }
  }
}

//TODO Can be removed,
// TODO this will become part of the button printing function
void PrintState(int State){
	osMutexWait(myMutex01Handle, osWaitForever);
	BSP_LCD_SetFont(&Font12);
	if(State == State_STOP){
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_DisplayStringAt(10, 100, (uint8_t*) "Stop", LEFT_MODE);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	}
	else if(State == State_PLAY){
		BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
		BSP_LCD_DisplayStringAt(10, 50, (uint8_t*) "Play", LEFT_MODE);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	}
	else{
		BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
		BSP_LCD_DisplayStringAt(100, 100, (uint8_t*) "ERROR", LEFT_MODE);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	}
    osMutexRelease(myMutex01Handle);

}

//TODO Change to only clear the window
void ClearWindow() {
    // Get the min and max window parts
    //osMutexWait(windowbuf_Handle, osWaitForever);
//    int x_min = window.position[0];
//    int x_max = window.position[1];
//    int y_min = window.position[2];
//    int y_max = window.position[3];

	int x_min = 0;
	int x_max = BSP_LCD_GetXSize();
	int y_min = 0;
	int y_max = BSP_LCD_GetYSize();
    //osMutexRelease(windowbuf_Handle);

    int windowlen    = x_max - x_min;
    osMutexWait(myMutex01Handle, osWaitForever);
    uint32_t counter = 0;

    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

    for (counter = y_min; counter < y_max; counter++) {
        BSP_LCD_DrawHLine(x_min, counter, windowlen);
    }

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    osMutexRelease(myMutex01Handle);
}

// TODO Remove hard coded hack for input values
// TODO Use the window buffer as input
// TODO Use the window struct
// TODO Take readings from task 1
void GraphProcess() {
    // take the window buffer
    // graph it
    osMutexWait(windowbuf_Handle, osWaitForever);

    int x_min = 68;
	int x_max = BSP_LCD_GetXSize() - 2;
	int y_min = 2;
	int y_max = 144;
	window.line_colour = LCD_COLOR_BLACK;
    int line_colour = window.line_colour;
    // start index of the data
    int element = window.next;

    int Xpos = window.position[0];
    // Scale window buffer to fill the height based on the maximum value in window
    // buffer

    int Ypos = (Window_buffer[element][0] / window.auto_scale) * window.height;
    int Y_prev = 0;
    static int i = 68;
    if(i >= x_max){
    	i = x_min;
    }

	// Scale window buffer to fill the height based on the maximum value in
	// window buffer
	Ypos = 2 + ((144 - 2) / (2147483647.0 - 0)) * (rand() - 0);//(Window_buffer[element][0] / window.auto_scale) * window.height;
	// TODO add mutex wait
	osMutexWait(myMutex01Handle, osWaitForever);
	// erase old dot
	//TODO Remove 5 step jump
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	//Erase the old pixel
	BSP_LCD_DrawVLine(i + 0, y_min, y_max - y_min);
	BSP_LCD_DrawVLine(i + 1, y_min, y_max - y_min);
	BSP_LCD_DrawVLine(i + 2, y_min, y_max - y_min);
	BSP_LCD_DrawVLine(i + 3, y_min, y_max - y_min);
	BSP_LCD_DrawVLine(i + 4, y_min, y_max - y_min);
	//Draw new pixel
	BSP_LCD_SetTextColor(line_colour);
	BSP_LCD_DrawLine(i-5, Y_prev, i, Ypos);
	// TODO add mutex release
	osMutexRelease(myMutex01Handle);

	element++;
	if (element == window.width) {
		element = 0;
	}
	i+=5;
    // TODO add some function that prints the values based on some colour
    // gradient?

    // TODO dont print the graph in the BMP spot if BMP is on
    osMutexRelease(windowbuf_Handle);
}
