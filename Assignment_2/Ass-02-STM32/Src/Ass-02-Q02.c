#include "Ass-02.h"

/*******************************************************************************************
**********************************Calculator Initilisation**********************************
*******************************************************************************************/
void CalculatorInit(){
  
  // Initialize and turn on LCD and calibrate the touch panel
  BSP_LCD_Init();
  BSP_LCD_DisplayOn();
  BSP_TP_Init();

  // Display welcome message
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  //TODO Add some sort of animation here
  if(title_animation() != 0) printf("SYSTEM ERROR: Could not display title animation\n");

  //Set basic layout
  if(calculator_layout() != 0){
	  printf("SYSTEM ERROR : unable to draw layout");
  }

  //Initilise numpad layout
  if(draw_numpad() != 0){
 	  printf("SYSTEM ERROR : unable to draw numpad");
  }

  //Calloc enough memory for the input string pointers
  cal_p.input = (char**) calloc(1, sizeof(char*));
  if(cal_p.input == 0){
	  printf("ERROR: Calloc input memory");
  }

  printf("SYS_INFO: Exiting Initilisation\n");
}


int calculator_layout(){

  //Calculate grid spacing based off given number of columns and rows
  int num_Vline = 5;
  int num_Hline = 5;

  //Get size of LCD
  int display_height = BSP_LCD_GetYSize();
  int display_width = BSP_LCD_GetXSize();

  //Divide LCD size into rows and columns 
  int cell_width = display_width / num_Vline;
  int cell_height = display_height / num_Hline;

  if(debugsys == 1) printf("DEBUGSYS 1: cew %i, ceh %i\n", cell_width, cell_height);

  //Populate grid_space struct
  int prev_width = display_width;
  int prev_height = display_height;
  int curr_height = prev_height;
  int curr_width = prev_width;
  int temp = 0;

  //For each row and column 
  //Calculate the min and max cell areas
  for(int i=0; i<num_Hline; i++){
  	prev_height = curr_height;
  	curr_height = curr_height - cell_height;
  	curr_width = display_width;

    if(debugsys == 1) printf("DEBUGSYS 2: pw %i, ph %i, cw %i, ch %i\n", prev_width, prev_height, curr_width, curr_height);

    for(int j=0; j<num_Vline; j++){
      prev_width = curr_width;
      curr_width = prev_width - cell_width;

      //Store min and max coordinates in grid_space array
      cal_p.Area[temp][0] = curr_width;
      cal_p.Area[temp][1] = prev_width;
      cal_p.Area[temp][2] = curr_height;
      cal_p.Area[temp][3] = prev_height;

      if(debugsys == 1) printf("DEBUGSYS 3: pw %i, ph %i, cw %i, ch %i\n", prev_width, prev_height, curr_width, curr_height);
      temp = temp+1;

    }
  }

  //Draw grid
  uint16_t x_pos = 0;
  uint16_t y_pos = 0;
  uint16_t len = 0;

  //y_pos is fixed for vertical lines
  y_pos = cell_height;

  //Length is the cell spacing for the horizontal lines * the number of horizontal lines
  len = num_Hline * cell_height - cell_height;

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  //For each vertical line calculate the x position and draw it
  for(int l=0; l<num_Vline; l++){
  	if(l > 3){
  		y_pos = 0;
  		len = num_Hline * cell_height;
  	}
    x_pos = l * cell_width;
    BSP_LCD_DrawVLine(x_pos, y_pos, len);
    if(debugsys == 1) printf("DEBUGSYS Vline: x %i, y %i, len %i\n", x_pos, y_pos, len);
  }

  //x_pos is fixed for horizontal lines
  x_pos = display_width;

  //Length is the cell spacing for the vertical lines * the number of vertical lines
  len = display_width;

  //For each horizontal line calculate the y position and draw it
  for(int l=0; l<num_Hline; l++){
    y_pos = l * cell_height;
    BSP_LCD_DrawHLine(0, y_pos, len);
    if(debugsys == 1) printf("DEBUGSYS Hline: x %i, y %i, len %i\n", x_pos, y_pos, len);
  }

  return 0;

}


int draw_numpad(){

  //Draw numbers 0 - 9 in the relevant positions
  //Draw math symbols on first screen + - etc
  //Draw common symbols, AC del = etc
  for(int i=0; i<21; i++){
		if(draw_item(i, 0) == 0){
			cal_p.Area[i][4] = i - 0;
			if(debugsys == 1) printf("DEBUGSYS numpad: item %s, cell_number %i, cal_p %i\n", cal_p.items[i], i, cal_p.Area[i][4]);
		}
  }
  return 0;
}


int draw_sym(){

  //Draw symbol screen (, ), % etc
  //Draw common symbols, AC del = etc
  for(int i=0; i<21; i++){
  	if(draw_item(i, 21) == 0){
  		cal_p.Area[i][4] = i+21;
  		if(debugsys == 1) printf("DEBUGSYS: item %s, cell_number %i, cal_p %i\n", cal_p.items[i], i, cal_p.Area[i][4]);

  	}
  }
  return 0;
}


int draw_item(int cell_number, int offset){

  //Draws specific character passed into function
  int x_min = cal_p.Area[cell_number][0];
  int x_max = cal_p.Area[cell_number][1];
  int y_min = cal_p.Area[cell_number][2];
  int y_max = cal_p.Area[cell_number][3];

  if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0) printf("ERROR CLEARING CELL\n");

  //Find center of cell given
  int x_pos = ((x_max - x_min) / 2.0) + x_min;
  int y_pos = ((y_max - y_min) / 2.0) + y_min;

	BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(x_pos, y_pos, (uint8_t*)cal_p.items[cell_number + offset], CENTER_MODE);

  return 0;
}


//Get the position of the screen touch and return the item number that was pessed
int get_touch_pos(int display_x, int display_y){
  for(int i=0; i<21; i++){
	  if((display_x >= cal_p.Area[i][0]) &&
       (display_x <= cal_p.Area[i][1]) && 
       (display_y >= cal_p.Area[i][2]) && 
       (display_y <= cal_p.Area[i][3])){
		  
      return (cal_p.Area[i][4]);
	  }
  }
  return 100;
}


/*******************************************************************************************
*************************************Calculator Process*************************************
*******************************************************************************************/
void CalculatorProcess(){

	static int button_debounce = 0;
	static int display_mode = 0;
	static double prev_ans = 0;
	int num_char = cal_p.num_char;

	int button_highlight = 0;
	static int touch_pos = 0;
	char output[50];

	if (BSP_TP_GetDisplayPoint(&display) == 0 && button_debounce >= 1000 && button_highlight == 0){
		//Given grid struct and position touched, returns area touched

		button_debounce = 0;

		touch_pos = get_touch_pos(display.x, display.y);
		printf("touch_pos %i, position touched %i, %i\n", touch_pos, display.x, display.y);
		if(touch_pos != 100){

			printf("Selected %s\n", cal_p.items[touch_pos]);

      if(LCD_Cell_Highlight(button_highlight, touch_pos) != 0) printf("ERROR: Could not highlight cell\n");

      else button_highlight = 1;

            
      //Do something with selected item
			
      //Selected Item DELETE
      if(strcmp(cal_p.items[touch_pos], "DEL") == 0){
        //Remove previous item from string

  			if(num_char > 0){
  				cal_p.num_char--;
  				cal_p.input[num_char] = '\0';
  			}
  			else{
  				printf("Whole string deleted");
  			}
			}

            //Selected Item AC
			else if(strcmp(cal_p.items[touch_pos], "AC") == 0){
				//Clear All items

				//Clear LCD
				if(clear_equation() != 0) printf("ERROR: Could not clear Equation\n");

        for(int i=0; i<cal_p.num_char; i++){
			    free(cal_p.input[0]);
        }

				cal_p.input = (char**) realloc(cal_p.input, sizeof(char*));
				if(cal_p.input == 0){
					printf("ERROR: Realloc input memory");
				}
				cal_p.num_char = 0;
			}

      //Selected Item Next screen
			else if(strcmp(cal_p.items[touch_pos], "<") == 0){ //strcmp(items[touch_pos], "<") == 0 TODO
				//Switch display mode
				display_mode++;
				if(display_mode > 1){
					display_mode = 0;
				}
				else if(display_mode < 0){
					display_mode = 1;
				}
				if(display_mode == 0){
					if(draw_numpad() != 0){
					 	  printf("SYSTEM ERROR : unable to draw sym");
					}
				}
				else if(display_mode == 1){
					if(draw_sym() != 0){
					 	  printf("SYSTEM ERROR : unable to draw numpad");
					}
				}
			}

      //Selected Item Equals
			else if(strcmp(cal_p.items[touch_pos], "=") == 0){
				for(int num_str=0; num_str < num_char; num_str++){
					printf("%i String of %i stings: string %s, length %i\n", num_str + 1, num_char, cal_p.input[num_str], strlen(cal_p.input[num_str]));
				}

				cal_p.formula = cal_p.input[0];
				for(int len=1; len<(cal_p.num_char); len++){
					strcat(cal_p.formula, cal_p.input[len]);
				}

				printf("Formula Expected: %s\n", cal_p.input[0]);
				printf("Formula Passed: %s\n", cal_p.formula);
				cal_p.result = parseFormula(); //Lowest precedent
				printf("Result %lf\n", cal_p.result);

				//Now do something here
				//Print answer to screen etc
				if(clear_equation() != 0) printf("ERROR: Could not clear Equation\n");

				if(draw_result() != 0){
					printf("ERROR: Could not print result\n");
				}

				for(int i=0; i<cal_p.num_char; i++){
          free(cal_p.input[0]);
        }

				cal_p.input = (char**) realloc(cal_p.input, sizeof(char*));
				if(cal_p.input == 0){
					printf("ERROR: Calloc input memory");
				}

				cal_p.num_char = 0;
			}

      //Selected Item ANSWER
			else if(strcmp(cal_p.items[touch_pos], "ANS") == 0){
				//Insert previous answer into equation as number
				printf("Writing String %s\n", cal_p.items[touch_pos]);
				snprintf(output, 50, "%f", prev_ans);

        if(Input_append(output) != 0) printf("ERROR\n");
			}

      //Selected Item must be a number or symbol
			else{
				//Append symbol(s) to string
				printf("Writing String %s\n", cal_p.items[touch_pos]);

				if(Input_append(cal_p.items[touch_pos]) != 0) printf("ERROR\n");
			}

		}
	}

  //TODO Fix this doesn't work
  else if(BSP_TP_GetDisplayPoint(&display) == 1 && button_highlight == 1){
    if(LCD_Cell_Highlight(button_highlight, touch_pos) != 0) printf("ERROR: Could not highlight cell\n");	//TODO Fix
    else button_highlight = 0;
  }

	button_debounce++;
}


int Input_append(char *item){

  int num_char = cal_p.num_char;
  char* new_string = (char*) calloc((strlen(item) + 1), sizeof(char));
  printf("strlen %i\n", strlen(item));
  strncpy(&new_string[0], item, strlen(item));

  cal_p.input = (char**) realloc(cal_p.input, num_char*sizeof(char*));
  if(cal_p.input == 0){
    printf("ERROR: Calloc input memory\n");
  }

  printf("SYS_INFO: Size of input pointer array %i\n", sizeof(cal_p.input));

  cal_p.input[num_char] = &new_string[0];
  printf("Item expected %s\n", item);
  printf("String copied %s\n", new_string);
  printf("String stored %s\n", (cal_p.input[num_char]));
  printf("num_char %i\n", num_char);


  //Call function to display equation on screen
  //TODO Don't clear, just write the next character
  if(clear_equation() != 0) printf("ERROR: Could not clear equation\n");

  if(draw_equation() != 0) printf("ERROR: Could not draw equation\n");

  cal_p.num_char++;

  return 0;
}


int LCD_Cell_Colour(int x_min, int x_max, int y_min, int y_max, int text_colour, int cell_colour){

	BSP_LCD_SetTextColor(cell_colour);

	/* Draw a rectangle with background color */
	BSP_LCD_FillRect((x_min + 1), (y_min + 1), (x_max - x_min - 2), (y_max - y_min - 2));

	BSP_LCD_SetTextColor(text_colour);

	return 0;
}

//TODO Fix this , doesn't work
int LCD_Cell_Highlight(int status, int cell_number){
  
  //Draw coloured cell
  //Redraw symbol

  int x_min = cal_p.Area[cell_number][0];
  int x_max = cal_p.Area[cell_number][1];
  int y_min = cal_p.Area[cell_number][2];
  int y_max = cal_p.Area[cell_number][3];
  int item_num = cal_p.Area[cell_number][4];

  if(status == 0){
    //Set highlight to off
    if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0) printf("ERROR CLEARING CELL\n");
    if(draw_item(item_num, 0) != 0) printf("ERROR: Could not redraw symbol\n");

    printf("SYS_INFO: Cell highlighted to WHITE text BLACK\n");

    return 0;
  }
  else if (status == 1){
    //Set highlight to on
    if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_YELLOW) != 0) printf("ERROR CLEARING CELL\n");
    if(draw_item(item_num, 0) != 0) printf("ERROR: Could not redraw symbol\n");

    printf("SYS_INFO: Cell highlighted to YELLOW text BLACK\n");

    return 0;
  }

  return 1;
}

int draw_result(){

  //Draws specific character passed into function
  int x_min = cal_p.Area[24][0];
  int x_max = cal_p.Area[21][1];
  int y_min = cal_p.Area[24][2];
  int y_max = cal_p.Area[24][3];

  if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0) printf("ERROR CLEARING CELL\n");

  //Find center of cell given
  int x_pos = ((x_max - x_min) / 2.0) + x_min;
  int y_pos = ((y_max - y_min) / 2.0) + y_min;

  printf("x_min %i, x_max %i, y_min %i, y_max %i, x_pos %i, y_pos %i\n", x_min, x_max, y_min, y_max, x_pos, y_pos);

  char *result_str[10];
  snprintf(*result_str, 10, "%f", cal_p.result);

  printf("result_str %s\n", *result_str);

	BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(x_min + 5, y_pos, (uint8_t*)result_str, LEFT_MODE);

  return 0;
}

int draw_equation(){

  //Draws specific character passed into function
  int x_min = cal_p.Area[24][0];
  int x_max = cal_p.Area[21][1];
  int y_min = cal_p.Area[24][2];
  int y_max = cal_p.Area[24][3];

  int offset = 0;

  if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0) printf("ERROR CLEARING CELL\n");

  //Find center of cell given
  int x_pos = ((x_max - x_min) / 2.0) + x_min;
  int y_pos = ((y_max - y_min) / 2.0) + y_min;

  printf("x_min %i, x_max %i, y_min %i, y_max %i, x_pos %i, y_pos %i\n", x_min, x_max, y_min, y_max, x_pos, y_pos);

  //Roll around the display
  if(cal_p.num_char > LCDResultlen){
    offset = LCDResultlen - cal_p.num_char;
  }
  else{
    offset = 0;
  }
  
  char* equation = cal_p.input[offset];
  for(int len=offset+1; len<(cal_p.num_char); len++){
    strcat(equation, cal_p.input[len]);
  }

  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(x_min + 5, y_pos, (uint8_t*)equation, LEFT_MODE);

  return 0;
}

int clear_equation(){

	//Draws specific character passed into function
	int x_min = cal_p.Area[24][0];
	int x_max = cal_p.Area[21][1];
	int y_min = cal_p.Area[24][2];
	int y_max = cal_p.Area[24][3];

  if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0) printf("ERROR CLEARING CELL\n");

  return 0;
}
