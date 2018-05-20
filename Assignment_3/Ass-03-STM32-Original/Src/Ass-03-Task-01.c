//     $Date: 2018-05-10 10:07:15 +1000 (Thu, 10 May 2018) $
// $Revision: 1301 $
//   $Author: Peter $

#include "Ass-03.h"

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE
//

void Ass_03_Task_01(void const* argument) {
    // TODO Initilise the input buffer, window buffer, buttons etc here
    windowInit();

    static int State_Thread1 = State_PLAY;

    static int button_debounce = 0;
    static int off_debounce    = 0;
    static int holding         = 0;
    static int popup           = 0;

    while (1) {
        osMessagePut(myQueue01Handle, (uint32_t) State_Thread1, 0);
        // Inside the while loop, look for simulated_DMA == 1

        // Reads touch panel input based on a timer and draws a dot on the LCD
        // screen and send a message to Task 2.

        // Check for touch
        if (BSP_TP_GetDisplayPoint(&display) == 0) {
            button_debounce++;

            // If the button has successfully debounced and the user isn't holding the
            // button

            // TODO Change the debounce to use a timer
            if (button_debounce >= 50 && holding == 0) {
                button_debounce = 0;
                off_debounce    = 0;
                holding         = 1;

                // TODO Get Touch Position
                touch_pos = get_touch_pos(display.x, display.y, popup);
                // TODO Decide what button was pressed

                // This will become a check for each button pressed.
                // if (State_Thread1 == State_PLAY) {
                //     State_Thread1 = State_STOP;
                //     safe_printf("State set to stop\n");
                // }
                // else if (State_Thread1 == State_STOP) {
                //     State_Thread1 = State_PLAY;
                //     safe_printf("State set to play\n");
                // }
            }

            else if (button_debounce >= 50 && holding == 1) {
                button_debounce = 0;
                off_debounce    = 0;
            }
        }
        // No button pressed, debounce this
        else {
            off_debounce++;
            // User is definately not pressing a button, reset the holding flag
            if (off_debounce > 100) {
                holding         = 0;
                button_debounce = 0;
                off_debounce    = 0;
            }
        }

        // TODO Process this each round through
        // TODO Probably need a signal wait or something before this
        if (State_Thread1 == State_PLAY) {
            // take the interrupt from the dma
            if (1) {  // Simulated_DMA() == 1) {
                // MUTEX ON INPUT BUFFER
                osMutexWait(inputbuf_Handle, osWaitForever);
                // store the 40 samples into the input buffer
                // TODO Probably won't be able to cheat this but oh well
                // Move on you've already done that]
                input.next += 40;
                // MUTEX ON WINDOW BUFFER
                osMutexWait(windowbuf_Handle, osWaitForever);
                // map values to window buffer

                osMessagePut(myQueue01Handle, (uint32_t) State_Thread1, 0);

                // TODO Uncomment this, add function back in and change it to using the
                // non global variables
                //				if (populateWindow_avg() != 0) {
                //					safe_printf("%sERROR:%s Could
                // not map input buffer to window buffer\n", ERROR_M,
                // DEFAULT_COLOUR_M);
                //				}

                // MUTEX OFF WINDOW BUFFER
                osMutexRelease(windowbuf_Handle);
                // MUTEX OFF INPUT BUFFER
                osMutexRelease(inputbuf_Handle);
            }

            // draw window buffer - interrupt
            // TODO this may have to be 5hz or something instead of 25hz
        }
        else if (State_Thread1 == State_STOP) {
            // disregard the interrupt given by the dma
            if (1) {  // Simulated_DMA() == 1) {
                osMessagePut(myQueue01Handle, (uint32_t) State_Thread1, 0);
            }
            else {
                // Do something
            }
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
