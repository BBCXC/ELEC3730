#include "Ass-03-Window.h"
#include "Ass-03.h"

void windowInit() {
    //  Window_buffer[window.width][2];// = () calloc();
    window.zoom_coeff  = 1;
    window.buflen      = Max_Samples / window.zoom_coeff / window.width;
    window.width       = 250;  //       = 250;
    window.height      = 142;  //     = 142;
    window.position[0] = 68;   //{68, 318, 2, 144};  // = {x_min, x_max, y_min, y_max};
    window.position[1] = 318;
    window.position[2] = 2;
    window.position[3] = 144;

    window.bg_colour   = LCD_COLOR_WHITE;
    window.line_colour = LCD_COLOR_BLACK;
    window.grid_colour = LCD_COLOR_GRAY;
    window.zoom_coeff  = 1;   // = 1;  // Number between 1 and MAX_ZOOM
    window.buflen      = 40;  // Number of input values that are mapped to the
                              // window buffer

    window.auto_scale = 1023;  // =  // Holds the maximum value that the window buffer has on the
                               // screen

    window.next = 0;  // Holds the position in the window buffer array
                      // that is the latest filled

    static int Window_buffer[window.width][2];  // window.width][2];
}

int Get_Zoom_Coeff_w() {
    osMutexWait(windowbuf_Handle, osWaitForever);
    int temp = window.zoom_coeff;
    osMutexRelease(windowbuf_Handle);
    return temp;
}

void Set_Zoom_Coeff_w(int Value) {
    osMutexWait(windowbuf_Handle, osWaitForever);
    window.zoom_coeff = Value;
    osMutexRelease(windowbuf_Handle);
}

int Get_Buflen_w() {
    osMutexWait(windowbuf_Handle, osWaitForever);
    int temp = window.buflen;
    osMutexRelease(windowbuf_Handle);
    return temp;
}

void Set_Buflen_w(int Value) {
    osMutexWait(windowbuf_Handle, osWaitForever);
    window.buflen = Value;
    osMutexRelease(windowbuf_Handle);
}

int Get_Auto_Scale_w() {
    osMutexWait(windowbuf_Handle, osWaitForever);
    int temp = window.auto_scale;
    osMutexRelease(windowbuf_Handle);
    return temp;
}

void Set_Auto_Scale_w(int Value) {
    osMutexWait(windowbuf_Handle, osWaitForever);
    window.auto_scale = Value;
    osMutexRelease(windowbuf_Handle);
}

int Get_Next_w() {
    osMutexWait(windowbuf_Handle, osWaitForever);
    int temp = window.next;
    osMutexRelease(windowbuf_Handle);
    return temp;
}

void Set_Next_w(int Value) {
    osMutexWait(windowbuf_Handle, osWaitForever);
    window.next = Value;
    osMutexRelease(windowbuf_Handle);
}

int Get_Colour_BG_w() {
    osMutexWait(windowbuf_Handle, osWaitForever);
    int temp = window.bg_colour;
    osMutexRelease(windowbuf_Handle);
    return temp;
}

int Get_Colour_Line_w() {
    osMutexWait(windowbuf_Handle, osWaitForever);
    int temp = window.line_colour;
    osMutexRelease(windowbuf_Handle);
    return temp;
}

int Get_Colour_Grid_w() {
    osMutexWait(windowbuf_Handle, osWaitForever);
    int temp = window.grid_colour;
    osMutexRelease(windowbuf_Handle);
    return temp;
}
