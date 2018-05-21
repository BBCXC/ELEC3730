#include "Ass-03-Window.h"
#include "Ass-03.h"

void window_init() {

    window.zoom_coeff = 1;
    window.buflen     = Max_Samples / window.zoom_coeff / window.width;
    window.width      = 250;  // TODO Calculate
    window.height     = 142;  // TODO Calculate

    window.position[0] = 68;   // TODO Calculate
    window.position[1] = window.position[0] + window.width;  // TODO Calculate
    window.position[2] = 2;    // TODO Calculate
    window.position[3] = window.position[2] + window.height;  // TODO Calculate

    window.bg_colour   = LCD_COLOR_WHITE;
    window.line_colour = LCD_COLOR_BLACK;
    window.grid_colour = LCD_COLOR_GRAY;
    window.zoom_coeff  = 1;   // Number between 1 and MAX_ZOOM
    window.buflen      = 40;  // Number of input values that are mapped to the window buffer

    window.auto_scale = 1023;  // Holds the maximum value that the window buffer has on the screen

    window.next = 0;  // Holds the position in the window buffer array
                      // that is the latest filled
}

int Get_Window_Buffer(int Position, int Value) {
    osMutexWait(windowbuf_Handle, osWaitForever);
    //int temp = Window_buffer[Position][Value];
    int temp = 0;
    osMutexRelease(windowbuf_Handle);
    return temp;
}

void Set_Window_Buffer(int Position, int Value1, int Value2) {
    osMutexWait(windowbuf_Handle, osWaitForever);
    //Window_buffer[Position][Value1] = Value2;
    osMutexRelease(windowbuf_Handle);
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
