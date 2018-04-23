#include "Ass-02.h"

//Design some layout parameters
//Scale to actual screen size
//

//Given scaled width and height
//Calculates grid positions and draws grid
void calculator_layout(int scaled_width, int scaled_height){

	grid_struct grid_space;

    //Calculate grid spacing based off given width and height
    int num_Vline = 4;
    int num_Hline = 5;

    int scaled_width = scaled_width - (scaled_width % num_Vline);
    int cell_width = scaled_width / num_Vline;
    int scaled_height = scaled_height - (scaled_height % num_Hline);
    int cell_height = scaled_height / num_Hline;

     if(debug_sys == 1) printf("DEBUG_SYS: sw %i, sh %i, cew %i, ceh %i\n", scaled_width, scaled_height, cell_width, cell_height);

    //Populate grid_space struct
    int prev_width = get_width();
    int prev_height = get_height();
    int curr_height = prev_height;
    int curr_width = prev_width;

    for(int i=0; i<5; i++){
        curr_width = prev_width;
        if(debug_sys == 1) printf("DEBUG_SYS: pw %i, ph %i, cw %i, ch %i\n", prev_width, prev_height, curr_width, curr_height);
        for(int j=0; j<4; j++){
            curr_width = curr_width - cell_width;
            grid_space.Area_[i][j] = (curr_width, prev_width, curr_height, prev_height);
            if(debug_sys == 1) printf("DEBUG_SYS: pw %i, ph %i, cw %i, ch %i\n", prev_width, prev_height, curr_width, curr_height);
        }
        curr_height = curr_height - cell_height;
    }



    //Draw grid
    uint16_t x_pos = 0;
    uint16_t y_pos = 0;
    uint16_t len = 0;

    //y_pos is fixed for vertical lines
    y_pos = get_height() - scaled_height;

    //Length is the cell spacing for the horizontal lines * the number of horizontal lines
    len = num_Hline * cell_height;

    //For each vertical line calculate the x position and draw it
    for(int l=0; l<num_Vline; l++){
        x_pos = num_Vline * cell_width + get_width() - scaled_width;
        BSP_LCD_DrawVLine(x_pos, y_pos, len);
        if(debug_sys == 1) printf("DEBUG_SYS: x %i, y %i, len %i\n", x_pos, y_pos, len);
    }

    //x_pos is fixed for horizontal lines
    x_pos = get_width() - scaled_width;

    //Length is the cell spacing for the vertical lines * the number of vertical lines
    len = num_Vline * cell_width;

    //For each horizontal line calculate the y position and draw it
    for(int l=0; l<num_Hline; l++){
        y_pos = num_Hline * cell_height + get_height() - scaled_height;
        BSP_LCD_DrawHLine(x_pos, y_pos, len);   
        if(debug_sys == 1) printf("DEBUG_SYS: x %i, y %i, len %i\n", x_pos, y_pos, len);
    }
}

int draw_numpad(){

    //Draw numbers 0 - 9 in the relevant positions
    //Draw math symbols on first screen + - etc
    //Draw common symbols, AC del = etc
    char** items = {"0", 0,
                    "1", 1,
                    "2", 2,
                    "3", 3,
                    "4", 4,
                    "5", 5,
                    "6", 6,
                    "7", 7,
                    "8", 8,
                    "9", 9,
                    ".", 10,
                    "=", 11,
                    "<x", 12,
                    "+", 13,
                    "-", 14,
                    "*", 15,
                    "/", 16,
                    "C", 17,
                    "<", 18,
                    ">", 19};

    grid_struct grid_space;

    for(int i, i<(strlen(*item)/2); i++){
        if(draw_item(grid_space, item[2*i], item[2*i+1]) == 0){
            if(debug_sys == 1) printf("DEBUG_SYS: item %s, cell_number %i\n", item[2*i], item[2*i+1]);
        }
    }
    return 1;
}

int draw_sym(){

    //Draw symbol screen (, ), % etc
    //Draw common symbols, AC del = etc

    char** items = {"(", 0,
                    ")", 1,
                    "%", 2,
                    "!", 3,
                    "", 4,
                    "", 5,
                    "", 6,
                    "", 7,
                    "", 8,
                    "", 9,
                    "", 10,
                    "=", 11,
                    "<x", 12,
                    "", 13,
                    "", 14,
                    "", 15,
                    "", 16,
                    "ANS", 17,
                    "<", 18,
                    ">", 19};

    grid_struct grid_space;

    for(int i, i<(strlen(*item)/2); i++){
        if(draw_item(grid_space, item[2*i], item[2*i+1]) == 0){
            if(debug_sys == 1) printf("DEBUG_SYS: item %s, cell_number %i\n", item[2*i], item[2*i+1]);
        }
    }
    return 1;
}

int draw_item(grid_struct grid_space, char* item, int cell_number){
    //Draws specific character passed into function

    int x_min = grid_space.Area_[cell_number][0];
    int x_max = grid_space.Area_[cell_number][1];
    int y_min = grid_space.Area_[cell_number][2];
    int y_max = grid_space.Area_[cell_number][3];

    //Find center of cell given
    int x_pos = ((x_max - x_min) / 2.0) + x_min;
    int y_pos = ((y_max - y_min) / 2.0) + y_min;
    char* string =  *item;

    //BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(x_pos, y_pos, (uint8_t*)string, CENTER_MODE);
    return 1;
}