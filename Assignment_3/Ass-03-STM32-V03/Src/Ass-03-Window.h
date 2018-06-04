//     $Date: 2018-05-10 10:07:15 +1000 (Thu, 10 May 2018) $
// $Revision: 1301 $
//   $Author: Peter $

// Assignment 3 include file

#ifndef ASS_03_WINDOW_H_
#define ASS_03_WINDOW_H_

#include "Ass-03.h"

#define Max_Samples 10000
#define MAX_ZOOM 1
#define MIN_ZOOM 1

typedef struct {
    int width;        // 250;
    int height;       // 142;
    int position[4];  // {x_min, x_max, y_min, y_max};

    int bg_colour;
    int line_colour;
    int grid_colour;

    int zoom_coeff;  // = 1;  // Number between 1 and MAX_ZOOM
    int buflen;      // Number of input values that are mapped to the
                     // window buffer

    int auto_scale;  // =  // Holds the maximum value that the window buffer has on the
                     // screen

    int next;  // = 0;  // Holds the position in the window buffer array
               // that is the latest filled

} window_t;
window_t window;

extern void window_init();
extern int Get_Window_Width();
extern int Get_Window_Height();
extern int Get_Window_Buffer(int Position, int Value);
extern void Set_Window_Buffer(int Position, int Value1, int Value2);
extern int Get_Zoom_Coeff_w();
extern void Set_Zoom_Coeff_w(int Value);
extern int Get_Buflen_w();
extern void Set_Buflen_w(int Value);
extern int Get_Auto_Scale_w();
extern void Set_Auto_Scale_w(int Value);
extern int Get_Next_w();
extern void Set_Next_w(int Value);
extern int Get_Colour_BG_w();
extern int Get_Colour_Line_w();
extern int Get_Colour_Grid_w();

#endif /* ASS_03_WINDOW_H_ */
