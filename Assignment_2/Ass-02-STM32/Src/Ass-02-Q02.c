/*
 * Author 	   : Mitchell Marotta C3258958
 * 				 Taylor Young C3206230
 * Date	  	   : 4 May 2018
 * Description : Q2
 *				 Calculator interface, used with the touchscreen
 */

/*
LCD_COLOR_BLUE          0x001F
LCD_COLOR_GREEN         0x07E0
LCD_COLOR_RED           0xF800
LCD_COLOR_CYAN          0x07FF
LCD_COLOR_MAGENTA       0xF81F
LCD_COLOR_YELLOW        0xFFE0
LCD_COLOR_LIGHTBLUE     0x841F
LCD_COLOR_LIGHTGREEN    0x87F0
LCD_COLOR_LIGHTRED      0xFC10
LCD_COLOR_LIGHTCYAN     0x87FF
LCD_COLOR_LIGHTMAGENTA  0xFC1F
LCD_COLOR_LIGHTYELLOW   0xFFF0
LCD_COLOR_DARKBLUE      0x0010
LCD_COLOR_DARKGREEN     0x0400
LCD_COLOR_DARKRED       0x8000
LCD_COLOR_DARKCYAN      0x0410
LCD_COLOR_DARKMAGENTA   0x8010
LCD_COLOR_DARKYELLOW    0x8400
LCD_COLOR_WHITE         0xFFFF
LCD_COLOR_LIGHTGRAY     0xD69A
LCD_COLOR_GRAY          0x8410
LCD_COLOR_DARKGRAY      0x4208
LCD_COLOR_BLACK         0x0000
LCD_COLOR_BROWN         0xA145
LCD_COLOR_ORANGE        0xFD20
*/

#include "Ass-02.h"

/***********************************************************************************************************************
************************************************Calculator
*Initilisation************************************************
***********************************************************************************************************************/
// Initilise the calculator and draw number pad
void CalculatorInit() {
  // Initialize and turn on LCD and calibrate the touch panel
  BSP_LCD_Init();
  BSP_LCD_DisplayOn();
  BSP_TP_Init();

  // Clear screen
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  // Display title animation
  if (title_animation() != 0) {
    printf("%sERROR:%s Unable to draw animation", ERROR_M, DEFAULT_COLOUR_M);
  }

  // Set basic layout of calculator grid
  if (calculator_layout() != 0) {
    printf("%sERROR:%s Unable to draw layout", ERROR_M, DEFAULT_COLOUR_M);
  }

  // Initilise numpad symbol layout
  if (draw_numpad() != 0) {
    printf("%sERROR:%s Unable to draw numpad", ERROR_M, DEFAULT_COLOUR_M);
  }

  // If debug information is ON
  if (Get_Debug() == 1) {
    // Print grid cell spacings
    for (int tem = 0; tem < 25; tem++) {
      printf("%sDEBUG_INFO:%s pw %i, ph %i, cw %i, ch %i, pos %i, item %s\n",
             DEBUG_M, DEFAULT_COLOUR_M, Get_Area(tem, 0), Get_Area(tem, 1),
             Get_Area(tem, 2), Get_Area(tem, 3), Get_Area(tem, 4),
             Get_Item(Get_Area(tem, 4)));
    }
  }

  // Allocate memory for the input
  if (allocate_memory() != 0)
    printf("%sERROR:%s Could not allocate memory for EQUATION\n", ERROR_M,
           DEFAULT_COLOUR_M);

  // System information, memory for input
  if (Get_System() == 1)
    printf(
        "%sSYSTEM_INFO:%s Equation currently has %i memory free of total %i\n",
        SYS_M, DEFAULT_COLOUR_M, Get_Size() - Get_Pos(), Get_Size());
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Exiting init\n", DEBUG_M, DEFAULT_COLOUR_M);
}

// Draws basic grid pattern
int calculator_layout() {
  // Calculate grid spacing based off given width and height
  int num_Vline = 5;
  int num_Hline = 5;

  // LCD X and Y Size
  int display_height = BSP_LCD_GetYSize();
  int display_width = BSP_LCD_GetXSize();

  // Cell width and height for LCD
  int cell_width = display_width / num_Vline;
  int cell_height = display_height / num_Hline;

  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s cew %i, ceh %i\n", DEBUG_M, DEFAULT_COLOUR_M,
           cell_width, cell_height);

  // Populate grid_space struct
  int prev_width = display_width;
  int prev_height = display_height;
  int curr_height = prev_height;
  int curr_width = prev_width;
  int temp = 0;

  // Populate stuct with min max, x y coordinates for each button
  for (int i = 0; i < num_Hline; i++) {
    prev_height = curr_height;
    curr_height = curr_height - cell_height;
    curr_width = display_width;
    if (Get_Debug() == 1)
      printf("%sDEBUG_INFO:%s pw %i, ph %i, cw %i, ch %i\n", DEBUG_M,
             DEFAULT_COLOUR_M, prev_width, prev_height, curr_width,
             curr_height);

    for (int j = 0; j < num_Vline; j++) {
      prev_width = curr_width;
      curr_width = prev_width - cell_width;

      // Store parameter calculated in correct position
      Set_Area(temp, 0, curr_width);
      Set_Area(temp, 1, prev_width);
      Set_Area(temp, 2, curr_height);
      Set_Area(temp, 3, prev_height);

      if (Get_Debug() == 1)
        printf("%sDEBUG_INFO:%s pw %i, ph %i, cw %i, ch %i\n", DEBUG_M,
               DEFAULT_COLOUR_M, prev_width, prev_height, curr_width,
               curr_height);

      temp = temp + 1;
    }
  }

  // Draw grid based on calculated positions
  uint16_t x_pos = 0;
  uint16_t y_pos = 0;
  uint16_t len = 0;

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  /**************************************************Vertical
   * Grid***************************************************/
  // y_pos is fixed for vertical lines
  y_pos = cell_height;
  // Length is the cell spacing for the horizontal lines * the number of
  // horizontal lines
  len = num_Hline * cell_height - cell_height;

  // For each vertical line calculate the x position and draw it
  for (int l = 0; l < num_Vline; l++) {
    if (l > 3) {
      y_pos = 0;
      len = num_Hline * cell_height;
    }
    x_pos = l * cell_width;
    BSP_LCD_DrawVLine(x_pos, y_pos, len);
    if (Get_Debug() == 1)
      printf("%sDEBUG_INFO Vline:%s x %i, y %i, len %i\n", DEBUG_M,
             DEFAULT_COLOUR_M, x_pos, y_pos, len);
  }

  /*************************************************Horizontal
   * Grid**************************************************/
  // x_pos is fixed for horizontal lines
  x_pos = display_width;
  // Length is the cell spacing for the vertical lines * the number of vertical
  // lines
  len = display_width;

  // For each horizontal line calculate the y position and draw it
  for (int l = 0; l < num_Hline; l++) {
    y_pos = l * cell_height;
    BSP_LCD_DrawHLine(0, y_pos, len);
    if (Get_Debug() == 1)
      printf("%sDEBUG_INFO Hline:%s x %i, y %i, len %i\n", DEBUG_M,
             DEFAULT_COLOUR_M, x_pos, y_pos, len);
  }
  return 0;
}

// Draw numpad layout
int draw_numpad() {
  // Draw numbers 0 - 9 in the relevant positions
  // Draw math symbols on first screen + - etc
  // Draw common symbols, AC del = etc

  // Loop through each item and draw it in the correct cell, record the item
  // number drawn
  for (int i = 0; i < 21; i++) {
    if (draw_item(i, 0, LCD_COLOR_BLACK, LCD_COLOR_WHITE) == 0) {
      Set_Area(i, 4, i - 0);
      DEBUG_P
      if (Get_Debug() == 1)
        printf(
            "%sDEBUG_INFO Numpad:%s item %s, cell_number %i, grid_space %i\n",
            DEBUG_M, DEFAULT_COLOUR_M, Get_Item(i), i, Get_Area(i, 4));
    } else {
      printf("%sERROR:%s Could not draw item\n", ERROR_M, DEFAULT_COLOUR_M);
    }
  }
  return 0;
}

// Draw symbols layout
int draw_sym() {
  // Draw symbol screen (, ), % etc
  // Draw common symbols, AC del = etc

  // Loop through each item and draw it in the correct cell, record the item
  // number drawn
  for (int i = 0; i < 21; i++) {
    if (draw_item(i, 21, LCD_COLOR_BLACK, LCD_COLOR_WHITE) == 0) {
      Set_Area(i, 4, i + 21);
      if (Get_Debug() == 1)
        printf("%sDEBUG_INFO DRAW_SYM:%s item %s, cell_number %i, item %i\n",
               DEBUG_M, DEFAULT_COLOUR_M, Get_Item(i), i, Get_Area(i, 4));
    }
  }
  return 0;
}

////Draw specific item in a specific cell
// int draw_item(int cell_number, int offset, int text_colour, int cell_colour){
//	//Cell_number : 0 to 21
//	//Offset : 0 or 21, depending on symbol or number screen
//
//    //Draws specific character passed into function
//    int x_min = Get_Area(cell_number, 0);
//    int x_max = Get_Area(cell_number, 1);
//    int y_min = Get_Area(cell_number, 2);
//    int y_max = Get_Area(cell_number, 3);
//
//    if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, text_colour, cell_colour)
//    != 0){
//		printf("%sERROR:%s Could not clear cell\n", ERROR_M,
// DEFAULT_COLOUR_M);
//    }
//    //Find center of cell given
//    int x_pos = ((x_max - x_min) / 2.0) + x_min;
//    int y_pos = ((y_max - y_min) / 2.0) + y_min;
//
//  	BSP_LCD_SetFont(&Font16);
//    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
//    BSP_LCD_DisplayStringAt(x_pos, y_pos, (uint8_t*)Get_Item(cell_number +
//    offset), CENTER_MODE);
//
//    return 0;
//}

// Return the item touched on the screen
// int get_touch_pos(int display_x, int display_y){
//	for(int i=0; i<21; i++){
//		if((display_x >= Get_Area(i, 0)) &&
//		   (display_x <= Get_Area(i, 0)) &&
//		   (display_y >= Get_Area(i, 0)) &&
//		   (display_y <= Get_Area(i, 0))){
//			return (grid_space_p.Area[i][4]);
//		}
//	}
//	printf("Touched Coordinates %i, %i\n", display_x, display_y);
//	return 100;
//}

/*******************************************************************************************
*************************************Calculator
*Process*************************************
*******************************************************************************************/
// Main calculator process function
void CalculatorProcess() {
  static int button_debounce = 0;
  static int off_debounce = 0;
  static int holding = 0;
  static int display_mode = 0;
  static int button_highlight = 0;
  static int touch_pos = 0;

  char output_ans[50];

  // getDisplayPoint(&display, Read_Ads7846(), &matrix );
  if (BSP_TP_GetDisplayPoint(&display) == 0) {
    button_debounce++;

    // If the button has successfully debounced and the user isn't holding the
    // button
    if (button_debounce >= 50 && holding == 0) {
      button_debounce = 0;
      off_debounce = 0;
      holding = 1;

      // Check that my char** isn't out of space
      if (Get_Pos() >= Get_Size()) {
        // Reallocate memory
        if (reallocate_memory() != 0)
          printf("%sERROR:%s Unable to reallocate enough memory\n", ERROR_M,
                 DEFAULT_COLOUR_M);
      }

      // Given grid struct and position touched, returns area touched
      touch_pos = get_touch_pos(display.x, display.y);
      if (Get_Debug() == 1)
        printf("%sDEBUG_INFO:%s touch_pos %i, position touched %i, %i\n",
               DEBUG_M, DEFAULT_COLOUR_M, touch_pos, display.x, display.y);

      if (touch_pos != 100) {
        if (Get_System() == 1)
          printf("%sSYSTEM_INFO:%s Selected %s\n", SYS_M, DEFAULT_COLOUR_M,
                 Get_Item(touch_pos));

        // Do something with selected item
        // If the selected item was delete
        // Delete the previous entry and redraw the equation
        if (strcmp(Get_Item(touch_pos), "DEL") == 0) {
          // Remove previous item from string
          if (Get_Pos() > 0) {
            if (Get_System() == 1)
              printf(
                  "%sSYSTEM_INFO:%s Formula Contains Before Deleting: %s, "
                  "equation.pos %i\n",
                  SYS_M, DEFAULT_COLOUR_M, Get_Formula(), Get_Pos());
            Increment_Pos(-1);
            Set_Input_Str(Get_Pos(), NULL);
            Set_Formula_c(Get_Pos(), NULL);
            if (Get_System() == 1)
              printf(
                  "%sSYSTEM_INFO:%s Formula Contains After Deleting: %s, "
                  "equation.pos %i\n",
                  SYS_M, DEFAULT_COLOUR_M, Get_Formula(), Get_Pos());
            if (draw_equation() != 0)
              printf("%sERROR:%s Could not draw equation\n", ERROR_M,
                     DEFAULT_COLOUR_M);
          } else {
            if (Get_Debug() == 1)
              printf("%sDEBUG_INFO:%s Whole string deleted\n", DEBUG_M,
                     DEFAULT_COLOUR_M);
          }
        }

        // If AC was selected
        // Clear the entire equation
        // Clear the screen
        else if (strcmp(Get_Item(touch_pos), "AC") == 0) {
          // Clear All items
          // Clear LCD
          if (clear_equation() != 0)
            printf("%sERROR:%s Could not clear Equation\n", ERROR_M,
                   DEFAULT_COLOUR_M);
          DEBUG_P
          while (Get_Pos() > 0) {
            Increment_Pos(-1);
            Set_Input_Str(Get_Pos(), NULL);
          }
          if (Get_Pos() == 0) {
            if (Get_Debug() == 1)
              printf("%sDEBUG_INFO:%s Whole string deleted\n", DEBUG_M,
                     DEFAULT_COLOUR_M);
          }

        }

        // If symbol screen selected
        // Note that the display mode has changed
        // Redraw the items to be numpad|symbols
        else if ((strcmp(Get_Item(touch_pos), ">") == 0) ||
                 (strcmp(Get_Item(touch_pos), "<") == 0)) {
          // Switch display mode
          display_mode++;
          if (display_mode > 1) {
            display_mode = 0;
          } else if (display_mode < 0) {
            display_mode = 1;
          }
          if (display_mode == 0) {
            if (draw_numpad() != 0) {
              printf("%sERROR:%s Unable to draw sym", ERROR_M,
                     DEFAULT_COLOUR_M);
            }
          } else if (display_mode == 1) {
            if (draw_sym() != 0) {
              printf("%sERROR:%s Unable to draw numpad", ERROR_M,
                     DEFAULT_COLOUR_M);
            }
          }
        }

        // If the equals was selected
        else if (strcmp(Get_Item(touch_pos), "=") == 0 && Get_Pos() > 0) {
          // Equate equation
          // Call function return answer
          // prev_ans = parseFormula(input);
          // Save answer
          if (Get_System() == 1) {
            for (int num_str = 0; num_str < Get_Pos(); num_str++) {
              printf(
                  "%sSYSTEM_INFO:%s %i String of %i stings: string %s, length "
                  "%i\n",
                  SYS_M, DEFAULT_COLOUR_M, num_str + 1, Get_Pos(),
                  Get_Input_Str(num_str), strlen(Get_Input_Str(num_str)));
            }
          }

          if (Get_System() == 1)
            printf(
                "%sSYSTEM_INFO:%s Formula Contains Before Parsing: %s, "
                "equation.pos %i\n",
                SYS_M, DEFAULT_COLOUR_M, Get_Formula(), Get_Pos());

          if (parseFormula() == 0) {
            Set_Prev_ans(Get_Result());
            if (Get_System() == 1)
              printf("%sSYSTEM_INFO:%s Result %g\n", SYS_M, DEFAULT_COLOUR_M,
                     Get_Result());

            // Print answer to screen
            if (clear_equation() != 0)
              printf("%sERROR:%s Could not clear Equation\n", ERROR_M,
                     DEFAULT_COLOUR_M);
            if (draw_result(0) != 0)
              printf("%sERROR:%s Could not print result\n", ERROR_M,
                     DEFAULT_COLOUR_M);
            DEBUG_P
          } else {
            Set_Prev_ans(0);
            // Print answer to screen
            if (clear_equation() != 0)
              printf("%sERROR:%s Could not clear Equation\n", ERROR_M,
                     DEFAULT_COLOUR_M);
            if (draw_result(1) != 0)
              printf("%sERROR:%s Could not print result\n", ERROR_M,
                     DEFAULT_COLOUR_M);
          }

          // Clear formula
          // Free output.formula here
          //					free(output.formula);
          DEBUG_P
          for (int j = 0; j < Get_Pos(); j++) {
            Free_Input_Str(j);
          }
          DEBUG_P
          while (Get_Pos() > 0) {
            Increment_Pos(-1);
            Set_Input_Str(Get_Pos(), NULL);
          }
          if (Get_Pos() == 0) {
            if (Get_Debug() == 1)
              printf("%sDEBUG_INFO:%s Whole string deleted\n", DEBUG_M,
                     DEFAULT_COLOUR_M);
          }

          Set_Pos(0);

          if (Get_System() == 1)
            printf("%sSYSTEM_INFO:%s Previous answer saved as %lf\n", SYS_M,
                   DEFAULT_COLOUR_M, Get_Prev_ans());

          Set_First_Time(1);
        }

        // If previous answer selected, append the previous answer to the
        // current equation
        else if (strcmp(Get_Item(touch_pos), "ANS") == 0) {
          // Insert previous answer into equation as number
          if (Get_Debug() == 1)
            printf("%sDEBUG_INFO:%s Writing String %s\n", DEBUG_M,
                   DEFAULT_COLOUR_M, Get_Item(touch_pos));
          snprintf(output_ans, 50, "%f", Get_Prev_ans());

          if (Input_append(output_ans) != 0)
            printf("%sERROR:%s Could not append string\n", ERROR_M,
                   DEFAULT_COLOUR_M);
        }

        // Else the calculation solver should be able to handle the input, jsut
        // append it to the string
        else {
          // Append symbol(s) to string
          if (Get_Debug() == 1)
            printf("%sDEBUG_INFO:%s Writing String %s\n", DEBUG_M,
                   DEFAULT_COLOUR_M, Get_Item(touch_pos));

          if (Input_append(Get_Item(touch_pos)) != 0)
            printf("%sERROR:%s Could not append string\n", ERROR_M,
                   DEFAULT_COLOUR_M);
        }

        // A button was pressed, highlight the cell of the button
        if (button_highlight == 0) {
          if (LCD_Cell_Highlight(button_highlight, touch_pos, display_mode) !=
              0) {
            printf("%sERROR:%s Could not highlight cell\n", ERROR_M,
                   DEFAULT_COLOUR_M);
          }
          button_highlight = 1;
        }
      }
      if (Get_Debug() == 1) {
        for (int i = 0; i < Get_Pos(); i++) {
          printf("%sDEBUG_INFO:%s Equation.Input[%i] Contains %s  length %i\n",
                 DEBUG_M, DEFAULT_COLOUR_M, i, Get_Input_Str(i),
                 strlen(Get_Input_Str(i)));
        }
      }
      if (Get_Debug() == 1)
        printf(
            "%sDEBUG_INFO:%s Equation currently has %i memory free of total "
            "%i\n",
            DEBUG_M, DEFAULT_COLOUR_M, Get_Size() - Get_Pos(), Get_Size());
    }
    // If the user is holding the button down, don't perform the action
    else if (button_debounce >= 50 && holding == 1) {
      button_debounce = 0;
      off_debounce = 0;
    }
  }

  // No button pressed, debounce this
  else {
    off_debounce++;
    // User is definately not pressing a button, reset the holding flag
    if (off_debounce > 100) {
      holding = 0;
      button_debounce = 0;
      off_debounce = 0;
      if (button_highlight == 1) {
        if (LCD_Cell_Highlight(button_highlight, touch_pos, display_mode) !=
            0) {
          printf("%sERROR:%s Could not highlight cell\n", ERROR_M,
                 DEFAULT_COLOUR_M);
        }
        button_highlight = 0;
        if (Get_System() == 1)
          printf("%sSYSTEM_INFO:%s Cleared Highlight Now\n", SYS_M,
                 DEFAULT_COLOUR_M);
      }
    }
  }
}

// Colours a specific cell a given colour
int LCD_Cell_Colour(int x_min, int x_max, int y_min, int y_max, int text_colour,
                    int cell_colour) {
  BSP_LCD_SetTextColor(cell_colour);
  /* Draw a rectangle with background color */
  BSP_LCD_FillRect((x_min + 1), (y_min + 1), (x_max - x_min - 2),
                   (y_max - y_min - 2));
  BSP_LCD_SetTextColor(text_colour);

  return 0;
}

// TODO Doesn't seem to work at all. Probably just remove for now to simplify
// Highlight a specific cell and redraw the symbol
int LCD_Cell_Highlight(int status, int item_number, int display_mode) {
  // Status : on|off
  // Item_number : 0 to 41
  // display_mode : number|symbol

  // Draw coloured cell
  // Redraw symbol
  int offset = display_mode == 0 ? 0 : 21;
  int cell_number = item_number - offset;

  if (cell_number < 0 || cell_number > 20) {
    printf("%sERROR:%s Cell_number out of range %i, offset %i\n", ERROR_M,
           DEFAULT_COLOUR_M, cell_number, offset);
  } else {
    int x_min = Get_Area(cell_number, 0);
    int x_max = Get_Area(cell_number, 1);
    int y_min = Get_Area(cell_number, 2);
    int y_max = Get_Area(cell_number, 3);

    printf("%sDEBUG_INFO %sLCD_HIGHLIGHT: cell_number %i, offset %i\n", DEBUG_M,
           DEFAULT_COLOUR_M, cell_number, (display_mode == 0 ? 0 : 21));
    printf(
        "%sDEBUG_INFO %sLCD_HIGHLIGHT: x_min %i, x_max %i, y_min %i, y_max "
        "%i\n",
        DEBUG_M, DEFAULT_COLOUR_M, x_min, x_max, y_min, y_max);

    // Highlight is currently on
    if (status == 1) {
      // Set highlight to off
      HAL_GPIO_TogglePin(GPIOD, LD4_Pin);

      if (LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK,
                          LCD_COLOR_WHITE) != 0) {
        printf("%sERROR:%s Could not clear cell\n", ERROR_M, DEFAULT_COLOUR_M);
      }
      if (draw_item(cell_number, offset, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0)
        printf("%sERROR:%s Could not redraw symbol\n", ERROR_M,
               DEFAULT_COLOUR_M);

      return 0;
    }

    // Highlight is currently off
    else if (status == 0) {
      // Set highlight to on
      HAL_GPIO_TogglePin(GPIOD, LD4_Pin);

      if (LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK,
                          LCD_COLOR_YELLOW) != 0) {
        printf("%sERROR:%s Could not clear cell\n", ERROR_M, DEFAULT_COLOUR_M);
      }
      if (draw_item(cell_number, offset, LCD_COLOR_BLACK, LCD_COLOR_YELLOW) !=
          0)
        printf("%sERROR:%s Could not redraw symbol\n", ERROR_M,
               DEFAULT_COLOUR_M);
      return 0;
    } else {
      printf("%sERROR:%s Unknown status %i, Cell_number %i, offset %i\n",
             ERROR_M, DEFAULT_COLOUR_M, status, cell_number, offset);
    }
  }
  return 1;
}

// Draw the result of the equation on the screen
int draw_result(int status) {
  // If status is 0 print result
  // else print syntax %sERROR%s
  // Draws specific character passed into function
  int x_min = Get_Area(24, 0);
  int x_max = Get_Area(21, 1);
  int y_min = Get_Area(24, 2);
  int y_max = Get_Area(24, 3);

  if (LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK,
                      LCD_COLOR_WHITE) != 0) {
    printf("%sERROR:%s Could not clear cell\n", ERROR_M, DEFAULT_COLOUR_M);
  }

  // Find center of cell given
  int x_pos = ((x_max - x_min) / 2.0) + x_min;
  int y_pos = ((y_max - y_min) / 2.0) + y_min;

  if (Get_Debug() == 1)
    printf(
        "%sDEBUG_INFO:%s x_min %i, x_max %i, y_min %i, y_max %i, x_pos %i, "
        "y_pos %i\n",
        DEBUG_M, DEFAULT_COLOUR_M, x_min, x_max, y_min, y_max, x_pos, y_pos);

  char result_str[13];
  if (status == 0)
    snprintf(result_str, 10, "%g", Get_Result());
  else
    strcpy(result_str, "Syntax Error");

  if (clear_equation() != 0)
    printf("%sERROR:%s Could not clear Equation\n", ERROR_M, DEFAULT_COLOUR_M);

  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s result_str %s\n", DEBUG_M, DEFAULT_COLOUR_M,
           result_str);

  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(x_min + 5, y_pos, (uint8_t*)result_str, LEFT_MODE);
  DEBUG_P
  return 0;
}

// Clear the equation from the screen
int clear_equation() {
  // Draws specific character passed into function
  int x_min = Get_Area(24, 0);
  int x_max = Get_Area(21, 1);
  int y_min = Get_Area(24, 2);
  int y_max = Get_Area(24, 3);
  printf(
      "%sDEBUG_INFO Clearing Screen:%s x_min %i, x_max %i, y_min %i, y_max "
      "%i\n",
      DEBUG_M, DEFAULT_COLOUR_M, x_min, x_max, y_min, y_max);

  if (LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK,
                      LCD_COLOR_WHITE) != 0) {
    printf("%sERROR:%s Could not clear cell\n", ERROR_M, DEFAULT_COLOUR_M);
  }

  return 0;
}
