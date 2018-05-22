//     $Date: 2018-05-22 06:24:02 +1000 (Tue, 22 May 2018) $
// $Revision: 1330 $
//   $Author: Peter $

#include "Ass-03.h"

//
// This task can be used as the main pulse rate application as it takes
// input from the front panel.
//
// *** MAKE UPDATES TO THE CODE AS REQUIRED ***
//
// Draw the boxes that make up the buttons and take action when the
// buttons are pressed. See suggested updates for the touch panel task
// that checks for button presses. Can do more in that task.

#define PLAY_BUTTON "play"
#define STOP_BUTTON "stop"
#define SAVE_BUTTON "save"
#define LOAD_BUTTON "load"
#define ZOOM_IN_BUTTON "zoom_in"
#define ZOOM_OUT_BUTTON "zoom_out"
#define RESET_BUTTON "reset"
#define UP_BUTTON "up"
#define DOWN_BUTTON "down"
#define NEW_BUTTON "new"
#define OVERWRITE_BUTTON "overwrite"
#define CANCEL_BUTTON "cancel"

void Ass_03_Task_02(void const* argument) {

    int touch_pos = 100;
    char* button_pressed;
    Coordinate display;

    osSignalWait(1, osWaitForever);
    safe_printf("Hello from Task 2 - Pulse Rate Application (touch screen input)\n");

    while (1) {
    	//safe_printf("Here    2\n");
        if (getfp(&display) == 0) {
            // If the position touched matches some given coordinates
            touch_pos = get_touch_pos(display.x, display.y, Get_Popup_Status());
            if (touch_pos != 100) {
            	//safe_printf("Here    3\n");
            	button_pressed = Get_Button_Name(touch_pos, Get_Popup_Status());
                // Do something on that touch
                safe_printf("Task 2: (touch: %s, %3d,%3d)\n", button_pressed, display.x, display.y);

                // Touched stop
                if (strcmp(button_pressed, STOP_BUTTON) == 0) {
                    Set_State_Thread(0);
                    safe_printf("Set state to stop\n");
                }
                else if (strcmp(button_pressed, PLAY_BUTTON) == 0) {
                    Set_State_Thread(1);
                    safe_printf("Set state to play\n");
                }
                else if (strcmp(button_pressed, SAVE_BUTTON) == 0) {
                    safe_printf("Button pressed SAVE_BUTTON\n");
                    // TODO Call save function - popup window
                    if (Save_Popup() != 0) {
                        safe_printf("%sERROR:%s Save_Popup function failed\n", ERROR_M, DEFAULT_COLOUR_M);
                    }
                }
                else if (strcmp(button_pressed, LOAD_BUTTON) == 0) {
                    safe_printf("Button pressed LOAD_BUTTON\n");
                    // TODO Call Load function
                    if (Load_File() != 0) {
                        safe_printf("%sERROR:%s Load_File function failed\n", ERROR_M, DEFAULT_COLOUR_M);
                    }
                }
                else if (strcmp(button_pressed, ZOOM_IN_BUTTON) == 0) {
                    safe_printf("Button pressed ZOOM_IN_BUTTON\n");
                    // TODO increment zoom_coeff
                }
                else if (strcmp(button_pressed, ZOOM_OUT_BUTTON) == 0) {
                    safe_printf("Button pressed ZOOM_OUT_BUTTON\n");
                    // TODO decrement zoom_coeff
                }
                else if (strcmp(button_pressed, RESET_BUTTON) == 0) {
                    safe_printf("Button pressed RESET_BUTTON\n");
                    // TODO Reset zoom_coeff to default
                }
                else if (strcmp(button_pressed, UP_BUTTON) == 0) {
                    safe_printf("Button pressed UP_BUTTON\n");
                    // TODO Increment up list of items
                }
                else if (strcmp(button_pressed, DOWN_BUTTON) == 0) {
                    safe_printf("Button pressed DOWN_BUTTON\n");
                    // TODO Increment down list of items
                }
                else if (strcmp(button_pressed, NEW_BUTTON) == 0) {
                    safe_printf("Button pressed NEW_BUTTON\n");
                    // TODO call save_new function
                    if (Save_New() != 0) {
                        safe_printf("%sERROR:%s Save_New function failed\n", ERROR_M, DEFAULT_COLOUR_M);
                    }
                }
                else if (strcmp(button_pressed, OVERWRITE_BUTTON) == 0) {
                    safe_printf("Button pressed OVERWRITE_BUTTON\n");
                    // TODO call save_overwrite function
                    if (Save_Overwrite() != 0) {
                        safe_printf("%sERROR:%s Save_Overwrite function failed\n", ERROR_M, DEFAULT_COLOUR_M);
                    }
                }
                else if (strcmp(button_pressed, CANCEL_BUTTON) == 0) {
                    safe_printf("Button pressed CANCEL_BUTTON\n");
                    // TODO clear popup window
                    //      redraw original window
                    if (Clear_Popup() != 0) {
                        safe_printf("%sERROR:%s Clear_Popup function failed\n", ERROR_M, DEFAULT_COLOUR_M);
                    }
                }
                else {
                    safe_printf("Unknown Position Pressed\n");
                }
            }
            else{
            	safe_printf("Touch is %s\n", touch_pos);
            }
        }
    }
}

int Save_Popup() {
    if (Get_Debug() == 1) {
        safe_printf("%sDEBUG INFO:%s Entered Save_Popup()\n", DEBUG_M, DEFAULT_COLOUR_M);
    }
    // Set State to popup
    Set_Popup_Status(1);
    Set_State_Thread(2);
    // Draw rectangle
    // Clear inside rectangle
    // Draw buttons inside
    // Set popup to on
    return 0;
}
int Load_File() {
    if (Get_Debug() == 1) {
        safe_printf("%sDEBUG INFO:%s Entered Load_File()\n", DEBUG_M, DEFAULT_COLOUR_M);
    }
    safe_printf("Unimplemented\n");
    return 0;
}
int Save_New() {
    if (Get_Debug() == 1) {
        safe_printf("%sDEBUG INFO:%s Entered Save_New()\n", DEBUG_M, DEFAULT_COLOUR_M);
    }
    safe_printf("Unimplemented\n");
    return 0;
}
int Save_Overwrite() {
    if (Get_Debug() == 1) {
        safe_printf("%sDEBUG INFO:%s Entered Save_Overwrite()\n", DEBUG_M, DEFAULT_COLOUR_M);
    }
    safe_printf("Unimplemented\n");
    return 0;
}
int Clear_Popup() {
    if (Get_Debug() == 1) {
        safe_printf("%sDEBUG INFO:%s Entered Clear_Popup()\n", DEBUG_M, DEFAULT_COLOUR_M);
    }
    Set_Popup_Status(0);
    Task_4_Init();
    return 0;
}
