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

    osEvent State_Thread;
    int Current_State = 0;

    int Window_buffer[250];

    //  for(int j=0; j<250; j++){
    //	  Window_buffer[j] = 0;
    //  }


    osSignalWait(1, osWaitForever);
    safe_printf("Hello from Task 4 - Analog Input (turn ADC knob or use pulse sensor)\n");

    // Draw a box to plot in
    osMutexWait(myMutex01Handle, osWaitForever);
    BSP_LCD_DrawRect(XOFF - 1, YOFF - 1, XSIZE + 1, YSIZE + 1);
    osMutexRelease(myMutex01Handle);

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
        Current_State = Get_Zoom_Coeff_w();
        if (Current_State == 0) {
            // Stop state
            osDelay(50);
            safe_printf("Stopped at position %d\n", last_xpos);
        }
        else {
            // Wait for first half of buffer
            osSemaphoreWait(myBinarySem05Handle, osWaitForever);
            osMutexWait(myMutex01Handle, osWaitForever);
            for (i = 0; i < 500; i = i + 500) {
                BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                BSP_LCD_DrawVLine(XOFF + xpos, YOFF, YSIZE);
                BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                ypos = (uint16_t)((uint32_t)(ADC_Value[i]) * YSIZE / 4096);
                BSP_LCD_DrawLine(XOFF + last_xpos, YOFF + last_ypos, XOFF + xpos, YOFF + ypos);
                Window_buffer[xpos] = ypos;
                // BSP_LCD_FillRect(xpos,ypos,1,1);
                last_xpos = xpos;
                last_ypos = ypos;
                xpos++;

                safe_printf("First half, %d, %d, %d\n", i, last_xpos, last_ypos);
            }
            osMutexRelease(myMutex01Handle);
            if (last_xpos >= XSIZE - 1) {
                xpos      = 0;
                last_xpos = 0;
            }

            // Wait for second half of buffer
            osSemaphoreWait(myBinarySem06Handle, osWaitForever);
            HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_SET);
            osMutexWait(myMutex01Handle, osWaitForever);
            for (i = 0; i < 500; i = i + 500) {
                BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                BSP_LCD_DrawVLine(XOFF + xpos, YOFF, YSIZE);
                BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                ypos = (uint16_t)((uint32_t)(ADC_Value[i + 500]) * YSIZE / 4096);
                BSP_LCD_DrawLine(XOFF + last_xpos, YOFF + last_ypos, XOFF + xpos, YOFF + ypos);
                // BSP_LCD_FillCircle(xpos,ypos,2);
                last_xpos = xpos;
                last_ypos = ypos;
                xpos++;

                safe_printf("Second half, %d, %d, %d\n", i + 500, last_xpos, last_ypos);
            }
            osMutexRelease(myMutex01Handle);
            if (last_xpos >= XSIZE - 1) {
                xpos      = 0;
                last_xpos = 0;
            }
            HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_RESET);
        }


        /*else{
            // Wait for first half of buffer
            osSemaphoreWait(myBinarySem05Handle, osWaitForever);
            osMutexWait(myMutex01Handle, osWaitForever);
            for(i=0;i<1000;i=i+500){
                BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                BSP_LCD_DrawVLine(XOFF+xpos,YOFF,YSIZE);
                BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
                ypos=(uint16_t)((uint32_t)(ADC_Value[i])*YSIZE/4096);
                Window_buffer[xpos] = ypos;
                safe_printf("Storing Xpos %d = %d, Stored %d\n", xpos, ypos, Window_buffer[xpos]);

                // BSP_LCD_FillRect(xpos,ypos,1,1);
                last_xpos=xpos;
                last_ypos=ypos;
                xpos++;
            }
            int temp2 = xpos;
            int last_ypos2 = 0;
            int ypos2 = 0;
            int temp = 0;

            for(temp=0; temp<250; temp++){
                ypos2 = Window_buffer[temp2];
                //BSP_LCD_DrawLine(XOFF+temp+last_xpos, YOFF+last_ypos2, XOFF+temp+xpos, YOFF+ypos2);
                last_ypos2 = ypos2;
                if(temp < 10){
                    safe_printf("Reading Xpos %d = %d\n", temp2, ypos2);
                }
                //safe_printf("temp %d\n", temp2);
                //safe_printf("Current ADC %4d, %4d, %4d\n", i, temp+last_xpos, last_ypos);
                temp2++;
                if(temp2>=XSIZE-1){
                    temp2 = 0;
                }

            }
            osMutexRelease(myMutex01Handle);
            if (last_xpos>=XSIZE-1)
            {
                xpos=0;
                last_xpos=0;
            }
            HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_RESET);
        }*/
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
