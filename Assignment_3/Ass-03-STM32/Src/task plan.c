#define PLAY_BUTTON "PLAY"
#define STOP_BUTTON "STOP"
#define SAVE_BUTTON "SAVE"
#define OVERWRITE_BUTTON "OVERWRITE"
#define NEW_BUTTON "NEW"
#define CANCEL_BUTTON "CANCEL"
#define LOAD_BUTTON "LOAD"
#define LEFT_SCROLL "LEFT"
#define RIGHT_SCROLL "RIGHT"
#define ZOOM_IN "IN"
#define ZOOM_OUT "OUT"
#define ZOOM_RESET "RESET"
#define MENU_UP "UP"
#define MENU_DOWN "DOWN"

#define State_PLAY 1
#define State_STOP 0

#include "Ass-03.h"

void Ass_03_Task_01(void const* argument) {
    // Outside of the while loop populate the input with the simulaed data

    Simulated_data();
    windowInit();

    for (int i = 0; i < 10000; i++) {
        Input_buffer[i] = simulated_data[i % 2000];
    }
    while (1) {
        static int State_Thread1 = State_PLAY;
        // Inside the while loop, look for simulated_DMA == 1

        // Reads touch panel input based on a timer and draws a dot on the LCD screen
        // and send a message to Task 2.

        // Check for touch
        if (BSP_TP_GetDisplayPoint(&display) == 0) {
            // Find what was touched
            if (PLAY_BUTTON) {
                State_Thread1 = State_PLAY;
            }
            else if (STOP_BUTTON) {
                State_Thread1 = State_STOP;
            }
            else if (SAVE_BUTTON) {
                // call what is essentially the stop function
                // on save ask if user wants to overwrite the current file in the menu
                State_Thread1 = State_STOP;
                if (OVERWRITE_BUTTON) {
                    // save file name
                    // rm current file
                    // MUTEX ON INPUT BUFFER
                    osMutexWait(inputbuf_Handle, osWaitForever);
                    // save file with saved name
                    // MUTEX OFF INPUT BUFFER
                    osMutexRelease(inputbuf_Handle);
                }
                else if (NEW_BUTTON) {
                    // MUTEX ON INPUT BUFFER
                    osMutexWait(inputbuf_Handle, osWaitForever);
                    // save file with name
                    // MUTEX OFF INPUT BUFFER
                    osMutexRelease(inputbuf_Handle);
                }
                else if (CANCEL_BUTTON) {
                }
                // redraw what was previously on the screen
            }
            else if (LOAD_BUTTON) {
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

            else if (LEFT_SCROLL) {
                // figure out what exactly needs to happen when scrolling
            }
            else if (RIGHT_SCROLL) {
                // figure out what exactly needs to happen when scrolling
            }
            else if (ZOOM_IN) {
                Set_zoom_coeff(1);
            }
            else if (ZOOM_OUT) {
                Set_zoom_coeff(-1);
            }
            else if (ZOOM_RESET) {
                Set_zoom_coeff(0);
            }

            else if (MENU_UP) {
            }
            else if (MENU_DOWN) {
            }
        }
        // Else do nothing
        else {
        }

        switch (State_Thread1) {
            case State_PLAY:
                // take the interrupt from the dma
                if (Simulated_DMA() == 1) {
                    // MUTEX ON INPUT BUFFER
                    osMutexWait(inputbuf_Handle, osWaitForever);
                    // store the 40 samples into the input buffer
                    // TODO Probably won't be able to cheat this but oh well
                    // Move on you've already done that]
                    input.next += 40;
                    // MUTEX ON WINDOW BUFFER
                    osMutexWait(windowbuf_Handle, osWaitForever);
                    // map values to window buffer
                    if (populateWindow_avg() != 0) {
                        safe_printf(
                            "%sERROR:%s Could not map input buffer to window buffer\n", ERROR_M, DEFAULT_COLOUR_M);
                    }
                    // MUTEX OFF WINDOW BUFFER
                    osMutexRelease(windowbuf_Handle);
                    // MUTEX OFF INPUT BUFFER
                    osMutexRelease(inputbuf_Handle);
                }

                // draw window buffer - interrupt
                // TODO this may have to be 5hz or something instead of 25hz
                break;
            case State_STOP:
                // disregard the interrupt given by the dma
                if (Simulated_DMA() == 1) {
                }
                else {
                    // Do something
                }
                break;

            default:
                // No idea what state we're in, just stop
                State_Thread1 = State_STOP;
                break;
        }
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

void windowInit() {
   // int Window_buffer[window.width][2];// = () calloc();
	window.zoom_coeff = 1;
    window.buflen = Max_Samples / window.zoom_coeff / window.width;
}

// clang-format off
/***********************************************************************************************************************
******************************************************* Thread 2 *******************************************************
***********************************************************************************************************************/
// clang-format on

void Ass_03_Task_02(void const* argument) {
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

    int x_min = bmp.window[0];
    int x_max = bmp.window[1];
    int y_min = bmp.window[2];
    int y_max = bmp.window[3];

    char* output;

    sprintf(output, "BPM: %d", bmp.value);

    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(x_max + 1, y_min + 1, (uint8_t*) output, LEFT_MODE);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

    // TODO Add colour to BPM reading if values is about preset limit
}