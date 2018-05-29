//     $Date: 2018-05-22 06:24:02 +1000 (Tue, 22 May 2018) $
// $Revision: 1330 $
//   $Author: Peter $

#include "Ass-03.h"

// This is the task that reads the analog input. A buffer is divided in two to
// allow working on one half of the buffer while the other half is being
// loaded using the DMA controller.
//
// This task also plots the wave form to the screen.
//
// *** MAKE UPDATES TO THE CODE AS REQUIRED ***
//
// Note that there needs to be a way of starting and stopping the display.

uint16_t ADC_Value[1000];

void Ass_03_Task_04(void const* argument) {
    uint16_t i;
    HAL_StatusTypeDef status;
    uint16_t xpos      = 0;
    uint16_t ypos      = 0;
    uint16_t last_xpos = 0;
    uint16_t last_ypos = 0;

    int Current_State  = 0;
    int Previous_State = 0;

    int Buf_len                = 250;
    uint16_t *Window_buffer = calloc(250, sizeof(int));
    static int semaphore_state = 0;
    static int begin = 0;
    static int win_ptr = 0;

    uint16_t ADC_Value[1000];

    osSignalWait(1, osWaitForever);
    safe_printf("Hello from Task 4 - Analog Input (turn ADC knob or use pulse sensor)\n");

    Task_4_Init();

    safe_printf("Task_4 Successfully initilised\n");

    // Start the conversion process
    status = HAL_ADC_Start_DMA(&hadc1, (uint32_t*) &ADC_Value, 1000);
    if (status != HAL_OK) {
        safe_printf("ERROR: Task 4 HAL_ADC_Start_DMA() %d\n", status);
    }

    // Start main loop
    while (1) {
        //		State_Thread = osMessageGet(myQueue02Handle, osWaitForever);
        //	    if (State_Thread.status == osEventMessage){
        //	    	Current_State =  (uint16_t)(State_Thread.value.v);
        //	    	safe_printf("Current State %d\n", Current_State);
        //	    }
        Current_State = Get_State_Thread();
        if (Current_State == 0) {
            // Stop state
            osDelay(100);
            safe_printf("Stopped at position %d\n", last_xpos);
        }
        else if (Current_State == 1) {
            Previous_State = 1;
            // Wait for first half of buffer
            int first = 1;
            int avg = 0;
            // If these change maybe i should reprint it all
			int Samples = Get_Zoom_Coeff_w() * 1000;
			int Bin_len = Samples / Buf_len;

			// Begin
			// Bin_len ~ 40 can change with zoom
			// Max value is 10,000
			if (begin % 1000 > 500) {
				// First semaphore on
				semaphore_state = 0;
			}
			else {
				// Second semaphore on
				semaphore_state = 1;
			}
			for (int i = begin; i < begin + Bin_len; i++) {
				// Store the ADC_Value in the ADC_Buffer
				if ((begin % 1000 > 500) && semaphore_state == 1) {
					osSemaphoreWait(myBinarySem05Handle, osWaitForever);
					semaphore_state = 0;
				}
				else if ((begin % 1000 <= 500) && semaphore_state == 0) {
					osSemaphoreWait(myBinarySem06Handle, osWaitForever);
					semaphore_state = 1;
				}

				// Add up all of the average values
				avg += ADC_Value[begin] * YSIZE / 4096;
				//safe_printf("ADC_Value[begin] %d, avg %d, state %d\n", ADC_Value[begin], avg, semaphore_state);// * YSIZE / 4096, avg);
			}
			//safe_printf("Avg %d, state %d\n", avg, semaphore_state);// * YSIZE / 4096, avg);
			static int count = 0;
			//safe_printf("Count %d\n", count++);
			    // Store the average in the buffer
//			    Increment_Win_Ptr();
				win_ptr++;
				if (win_ptr >= 250) {
					win_ptr = 0;
				}
			    Window_buffer[win_ptr] = (int) (avg / ((double) Bin_len));
			    //safe_printf("buf %d\n", Window_buffer[win_ptr]);

//			    // Find max
//			    int Max_Win = 0;
//			    for (int i = 0; i < 250; i++) {
//			        if (Window_buffer[i] > Max_Win) {
//			            Max_Win = Window_buffer[i];
//			        }
//			    }
			    last_xpos = 0;
			    last_ypos = 0;
			    osMutexWait(myMutex01Handle, osWaitForever);
			    for(int i = 0; i < 250; i++){
			    	win_ptr++;
					if (win_ptr >= 250) {
						win_ptr = 0;
					}
					BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
					BSP_LCD_DrawVLine(XOFF + i, YOFF, YSIZE);
					BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
					ypos = Window_buffer[win_ptr];
					BSP_LCD_DrawLine(XOFF + last_xpos, YOFF + last_ypos, XOFF + i, YOFF + ypos);



				   last_xpos = i;
				   last_ypos = ypos;

			    }
			    osMutexRelease(myMutex01Handle);

			    //safe_printf("x1 %d, y1 %d, x2 %d, y2 %d\n",last_xpos, last_ypos, win_ptr, ypos)


			    begin += Bin_len;
			    //safe_printf("Begin %d Samples\n", begin);
			    if (begin >= Samples / Get_Zoom_Coeff_w()) {
			        begin = 0;
			        //safe_printf("1000 Samples\n");
			    }

			    osDelay(100);
//			    // Draw Dot
//			    int scale     = 1;//? ? ;
//			    int win_ptr_s = win_ptr;
//			    int y_pos     = Window_buffer[win_ptr];
//			    int x_pos     = win_ptr;
//			    win_ptr++;
//			    if (win_ptr >= 250) {
//			        win_ptr = 0;
//			    }
//			    while (win_ptr_s != win_ptr) {
//			        // TODO scale
//			        BSP(x_pos, y_pos * scale, win_ptr, Window_buffer[win_ptr] * scale);
//			        x_pos = win_ptr;
//			        y_pos = Window_buffer[win_ptr];
//			        win_ptr++;
//			        if (win_ptr >= 250) {
//			            win_ptr = 0;
//			        }
//			    }
//			    win_ptr++;
//            osSemaphoreWait(myBinarySem05Handle, osWaitForever);
//            osMutexWait(myMutex01Handle, osWaitForever);
//            for (i = 0; i < 1000; i = i + 500) {
//                BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
//                BSP_LCD_DrawVLine(XOFF + xpos, YOFF, YSIZE);
//                BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
//                ypos = (uint16_t)((uint32_t)(ADC_Value[i]) * YSIZE / 4096);
//                BSP_LCD_DrawLine(XOFF + last_xpos, YOFF + last_ypos, XOFF + xpos, YOFF + ypos);
//
//                last_xpos = xpos;
//                last_ypos = ypos;
//                xpos++;
//                if (last_xpos >= XSIZE - 1) {
//                    xpos      = 0;
//                    last_xpos = 0;
//                }
//                if (i < 1000 / 2) {
//                }
//                else if (first == 1) {
//                    first = 0;
//                    // Wait for second half of buffer
//                    osSemaphoreWait(myBinarySem06Handle, osWaitForever);
//                    HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_SET);
//                }
//                else {
//                }
//            }
//            osMutexRelease(myMutex01Handle);
//            if (last_xpos >= XSIZE - 1) {
//                xpos      = 0;
//                last_xpos = 0;
//            }
//            HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_RESET);
        }
        else if (Current_State == 2) {
            if (Previous_State != 2) {
                Previous_State = 2;
                // Draw rectangle
                // Clear inside rectangle
                // Draw buttons inside
                // Set popup to on
                int pop_pos_0 = Get_Popup_Position(0);
                int pop_pos_1 = Get_Popup_Position(1);
                int pop_pos_2 = Get_Popup_Position(2);
                int pop_pos_3 = Get_Popup_Position(3);

                // Draw a box to plot in
                osMutexWait(myMutex01Handle, osWaitForever);

                BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                BSP_LCD_FillRect(pop_pos_0, pop_pos_1, pop_pos_2 - pop_pos_0, pop_pos_3 - pop_pos_1);

                BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                BSP_LCD_DrawRect(pop_pos_0, pop_pos_1, pop_pos_2 - pop_pos_0, pop_pos_3 - pop_pos_1);

                osMutexRelease(myMutex01Handle);

                pbutton_init();
            }
            else {
                osDelay(100);
            }
        }
        else {
            safe_printf("Current State is %d\n", Current_State);
        }

        // Check if we have changed directory
        if (Get_Dir_Chg() == 1) {

            Set_Dir_Chg(0);
            // Refresh the files shown
            Set_File_Index(0);
            DEBUG_P
            safe_printf("Num %d, Index %d\n", Get_File_Num(), Get_File_Index());
            DEBUG_P
            if (Get_File_Index() < Get_File_Num()) {

                int index = Get_File_Index();

                char* File_Name = "This is only a test";
                DEBUG_P
                Get_File_Name(&File_Name, index);
                DEBUG_P
                safe_printf("File_Name ----> %s\n", File_Name);
                DEBUG_P

                Set_File_Index(Get_File_Index() + 1);
                int x_pos = Get_File_Window_x();
                int y_pos = Get_File_Window_y();
                osMutexWait(myMutex01Handle, osWaitForever);
                BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                BSP_LCD_DisplayStringAt(x_pos, y_pos, (uint8_t*) File_Name, LEFT_MODE);
                osMutexRelease(myMutex01Handle);
            }
        }
    }
}

// STEPIEN: Add callback functions to see if this can be used for double buffering equivalent

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc) {
    osSemaphoreRelease(myBinarySem05Handle);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_SET);
    osSemaphoreRelease(myBinarySem06Handle);
    HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_RESET);
}

void Task_4_Init() {

    // Draw a box to plot in
    osMutexWait(myMutex01Handle, osWaitForever);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_DrawRect(XOFF - 1, YOFF - 1, XSIZE + 1, YSIZE + 1);
    osMutexRelease(myMutex01Handle);

    button_init();
}
