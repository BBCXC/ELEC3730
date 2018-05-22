//     $Date: 2018-05-10 10:07:15 +1000 (Thu, 10 May 2018) $
// $Revision: 1301 $
//   $Author: Peter $

// Assignment 3 include file

#ifndef ASS_03_INPUT_H_
#define ASS_03_INPUT_H_

#include "Ass-03.h"

typedef struct {
    int width;        //       = 250;
    int height;       //     = 142;
    int position[4];  // = {x_min, x_max, y_min, y_max};

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

} input_t;
input_t input;

extern void inputInit();

#endif /* ASS_03_INPUT_H_ */
