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

// uint16_t ADC_Value[1000];

void Ass_03_Task_04(void const* argument) {
    uint16_t i;
    HAL_StatusTypeDef status;
    uint16_t xpos           = 0;
    uint16_t ypos           = 0;
    uint16_t last_xpos      = 0;
    uint16_t last_ypos      = 0;
    uint16_t last_last_xpos = 0;
    uint16_t last_last_ypos = 0;

    int Current_State  = 0;
    int Previous_State = 0;

    static int Get_Zoom_Coeff_w = 10;
    int zoom             = 500;
    int max              = 142;
    int scale            = 1;

    int Buf_len                = 250;
    int* Window_buffer         = calloc(250, sizeof(int));
    static int semaphore_state = 0;
    static int begin           = 0;
    static int win_ptr         = 0;

    int delta_bpm   = 100;
    int bpm_samples = 0;
    double bpm_avg  = 0;
    int prev_bpm    = 0;
    int last_bpm    = 0;

    int recording    = 0;
    int record_steps = 0;
    int record_time  = 0;
    int record_delay = 0;


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
            // safe_printf("Stopped at position %d\n", last_xpos);
        }
        //        else if (Current_State == 1) {
        //        	// Wait for first half of buffer
        //        	    int first = 1;
        //
        //        	    // If these change maybe i should reprint it all
        //        	    int Samples = Get_Zoom_Coeff_w * 1000;  // (1 to 10) * 1000hz
        //        	    int Bin_len = Samples / Buf_len;
        //
        ////        	    while (begin < 1000) {
        //        	        double avg = 0;
        //        	        if (begin > 500) {
        //        	            // First semaphore on
        //        	            semaphore_state = 0;
        //        	        }
        //        	        else {
        //        	            // Second semaphore on
        //        	            semaphore_state = 1;
        //        	        }
        ////        	        for (int i = begin; i < begin + Bin_len;) {
        //        	            // Store the ADC_Value in the ADC_Buffer
        //        	            if ((begin > 500) && semaphore_state == 1) {
        //        	                osSemaphoreWait(myBinarySem05Handle, osWaitForever);
        //        	                semaphore_state = 0;
        //        	            }
        //        	            else if ((begin <= 500) && semaphore_state == 0) {
        //        	                osSemaphoreWait(myBinarySem06Handle, osWaitForever);
        //        	                semaphore_state = 1;
        //        	            }
        //
        //        	            // Add up all of the average values
        ////        	            avg += ADC_Value[i];  // * ((double)YSIZE / 4096.0);
        ////        	            printf("avg %lf ADC_Value %d\n", avg, ADC_Value[i]);
        ////        	            i = i + Bin_len;
        ////        	        }
        //
        //        	        win_ptr++;
        //        	        if (win_ptr >= 250) {
        //        	            win_ptr = 0;
        //        	        }
        //        	        Window_buffer[win_ptr] = (double)((double)142.0 * ((double)ADC_Value[begin] /
        //        (double)4096.0));//YSIZE * ((double) avg / (4096.0));  // TODO Map to the max
        //        	        //printf("Win_buf %d is %lf\n", win_ptr, Window_buffer[win_ptr]);
        //        	        max = 0.0;
        //        	        for(int i = 0; i < 250; i++){
        //        	        	if (Window_buffer[i] > max) {
        //        	        		max = Window_buffer[i];
        //        	        	}
        //        	        }
        //
        //        	        begin += Bin_len;
        //        	        // if (begin >= 1000) {
        //        	        //     begin = 0;
        //        	        //     // safe_printf("1000 Samples\n");
        //        	        // }
        ////        	    }
        //
        //        	   //printf("Max is %lf\n", max);
        //        	    double scale = (double)((double) (142.0) / ((double) max));
        //        	    //printf("Scale is %lf\n", scale);
        //        	    safe_printf("pos %3d, buffer %3.2lf, max %3.2lf, scale %2.2lf, output %4d\n",win_ptr,
        //        Window_buffer[win_ptr], max, scale, ((int) (Window_buffer[win_ptr] * scale)));
        //
        //        	    int last_xpos = 0;
        //        	    int last_ypos = 0;
        //        	    // osMutexWait(myMutex01Handle, osWaitForever);
        //        	    for (int i = 0; i < 250; i++) {
        //        	    	 osMutexWait(myMutex01Handle, osWaitForever);
        //        	        win_ptr++;
        //        	        if (win_ptr >= 250) {
        //        	            win_ptr = 0;
        //        	        }
        //        	         BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        //        	         BSP_LCD_DrawVLine(XOFF + i, YOFF, YSIZE);
        //        	         BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        //        	        int ypos = (int) (Window_buffer[win_ptr] * scale);
        //        	        //printf("px %3d, py %3d, cx %3d, cy %3d\n", XOFF + last_xpos, YOFF + last_ypos, XOFF + i,
        //        YOFF + ypos);
        //        	        //fprintf(fp, "%d, %d\n", i, ypos);
        //        	        BSP_LCD_DrawLine(XOFF + last_xpos, YOFF + last_ypos, XOFF + i, YOFF + ypos);
        //
        //        	        last_xpos = i;
        //        	        last_ypos = ypos;
        //
        //        	        osMutexRelease(myMutex01Handle);
        //        	    }
        //        	    // osMutexRelease(myMutex01Handle);
        //        	    //osDelay(10000);
        //        }

        else if (Current_State == 1) {
            if (recording == 1) {
                if (record_delay > 0) {
                	safe_printf("Recording in %d\n", record_delay);
                	record_delay--;
                }
                else if (record_steps > record_time * 1000 / zoom) {
                	safe_printf("Recording complete\n");
                    Set_State_Thread(0);
                    int data_len = 250;
                    int data[250];
                    for(int i=0; i<data_len; i++){
                    	data[i] = Window_buffer[win_ptr];
                    	win_ptr++;
                    	if(win_ptr >= 250){
                    		win_ptr = 0;
                    	}
                    }

                    // TODO
                    int save_state = 0;
                    char* FILE_NAME = "test.csv";
                    safe_printf("Writing recorded data\n");
                    Write_CSV(FILE_NAME, save_state, data, data_len);
                    recording = 0;
                }
                else{
                	safe_printf("Recording delay %d\n", record_delay);
                }
                if (record_delay == 0) {
                    record_steps++;
                    safe_printf("Record steps %d of %d\n", record_steps, record_time * 1000 / zoom);
                }
            }

            Previous_State = 1;
            // Wait for first half of buffer
            int first = 1;
            zoom      = 50 * Get_Zoom_Coeff_w;
            osSemaphoreWait(myBinarySem05Handle, osWaitForever);
            for (i = 0; i < 1000; i = i + zoom) {
                osMutexWait(myMutex01Handle, osWaitForever);
                BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                BSP_LCD_DrawVLine(XOFF + last_xpos, YOFF, YSIZE);
                BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

                BSP_LCD_DrawLine(XOFF + last_last_xpos, YOFF + last_last_ypos, XOFF + last_xpos, YOFF + last_ypos);


                BSP_LCD_SetTextColor(LCD_COLOR_RED);
                BSP_LCD_DrawVLine(XOFF + xpos, YOFF, YSIZE);
                BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                ypos = (uint16_t)((uint32_t)(ADC_Value[i])) * YSIZE / 4096;
                ypos = -1 * (ypos - YSIZE);
                BSP_LCD_DrawLine(XOFF + last_xpos, YOFF + last_ypos, XOFF + xpos, YOFF + ypos);
                //                Window_buffer[xpos] = ypos;
                // BSP_LCD_FillRect(xpos,ypos,1,1);
                last_last_xpos = last_xpos;
                last_last_ypos = last_ypos;
                last_xpos      = xpos;
                last_ypos      = ypos;
                xpos++;
                if (last_xpos >= XSIZE - 1) {
                    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                    BSP_LCD_DrawVLine(XOFF + last_xpos, YOFF, YSIZE);
                    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                    xpos           = 0;
                    last_xpos      = 0;
                    last_last_xpos = 0;
                }
                if (i < 1000 / 2) {
                    // safe_printf("First half, %d, %d, %d\n", i, last_xpos, last_ypos);
                }
                else if (first == 1) {
                    first = 0;
                    // Wait for second half of buffer
                    osSemaphoreWait(myBinarySem06Handle, osWaitForever);
                    HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_SET);
                    // safe_printf("Second half, %d, %d, %d\n", i, last_xpos, last_ypos);
                }
                else {
                    // safe_printf("Second half, %d, %d, %d\n", i, last_xpos, last_ypos);
                }
                osMutexRelease(myMutex01Handle);
                win_ptr++;
                if (win_ptr >= 250) {
                    win_ptr = 0;
                }
                Window_buffer[win_ptr] = (uint16_t)((uint32_t)(ADC_Value[i]));
                max                    = 0;
                for (int pos = 0; pos < 250; pos++) {
                    if (Window_buffer[pos] > max) {
                        max = Window_buffer[pos];
                        // safe_printf("Found new max %d\n", max);
                    }
                }
                // safe_printf("Current Val %d\n", ypos);
            }
            // safe_printf("Current Max %d\n", max);
            // scale = (int)(((double)YSIZE) / ((double)max));
            // safe_printf("Scale is %d, %d, %d\n", scale, YSIZE, max);

            for (int pos = 5; pos < 250; pos += 5) {
                if (Window_buffer[pos] - Window_buffer[pos - 5] > delta_bpm) {

                    if (prev_bpm != 0) {
                        bpm_samples++;
                        bpm_avg = (double) (bpm_avg + pos - prev_bpm) / 2.0;
                        // safe_printf("I got a sample, %g, %d\n", bpm_avg, pos - prev_bpm);
                        prev_bpm = pos;
                    }
                    else {
                        prev_bpm = pos;
                    }
                }
                else {
                    // safe_printf("%d, %d, %d\n", Window_buffer[pos], Window_buffer[pos - 5], Window_buffer[pos] -
                    // Window_buffer[pos - 5]);
                }
            }
            last_bpm++;
            if (last_bpm >= 50) {
                last_bpm    = 0;
                prev_bpm    = 0;
                bpm_samples = 0;

                safe_printf("BPM is %d\n", (int) (bpm_avg * 60 / Get_Zoom_Coeff_w));
            }

            if (last_xpos >= XSIZE - 1) {
                osMutexWait(myMutex01Handle, osWaitForever);
                BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                BSP_LCD_DrawVLine(XOFF + last_xpos, YOFF, YSIZE);
                BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                osMutexRelease(myMutex01Handle);
                xpos           = 0;
                last_xpos      = 0;
                last_last_xpos = 0;
            }
            HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_RESET);
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
        else if (Current_State == 3) {
            // We must be recording
            // Set the record variable
            record_time = Get_Record_Time();
            // Set the delay time
            record_delay = Get_Record_Delay();
            // State recording
            safe_printf("You want to record??\n");
            Get_Zoom_Coeff_w = record_time;
            recording = 1;

            Set_State_Thread(1);
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

// Map the xy range to the screen xy range
int Map_Y_Display(int Input, int max) {
    return (144 - ((Input - 0) / (max - 0) * (144 - 2) + 2));
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
