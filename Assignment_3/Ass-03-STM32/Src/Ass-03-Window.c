#include "Ass-03-Window.h"
#include "Ass-03.h"

void windowInit() {
   //  Window_buffer[window.width][2];// = () calloc();
	window.zoom_coeff = 1;
    window.buflen = Max_Samples / window.zoom_coeff / window.width;
    window.width = 250;        //       = 250;
    window.height = 142;       //     = 142;
    window.position[4];  // = {x_min, x_max, y_min, y_max};

    window.bg_colour = LCD_COLOR_WHITE;
    window.line_colour = LCD_COLOR_BLACK;
    window.grid_colour = LCD_COLOR_GRAY;
    window.zoom_coeff = 1;  // = 1;  // Number between 1 and MAX_ZOOM
    window.buflen = 40;      // Number of input values that are mapped to the
                         // window buffer

    window.auto_scale = 1023;  // =  // Holds the maximum value that the window buffer has on the
                         // screen

    window.next = 0;  // Holds the position in the window buffer array
                   // that is the latest filled
}

