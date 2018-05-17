void Task_01() {
    // Outside of the while loop populate the input with the simulaed data
    for (int i = 0; i < 10000; i++) {
        Input_buffer[j] = simulated_data[j % 2000];
    }
    // Inside the while loop, look for flag

    if (FLAG == 4) {
        // DMA has got new data
        // Process my DMA
        FLAG = 0;
    }


    // Reads touch panel input based on a timer and draws a dot on the LCD screen
    // and send a message to Task 2.

    // Check for touch
    if (touched every X) {
        // Find what was touched
        if (play_button) {
            State_Thread1 = State_PLAY;
        }
        else if (stop_button) {
            State_Thread1 = State_STOP;
        }
        else if (save_button) {
            // call what is essentially the stop function
            // on save ask if user wants to overwrite the current file in the menu
            State_Thread1 = State_STOP;
            if (overwrite_button) {
                // save file name
                // rm current file
                // MUTEX ON INPUT BUFFER
                osMutexWait(inputbuf_Handle, osWaitForever);
                // save file with saved name
                // MUTEX OFF INPUT BUFFER
                osMutexRelease(inputbuf_Handle);
            }
            else if (new_button) {
                // MUTEX ON INPUT BUFFER
                osMutexWait(inputbuf_Handle, osWaitForever);
                // save file with name
                // MUTEX OFF INPUT BUFFER
                osMutexRelease(inputbuf_Handle);
            }
            else if (cancel_button) {
            }
            // redraw what was previously on the screen
        }
        else if (load_button) {
            State_Thread1 = State_STOP;
            // MUTEX ON INPUT BUFFER
            osMutexWait(inputbuf_Handle, osWaitForever);
            // read into input buffer
            // MUTEX ON WINDOW BUFFER
            osMutexWait(windowbuf_Handle, osWaitForever);
            // map to window buffer
            // MUTEX OFF WINDOW BUFFER
            osMutexRelease(windowbuf_Handle);
            // MUTEX OFF INPUT BUFFER
            osMutexRelease(inputbuf_Handle);
            // send to task 2 to draw window buffer - interrupt
            State_Thread1 = State_PLAY;
        }

        else if (zoom function_button) {
            if (scroll left_button) {
                // figure out what exactly needs to happen when scrolling
            }
            else if (scroll right_button) {
                // figure out what exactly needs to happen when scrolling
            }
            else if (zoom in_button) {
                Set_zoom_coeff(1);
            }
            else if (zoom out_button) {
                Set_zoom_coeff(-1);
            }
            else if (reset zoom_button) {
                Set_zoom_coeff(0);
            }
        }
        else if (menu up_button) {
        }
        else if (menu down_button) {
        }
    }
    // Else do nothing
    else {
    }

    switch (State_Thread1) {
        case State_PLAY:
            // take the interrupt from the dma
            // MUTEX ON INPUT BUFFER
            osMutexWait(inputbuf_Handle, osWaitForever);
            // store the 40 samples into the input buffer
            // MUTEX ON WINDOW BUFFER
            osMutexWait(windowbuf_Handle, osWaitForever);
            // map values to window buffer
            if (populateWindow_avg() != 0) {
                printf("%sERROR:%s Could not map input buffer to window buffer\n", ERROR_M, DEFAULT_COLOUR_M);
            }
            // MUTEX OFF WINDOW BUFFER
            osMutexRelease(windowbuf_Handle);
            // MUTEX OFF INPUT BUFFER
            osMutexRelease(inputbuf_Handle);

            // draw window buffer - interrupt
            // TODO this may have to be 5hz or something instead of 25hz
            break;
        case State_STOP:
            // disregard the interrupt given by the dma
            break;
        default:
            // No idea what state we're in, just stop
            State_Thread1 = State_STOP;
            break;
    }
}

// Only called when input buffer has been populated with next 40 samples
int populateWindow_avg() {
    // Increment Window buffer position
    osMutexWait(inputbuf_Handle, osWaitForever);
    osMutexWait(windowbuf_Handle, osWaitForever);
    window.next++;
    if (window.next == window.width) {
        window.next = 0;
    }

    // Increment Input buffer position
    input.next++;
    if (input.next == Max_Samples) {
        input.next = 0;
    }

    // // Alternative to the other max finder
    // int temp = Window_buffer[window.next][0];
    // // if the previous max value is in the bucket i am about to overwrite
    // then re calculate the max
    // otherwise the max remains

    // TODO Decide if this implementation is worth it

    int avg_input = Input_buffer[input.next];

    for (int i = 1; i < window.buflen; i++) {
        input.next++;
        avg_input += Input_buffer[input.next];
    }
    osMutexRelease(inputbuf_Handle);
    // I want an int without losing the double division
    Window_buffer[window.next][0] = (int) (avg_input / (double) window.buflen);

    window.auto_scale = 0;
    for (int i = 0; i < window.width; i++) {
        if (Window_buffer[i][0] > window.auto_scale) {
            window.auto_scale = Window_buffer[i][0];
        }
    }
    osMutexRelease(windowbuf_Handle);

    // Window buffer now contains average of last 40 samples
    return 0;
}

void Set_zoom_coeff(int direction) {
    // Need to decide where this fits best.
    // Maybe should be contained within thread 1 and only the window buffer passed
    // to thread 2
    osMutexWait(windowbuf_Handle, osWaitForever);
    if (direction == 0) {
        // reset zoom
        window.zoom_coeff = 0;
    }
    else if (direction == -1) {
        // zoom out
        if (window.zoom_coeff != MIN_ZOOM) {
            window.zoom_coeff += -1;
        }
    }
    else if (direction == 1) {
        // zoom in
        if (window.zoom_coeff != MAX_ZOOM) {
            window.zoom_coeff += 1;
        }
    }
    osMutexRelease(windowbuf_Handle);

    // Re populate window
    // Redraw window
}

// clang-format off
/***********************************************************************************************************************
******************************************************* Thread 2 *******************************************************
***********************************************************************************************************************/
// clang-format on

void Task_02() {
    // Waits for a message from Task 1 and displays the result.

    // If signal from task 1
    // this might be message or signal

    // TODO This could be updated less frequenty if needed. eg 5hz or less

    // Do something
    GraphProcess();
    // Each time you draw the graph draw the BPM over the top of the graph
    Draw_BMP();

    //
}

void ClearWindow() {
    // Get the min and max window parts
    osMutexWait(windowbuf_Handle, osWaitForever);
    int x_min = window.position[0];
    int x_max = window.position[1];
    int y_min = window.position[2];
    int y_max = window.position[3];
    osMutexRelease(windowbuf_Handle);

    int windowlen    = x_max - x_min osMutexWait(myMutex01Handle, osWaitForever);
    uint32_t counter = 0;

    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

    for (counter = y_min; counter < y_max; counter++) {
        BSP_LCD_DrawHLine(x_min, counter, windowlen);
    }

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    osMutexRelease(myMutex01Handle);
}

void GraphProcess() {
    // take the window buffer
    // graph it
    osMutexWait(windowbuf_Handle, osWaitForever);

    int x_min = window.position[0];
    int x_max = window.position[1];
    int y_min = window.position[2];
    int y_max = window.position[3];

    int line_colour = window.line_colour;
    // start index of the data
    int element = window.next;

    int Xpos = window.position[0];
    // Scale window buffer to fill the height based on the maximum value in window
    // buffer

    int Ypos = (Window_buffer[element][0] / window.auto_scale) * window.height;

    for (int i = Xpos; i < window.position[1]; i++) {
        // Scale window buffer to fill the height based on the maximum value in
        // window buffer
        Ypos = (Window_buffer[element][0] / window.auto_scale) * window.height;
        // TODO add mutex wait
        osMutexWait(myMutex01Handle, osWaitForever);
        // erase old dot
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DrawVLine(x_min + element, y_min, y_max - y_min);
        LCD_DrawPixel(i, Ypos, line_colour);
        // TODO add mutex release
        osMutexRelease(myMutex01Handle);

        element++;
        if (element == window.width) {
            element = 0;
        }
    }
    // TODO add some function that prints the values based on some colour
    // gradient?

    // TODO dont print the graph in the BMP spot if BMP is on
    osMutexRelease(windowbuf_Handle);
}

typedef struct {
    int window[4];  // Position of BPM
    int value;      // Current BPM value

    int toggle;  // Whether BPM is active or not

} BMP_data_t;
BMP_data_t bmp;

void Draw_BMP() {
    // take given BMP
    // draw in given spot

    // TODO add bpm initilization function to remove drawing the "BMP" part again

    // TODO add bpm toggle

    x_min = bmp.window[0];
    x_max = bmp.window[1];
    y_min = bmp.window[2];
    y_max = bmp.window[3];

    char* output;

    sprintf(output, "BPM: %d", bmp.value);

    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(x_max + 1, y_min + 1, (uint8_t*) output, LEFT_MODE);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

    // TODO Add colour to BPM reading if values is about preset limit
}