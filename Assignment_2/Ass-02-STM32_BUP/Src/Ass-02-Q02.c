//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE 
//

 //Example code
void CalculatorInit(grid_struct *grid_space_p, char **input[])
{
  // STEPIEN: Assume horizontal display

	grid_struct grid_values;

  // Initialize and turn on LCD and calibrate the touch panel
  BSP_LCD_Init();
  BSP_LCD_DisplayOn();
  BSP_TP_Init();

  // Display welcome message
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  if(calculator_layout(&grid_values, 1, 1) != 0){
	  printf("SYSTEM ERROR : unable to draw layout");
  }

  *grid_space_p = grid_values;


  if(draw_numpad(&grid_values) != 0){
 	  printf("SYSTEM ERROR : unable to draw numpad");
  }

  *input = (char**) calloc(1, sizeof(char*));
  if(*input == 0){
	  printf("ERROR: Calloc input memory");
  }

  *grid_space_p = grid_values;
  printf("exiting init\n");
}

int calculator_layout(grid_struct *grid_space_p, int scaled_width, int scaled_height){

    //Calculate grid spacing based off given width and height
    int num_Vline = 5;
    int num_Hline = 5;
    int debug_sys = 1;	//TODO Figure out what to do about this, global variable???
    grid_struct grid_space;



    int display_height = BSP_LCD_GetYSize();
    int display_width = BSP_LCD_GetXSize();

    //scaled_width = scaled_width - (scaled_width % num_Vline);
    int cell_width = display_width / num_Vline;
    //scaled_height = scaled_height - (scaled_height % num_Hline);
    int cell_height = display_height / num_Hline;

    if(debug_sys == 1) printf("DEBUG_SYS 1: sw %i, sh %i, cew %i, ceh %i\n", scaled_width, scaled_height, cell_width, cell_height);

    //Populate grid_space struct
    int prev_width = display_width;
    int prev_height = display_height;
    int curr_height = prev_height;
    int curr_width = prev_width;
    int temp = 0;

    for(int i=0; i<num_Hline; i++){
    	prev_height = curr_height;
    	curr_height = curr_height - cell_height;
    	curr_width = display_width;
        //if(debug_sys == 1) printf("DEBUG_SYS 2: pw %i, ph %i, cw %i, ch %i\n", prev_width, prev_height, curr_width, curr_height);
        for(int j=0; j<num_Vline; j++){
        	prev_width = curr_width;
            curr_width = prev_width - cell_width;
            //printf("%i, %i, %i\n",i, j, temp);
            grid_space.Area[temp][0] = curr_width;
            grid_space.Area[temp][1] = prev_width;
            grid_space.Area[temp][2] = curr_height;
            grid_space.Area[temp][3] = prev_height;

            //printf("Should be here %i, %i, %i, %i\n", curr_width, prev_width, curr_height, prev_height);
            //if(debug_sys == 1) printf("DEBUG_SYS 3: pw %i, ph %i, cw %i, ch %i\n", prev_width, prev_height, curr_width, curr_height);
            temp = temp+1;

        }
    }
    *grid_space_p = grid_space;

    //Draw grid
    uint16_t x_pos = 0;
    uint16_t y_pos = 0;
    uint16_t len = 0;

    //y_pos is fixed for vertical lines
    y_pos = cell_height;// - scaled_height;

    //Length is the cell spacing for the horizontal lines * the number of horizontal lines
    len = num_Hline * cell_height - cell_height;

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    //For each vertical line calculate the x position and draw it
    for(int l=0; l<num_Vline; l++){
    	if(l > 3){
    		y_pos = 0;
    		len = num_Hline * cell_height;
    	}
        x_pos = l * cell_width;// + display_width;// - scaled_width;
        BSP_LCD_DrawVLine(x_pos, y_pos, len);
        if(debug_sys == 1) printf("DEBUG_SYS Vline: x %i, y %i, len %i\n", x_pos, y_pos, len);
    }

    //x_pos is fixed for horizontal lines
    x_pos = display_width;// - scaled_width;

    //Length is the cell spacing for the vertical lines * the number of vertical lines
    len = display_width;//num_Vline * cell_width;

    //For each horizontal line calculate the y position and draw it
    for(int l=0; l<num_Hline; l++){
        y_pos = l * cell_height;// + display_height - scaled_height;
        BSP_LCD_DrawHLine(0, y_pos, len);
        if(debug_sys == 1) printf("DEBUG_SYS Hline: x %i, y %i, len %i\n", x_pos, y_pos, len);
    }

    return 0;
}


int draw_numpad(grid_struct *grid_space_p){

	int debug_sys = 1;	//TODO Figure out what to do about this, global variable???

    //Draw numbers 0 - 9 in the relevant positions
    //Draw math symbols on first screen + - etc
    //Draw common symbols, AC del = etc
    char *items[21] = {"=", "+", "ANS", ".", "0", ">", "-", "3", "2", "1",
        			   "<", "/", "6", "5", "4", "AC", "x", "9", "8", " 7", "DEL"};

    for(int i=0; i<21; i++){
		if(draw_item(grid_space_p, items[i], i) == 0){
			grid_space_p->Area[i][4] = i - 0;
			if(debug_sys == 1) printf("DEBUG_SYS numpad: item %s, cell_number %i, grid_space %i\n", items[i], i, grid_space_p->Area[i][4]);

		}
    }
    return 0;
}

int draw_sym(grid_struct *grid_space_p){

	int debug_sys = 1;	//TODO Figure out what to do about this, global variable???

    //Draw symbol screen (, ), % etc
    //Draw common symbols, AC del = etc

    char *items[21] = {"=", "(", "ANS", "sqrt", "^", ">", ")", "atan", "acos", "asin",
			           "<", "", "tan", "cos", "sin", "AC", "", "log", "ln", "exp", "DEL"};

    for(int i=0; i<21; i++){
    	if(draw_item(grid_space_p, items[i], i) == 0){
    		grid_space_p->Area[i][4] = i + 21;
    		if(debug_sys == 1) printf("DEBUG_SYS: item %s, cell_number %i, grid_space %i\n", items[i], i, grid_space_p->Area[i][4]);

    	}
   }
   return 0;
}


int draw_item(grid_struct *grid_space_p, char* item, int cell_number){

	int debug_sys = 1;	//TODO Figure out what to do about this, global variable???

    //Draws specific character passed into function

    int x_min = grid_space_p->Area[cell_number][0];
    int x_max = grid_space_p->Area[cell_number][1];
    int y_min = grid_space_p->Area[cell_number][2];
    int y_max = grid_space_p->Area[cell_number][3];

    if(LCD_ClearCell(x_min, x_max, y_min, y_max) != 0) printf("ERROR CLEARING CELL");


//    printf("x_min, %i\n", x_min);
//    printf("x_max, %i\n", x_max);
//    printf("y_min, %i\n", y_min);
//    printf("y_max, %i\n", y_max);

    //Find center of cell given
    int x_pos = ((x_max - x_min) / 2.0) + x_min;
    int y_pos = ((y_max - y_min) / 2.0) + y_min;

    //printf("x_pos, %i\n", x_pos);
	//printf("y_pos, %i\n", y_pos);

	BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(x_pos, y_pos, (uint8_t*)item, CENTER_MODE);

    return 0;
}



int get_touch_pos(grid_struct *grid_space_p, int display_x, int display_y){
  for(int i=0; i<21; i++){
	  //printf("Tried Grid position %i, coordinates %i, %i, %i, %i\n", i, (*grid_space_p).Area[i][0], (*grid_space_p).Area[i][1], (*grid_space_p).Area[i][2], (*grid_space_p).Area[i][3]);
	  if((display_x >= (*grid_space_p).Area[i][0]) && (display_x <= (*grid_space_p).Area[i][1]) && (display_y >= (*grid_space_p).Area[i][2]) && (display_y <= (*grid_space_p).Area[i][3])){
		  //printf("Found Grid position %i, coordinates %i, %i, %i, %i\n", i, (*grid_space_p).Area[i][0], (*grid_space_p).Area[i][1], (*grid_space_p).Area[i][2], (*grid_space_p).Area[i][3]);
		  return ((*grid_space_p).Area[i][4]);
	  }
  }
  return 100;
}

void CalculatorProcess(grid_struct *grid_space_p, char **input[]){

	grid_struct grid_values = *grid_space_p;

	static int button_debounce = 0;
	static int display_mode = 0;
	static double prev_ans = 0;
	static int num_char = 0;

	char *items[42] = {"=", "+", "ANS", ".", "0", ">", "-", "3", "2", "1",
					   "<", "/", "6", "5", "4", "AC", "x", "9", "8", "7", "DEL",
					   "=", "(", "ANS", "sqrt", "^", ">", ")", "atan", "acos", "asin",
   			           "<", "pi", "tan", "cos", "sin", "AC", "", "log", "ln", "exp", "DEL"};

	// getDisplayPoint(&display, Read_Ads7846(), &matrix );
	if (BSP_TP_GetDisplayPoint(&display) == 0 && button_debounce >= 1000){
		//Given grid struct and position touched, returns area touched

		button_debounce = 0;

		int touch_pos = get_touch_pos(&grid_values, display.x, display.y);
		printf("touch_pos %i, position touched %i, %i\n", touch_pos, display.x, display.y);
		if(touch_pos != 100){
			printf("Selected %s\n", items[touch_pos]);

			//Do something with selected item


			if(strcmp(items[touch_pos], "DEL") == 0){
				//Remove previous item from string

				if(num_char > 0){
					num_char--;
					(*input)[num_char] = '\0';
				}
				else{
					printf("Whole string deleted");
				}
			}
			else if(strcmp(items[touch_pos], "AC") == 0){
				//Clear All items
			}
			else if(strcmp(items[touch_pos], "<") == 0){ //strcmp(items[touch_pos], "<") == 0 TODO
				//Switch display mode
				display_mode++;
				if(display_mode > 1){
					display_mode = 0;
				}
				else if(display_mode < 0){
					display_mode = 1;
				}
				if(display_mode == 0){
					if(draw_numpad(&grid_values) != 0){
					 	  printf("SYSTEM ERROR : unable to draw sym");
					}
					*grid_space_p = grid_values;
				}
				else if(display_mode == 1){
					if(draw_sym(&grid_values) != 0){
					 	  printf("SYSTEM ERROR : unable to draw numpad");
					}
					*grid_space_p = grid_values;
				}
			}
			else if(strcmp(items[touch_pos], "=") == 0){
				//Equate equation
				//Call function return answer
				//prev_ans = parseFormula(input);
				//Save answer
			}
			else if(strcmp(items[touch_pos], "ANS") == 0){
				//Insert previous answer into equation as number

				//TODO Make this a function
				//TODO somehow make the double a string
				/*char *item = (char*)prev_ans;
				char* new_string = (char*) calloc((strlen(item) + 1), sizeof(char));
				printf("strlen %i\n", strlen(item));
				strncpy(&new_string[0], item, strlen(item));

				(*input)[num_char] = &new_string[0];

				printf("String copied %s\n", new_string);
				printf("String stored %s\n", ((*input)[num_char]));

				num_char++;*/
			}
			else{
				//Append symbol(s) to string
				printf("Writing String\n");
				//(*input)[num_char] = Input_append(&input, items[touch_pos]);

				//TODO Make this a function
				char *item = items[touch_pos];
				char* new_string = (char*) calloc((strlen(item) + 1), sizeof(char));
				printf("strlen %i\n", strlen(item));
				strncpy(&new_string[0], item, strlen(item));

				(*input)[num_char] = &new_string[0];

				printf("String copied %s\n", new_string);
				printf("String stored %s\n", ((*input)[num_char]));

				num_char++;

			}

		}
	}

	button_debounce++;
}

/*char *Input_append(char *input[], char *item){

	char* new_string = (char*) calloc((strlen(item) + 1), sizeof(char));

	printf("strlen %i\n", strlen(item));
	strncpy(&new_string[0], item, strlen(item));

	printf("String copied %s\n", new_string);

	return new_string;
}*/

int LCD_ClearCell(int x_min, int x_max, int y_min, int y_max){

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	/* Draw a rectangle with background color */
	BSP_LCD_FillRect((x_min + 1), (y_min + 1), (x_max - x_min - 2), (y_max - y_min - 2));

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

	return 0;
}

