uint8_t ADC_Value[1000];
int Buf_len             = 250;
uint16_t* Window_buffer = calloc(250, sizeof(int));

// Start the conversion process
status = HAL_ADC_Start_DMA(&hadc1, (uint32_t*) &ADC_Value, 1000);
if (status != HAL_OK) {
    safe_printf("ERROR: Task 4 HAL_ADC_Start_DMA() %d\n", status);
}

// Amount of zoom in seconds 1 to 10
int Zoom = 10;  // seconds
int avg  = 0;

// Start main loop
while (1) {
    // How many seconds do i want to display in hz
    int Samples = Zoom * 1000;
    // How big is each average bucket
    int Bin_len = Samples / Buf_len;

    // Look at the 1 second worth of data
    while (begin < 1000) {
        // Wait for first half of buffer
        if (begin > 500) {
            // If i'm in the first half of the buffer
            semaphore_state = 0;
        }
        else {
            // Second semaphore on
            semaphore_state = 1;
        }
        for (int i = begin; i < begin + Bin_len; i++) {
            // If im in the first half of the buffer wait for it
            if ((begin > 500) && semaphore_state == 1) {
                osSemaphoreWait(myBinarySem05Handle, osWaitForever);
                semaphore_state = 0;
            }
            // Otherwise im in the second half so wait for that
            else if ((begin <= 500) && semaphore_state == 0) {
                osSemaphoreWait(myBinarySem06Handle, osWaitForever);
                semaphore_state = 1;
            }

            // Add up all of the average values
            avg += ADC_Value[begin] * YSIZE / 4096;
        }

        // Increment the index of the buffer
        win_ptr++;
        if (win_ptr >= 250) {
            win_ptr = 0;
        }

        // Store the average
        Window_buffer[win_ptr] = (int) (avg / ((double) Bin_len));

        begin += Bin_len;
    }

    // Print my circular buffer
    last_xpos = 0;
    last_ypos = 0;
    osMutexWait(myMutex01Handle, osWaitForever);
    for (int i = 0; i < 250; i++) {
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
}