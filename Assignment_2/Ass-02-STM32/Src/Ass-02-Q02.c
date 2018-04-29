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

#define MemExpand 10


/***********************************************************************************************************************
************************************************Calculator Initilisation************************************************
***********************************************************************************************************************/
//Initilise the calculator and draw number pad
void CalculatorInit(){
	// Initialize and turn on LCD and calibrate the touch panel
  	BSP_LCD_Init();
  	BSP_LCD_DisplayOn();
  	BSP_TP_Init();

  	//Clear screen
  	BSP_LCD_Clear(LCD_COLOR_WHITE);
  	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

	//Display title animation
	if(title_animation() != 0){
	printf("ERROR : Unable to draw animation");
	}

	//Set basic layout of calculator grid
	if(calculator_layout() != 0){
	  printf("ERROR : Unable to draw layout");
	}

	//Initilise numpad symbol layout
	if(draw_numpad() != 0){
		  printf("ERROR : Unable to draw numpad");
	}

	//If debug information is ON
	if(info.debug == 1){
	//Print grid cell spacings
	for(int tem = 0; tem < 25; tem++){
		printf("DEBUG_INFO: pw %i, ph %i, cw %i, ch %i, pos %i, item %s\n",
				grid_space_p.Area[tem][0],
				grid_space_p.Area[tem][1],
				grid_space_p.Area[tem][2],
				grid_space_p.Area[tem][3],
				grid_space_p.Area[tem][4],
				grid_space_p.items[grid_space_p.Area[tem][4]]);
		}
	}

	//Allocate memory for the input
	if(allocate_memory() != 0) printf("ERROR: Could not allocate memory for EQUATION\n");

	//System information, memory for input
	if(info.system == 1) printf("SYS_INFO: Equation currently has %i memory free of total %i\n", 
								 equation.size - equation.pos, equation.size);
	if(info.debug == 1) printf("DEBUG_INFO: Exiting init\n");
}


//Draws basic grid pattern
int calculator_layout(){
	//Calculate grid spacing based off given width and height
	int num_Vline = 5;
	int num_Hline = 5;

	//LCD X and Y Size
	int display_height = BSP_LCD_GetYSize();
	int display_width = BSP_LCD_GetXSize();

	//Cell width and height for LCD
	int cell_width = display_width / num_Vline;
	int cell_height = display_height / num_Hline;

	if(info.debug == 1) printf("DEBUG_INFO: cew %i, ceh %i\n", cell_width, cell_height);

	//Populate grid_space struct
	int prev_width = display_width;
	int prev_height = display_height;
	int curr_height = prev_height;
	int curr_width = prev_width;
	int temp = 0;

	//Populate stuct with min max, x y coordinates for each button
	for(int i=0; i<num_Hline; i++){
		prev_height = curr_height;
		curr_height = curr_height - cell_height;
		curr_width = display_width;
		if(info.debug == 1) printf("DEBUG_INFO: pw %i, ph %i, cw %i, ch %i\n", 
									prev_width, prev_height, curr_width, curr_height);

		for(int j=0; j<num_Vline; j++){
			prev_width = curr_width;
		  	curr_width = prev_width - cell_width;

			grid_space_p.Area[temp][0] = curr_width;
			grid_space_p.Area[temp][1] = prev_width;
			grid_space_p.Area[temp][2] = curr_height;
			grid_space_p.Area[temp][3] = prev_height;

			if(info.debug == 1) printf("DEBUG_INFO: pw %i, ph %i, cw %i, ch %i\n", 
										prev_width, prev_height, curr_width, curr_height);

			temp = temp + 1;
		}
	}

	//Draw grid based on calculated positions
	uint16_t x_pos = 0;
	uint16_t y_pos = 0;
	uint16_t len = 0;

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

	/***************************************************Vertical Grid****************************************************/
	//y_pos is fixed for vertical lines
	y_pos = cell_height;
	//Length is the cell spacing for the horizontal lines * the number of horizontal lines
	len = num_Hline * cell_height - cell_height;

	//For each vertical line calculate the x position and draw it
	for(int l=0; l<num_Vline; l++){
		if(l > 3){
			y_pos = 0;
			len = num_Hline * cell_height;
		}
		x_pos = l * cell_width;
		BSP_LCD_DrawVLine(x_pos, y_pos, len);
		if(info.debug == 1) printf("DEBUG_INFO Vline: x %i, y %i, len %i\n", x_pos, y_pos, len);
	}

	/**************************************************Horizontal Grid***************************************************/
	//x_pos is fixed for horizontal lines
	x_pos = display_width;
	//Length is the cell spacing for the vertical lines * the number of vertical lines
	len = display_width;

	//For each horizontal line calculate the y position and draw it
	for(int l=0; l<num_Hline; l++){
		y_pos = l * cell_height;
		BSP_LCD_DrawHLine(0, y_pos, len);
		if(info.debug == 1) printf("DEBUG_INFO Hline: x %i, y %i, len %i\n", x_pos, y_pos, len);
	}

	return 0;
}

//Draw numpad layout
int draw_numpad(){
    //Draw numbers 0 - 9 in the relevant positions
    //Draw math symbols on first screen + - etc
    //Draw common symbols, AC del = etc

    for(int i=0; i<21; i++){
  		if(draw_item(i, 0, LCD_COLOR_BLACK, LCD_COLOR_WHITE) == 0){
  			grid_space_p.Area[i][4] = i - 0;
  			if(info.debug == 1) printf("DEBUG_INFO Numpad: item %s, cell_number %i, grid_space %i\n", 
										grid_space_p.items[i], i, grid_space_p.Area[i][4]);
  		}
    }
    return 0;
}

//Draw symbols layout
int draw_sym(){
    //Draw symbol screen (, ), % etc
    //Draw common symbols, AC del = etc

    for(int i=0; i<21; i++){
    	if(draw_item(i, 21, LCD_COLOR_BLACK, LCD_COLOR_WHITE) == 0){
    		grid_space_p.Area[i][4] = i+21;
    		if(info.debug == 1)printf("DEBUG_INFO DRAW_SYM: item %s, cell_number %i, item %i\n",
									   grid_space_p.items[i], i, grid_space_p.Area[i][4]);
    	}
   }
   return 0;
}

//Draw specific item in a specific cell
int draw_item(int cell_number, int offset, int text_colour, int cell_colour){
	//Cell_number : 0 to 21
	//Offset : 0 or 21, depending on symbol or number screen

    //Draws specific character passed into function
    int x_min = grid_space_p.Area[cell_number][0];
    int x_max = grid_space_p.Area[cell_number][1];
    int y_min = grid_space_p.Area[cell_number][2];
    int y_max = grid_space_p.Area[cell_number][3];

    if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, text_colour, cell_colour) != 0){
		printf("ERROR: Could not clear cell\n");
    }
    //Find center of cell given
    int x_pos = ((x_max - x_min) / 2.0) + x_min;
    int y_pos = ((y_max - y_min) / 2.0) + y_min;

  	BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(x_pos, y_pos, (uint8_t*)grid_space_p.items[cell_number + offset], CENTER_MODE);

    return 0;
}

//Return the cell touched on the screen
int get_touch_pos(int display_x, int display_y){
	for(int i=0; i<21; i++){
		if((display_x >= grid_space_p.Area[i][0]) &&
		   (display_x <= grid_space_p.Area[i][1]) &&
		   (display_y >= grid_space_p.Area[i][2]) &&
		   (display_y <= grid_space_p.Area[i][3])){
			return (grid_space_p.Area[i][4]);
		}
	}
	return 100;
}


/*******************************************************************************************
*************************************Calculator Process*************************************
*******************************************************************************************/
//Main calculator process function
void CalculatorProcess(){

	static int button_debounce = 0;
	static int off_debounce = 0;
	static int holding = 0;
	static int display_mode = 0;
	static int button_highlight = 0;
	static int touch_pos = 0;

	char output_ans[50];

	// getDisplayPoint(&display, Read_Ads7846(), &matrix );
	if (BSP_TP_GetDisplayPoint(&display) == 0){
		button_debounce++;

		if(button_debounce >= 50 && holding == 0){
			button_debounce = 0;
			off_debounce = 0;
			holding = 1;

			//Check that my char** isn't out of space
			if(equation.pos >= equation.size){
				//Reallocate memory
				if(reallocate_memory() != 0) printf("ERROR: Unable to reallocate enough memory\n");
			}

			//Given grid struct and position touched, returns area touched
			touch_pos = get_touch_pos(display.x, display.y);
			if(info.debug == 1)printf("DEBUG_INFO: touch_pos %i, position touched %i, %i\n", 
									   touch_pos, display.x, display.y);

			if(touch_pos != 100){

				if(info.system == 1)printf("SYS_INFO: Selected %s\n", grid_space_p.items[touch_pos]);

				//Do something with selected item
				
				if(strcmp(grid_space_p.items[touch_pos], "DEL") == 0){
					//Remove previous item from string
					if(equation.pos > 0){
						if(info.system == 1)printf("SYS_INFO: Formula Contains Before Deleting: %s, 
													equation.pos %i\n", 
													output.formula, equation.pos);
						equation.pos--;
						equation.input[equation.pos] = NULL;
						output.formula[equation.pos] = NULL;
						if(info.system == 1)printf("SYS_INFO: Formula Contains After Deleting: %s, 
													equation.pos %i\n", 
													output.formula, equation.pos);
						if(draw_equation() != 0) printf("ERROR: Could not draw equation\n");
					}
					else{
						if(info.debug == 1)printf("DEBUG_INFO: Whole string deleted\n");
					}
				}

				else if(strcmp(grid_space_p.items[touch_pos], "AC") == 0){
					//Clear All items
					//Clear LCD
					if(clear_equation() != 0) printf("ERROR: Could not clear Equation\n");

					while(equation.pos > 0){
						equation.pos--;
						equation.input[equation.pos] = NULL;
					}
					if(equation.pos == 0) {
						if(info.debug == 1)printf("DEBUG_INFO: Whole string deleted\n");
					}

				}

				else if((strcmp(grid_space_p.items[touch_pos], ">") == 0)){
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
							printf("ERROR: Unable to draw sym");
						}
					}
					else if(display_mode == 1){
						if(draw_sym() != 0){
							printf("ERROR: Unable to draw numpad");
						}
					}
				}

				else if(strcmp(grid_space_p.items[touch_pos], "=") == 0 && equation.pos > 0){
					//Equate equation
					//Call function return answer
					//prev_ans = parseFormula(input);
					//Save answer
					if(info.system == 1){
						for(int num_str=0; num_str < equation.pos; num_str++){
							printf("SYS_INFO: %i String of %i stings: string %s, length %i\n",
									num_str + 1, equation.pos, equation.input[num_str],
									strlen(equation.input[num_str]));
						}
					}

					if(info.system == 1) printf("SYS_INFO: Formula Contains Before Parsing: %s, 
												 equation.pos %i\n", 
												 output.formula, equation.pos);

					if(parseFormula() == 0){
						output.prev_ans = output.result;
						if(info.system == 1) printf("SYS_INFO: Result %lf\n", output.result);				

						//Print answer to screen
						if(clear_equation() != 0) printf("ERROR: Could not clear Equation\n");
						if(draw_result(0) != 0) printf("ERROR: Could not print result\n");
					}
					else{
						output.prev_ans = 0;
						//Print answer to screen
						if(clear_equation() != 0) printf("ERROR: Could not clear Equation\n");
						if(draw_result(1) != 0)	printf("ERROR: Could not print result\n");
					}

					//Clear formula
					//Free output.formula here
					free(output.formula);

					for(int j=0; j<equation.pos; j++){
						free(equation.input[j]);
					}

					while(equation.pos > 0){
						equation.pos--;
						equation.input[equation.pos] = NULL;
					}
					if(equation.pos == 0){
						if(info.debug == 1)printf("DEBUG_INFO: Whole string deleted\n");
					}

					equation.pos = 0;

					if(info.system == 1)printf("SYS_INFO: Previous answer saved as %lf\n", output.prev_ans);
				}

				else if(strcmp(grid_space_p.items[touch_pos], "ANS") == 0){
					//Insert previous answer into equation as number
					if(info.debug == 1)printf("DEBUG_INFO: Writing String %s\n", grid_space_p.items[touch_pos]);
					snprintf(output_ans, 50, "%f", output.prev_ans);


					if(Input_append(output_ans) != 0) printf("ERROR: Could not append string\n");
				}

				else{
					//Append symbol(s) to string
					if(info.debug == 1)printf("DEBUG_INFO: Writing String %s\n", grid_space_p.items[touch_pos]);

					if(Input_append(grid_space_p.items[touch_pos]) != 0) printf("ERROR: Could not append string\n");
				}

				if(button_highlight == 0){
					if(LCD_Cell_Highlight(button_highlight, touch_pos, display_mode) != 0){
						printf("ERROR: Could not highlight cell\n");	//TODO Fix
					}
					button_highlight = 1;
				}

			}
			if(info.debug == 1){
				for(int i=0; i<equation.pos; i++){
					printf("DEBUG_INFO: Equation.Input[%i] Contains %s  length %i\n",
							i, equation.input[i], strlen(equation.input[i]));
				}
			}
			if(info.debug == 1)printf("DEBUG_INFO: Equation currently has %i memory free of total %i\n", 
									   equation.size - equation.pos, equation.size);
		}
		else if(button_debounce >= 50 && holding == 1){
			button_debounce = 0;
		    off_debounce = 0;
		}
	}
	else{
	 	off_debounce++;
		if(off_debounce > 100){
			holding = 0;
		  	button_debounce = 0;
		  	off_debounce = 0;
		  	if(button_highlight == 1 ){
				if(LCD_Cell_Highlight(button_highlight, touch_pos, display_mode) != 0) printf("ERROR: Could not highlight cell\n");	//TODO Fix
			  	button_highlight = 0;
			  	if(info.system == 1)printf("SYS_INFO: Cleared Highlight Now\n");
			}
	  	}
	}
}

//Allocate memory for input string
int allocate_memory(){
  //Calloc enough memory for the input string pointers
  //Allocate arbituary amount of memory for initial max number of strings (10)

	equation.input = (char**) calloc(MemExpand, sizeof(char*));
	if(equation.input == 0){
		printf("ERROR: Calloc input memory");
		return 1;
	}
	equation.size = MemExpand;

	return 0;
}

//Expand previously allocated memory for input string
int reallocate_memory(){
	//Takes previously allocated section,
	//Allocates new sizeof(new_chunk + old_chunk)
	//Shifts old_chunk into new chunck

	for(int i=0; i<equation.pos; i++){
		if(info.debug == 1)printf("DEBUG_INFO: Equation.Input[%i] Before %s length %i\n", 
								   i, equation.input[i], strlen(equation.input[i]));
	}

	equation.input = (char**) realloc(equation.input, (equation.size + MemExpand)*sizeof(char*));
	if(equation.input == 0){
		printf("ERROR: Calloc input memory\n");
		return 1;
	}
	equation.size = equation.size + MemExpand;

	for(int i=0; i<equation.pos; i++){
		//strcpy(equation.input[i], temp[i]);
		if(info.debug == 1)printf("DEBUG_INFO: Equation.Input[%i] After %s  length %i\n", 
								   i, equation.input[i], strlen(equation.input[i]));
	}

	for(int i=equation.pos; i<equation.size; i++){
		equation.input[i] = "\0";
	}

	for(int i=0; i<equation.size; i++){
		//strcpy(equation.input[i], temp[i]);
		if(info.debug == 1)printf("DEBUG_INFO: Equation.Input[%i] After %s  length %i\n", 
								   i, equation.input[i], strlen(equation.input[i]));
	}

	return 0;
}

//Append equation strings into a single string
int Input_append(char *item){

	int num_char = equation.pos;
	char* new_string = (char*) calloc((strlen(item) + 1), sizeof(char));

	if(new_string == 0){
		printf("ERROR: Could not create memory for new_string\n");
	}
	strncpy(&new_string[0], item, strlen(item));

	equation.input[num_char] = &new_string[0];
	if(info.debug == 1){
		printf("DEBUG_INFO: Item expected %s\n", item);
		printf("DEBUG_INFO: String copied %s\n", new_string);
		printf("DEBUG_INFO: String stored %s\n", (equation.input[num_char]));
		printf("DEBUG_INFO: Num_char %i\n", num_char);
	}

	if(strcmp(new_string, (equation.input[num_char])) != 0){
		printf("ERROR: String copied not equal to string stored\n");
	}

	if(equation.pos == 0){
		//TODO Change the allocate memory function to handle this aswell
		output.formula = (char*) calloc(MemExpand, sizeof(char));
		if(output.formula == 0){
			printf("ERROR: Calloc output memory");
			return 1;
		}
		strncpy(output.formula, equation.input[0], strlen(equation.input[0]));
		if(info.debug == 1 || info.system == 1)printf("DEBUG_INFO: formula contains %s\n", output.formula);
	}
	else{
		//Reallocate memory if needed
		strcat(output.formula, equation.input[equation.pos]);
		if(info.debug == 1 || info.system == 1)printf("DEBUG_INFO: formula contains %s\n", output.formula);
	}
	//Call function to display equation on screen
	if(clear_equation() != 0) printf("ERROR: Could not clear equation\n");
	if(draw_equation() != 0) printf("ERROR: Could not draw equation\n");
	equation.pos++;

	return 0;
}

//Colours a specific cell a given colour
int LCD_Cell_Colour(int x_min, int x_max, int y_min, int y_max, int text_colour, int cell_colour){
	BSP_LCD_SetTextColor(cell_colour);
	/* Draw a rectangle with background color */
	BSP_LCD_FillRect((x_min + 1), (y_min + 1), (x_max - x_min - 2), (y_max - y_min - 2));
	BSP_LCD_SetTextColor(text_colour);

	return 0;
}


//TODO Doesn't seem to work at all. Probably just remove for now to simplify
//Highlight a specific cell and redraw the symbol
int LCD_Cell_Highlight(int status, int item_number, int display_mode){
	return 0;
	//Status : on|off
	//Item_number : 0 to 41
	//display_mode : number|symbol

	//Draw coloured cell
	//Redraw symbol
	int offset = display_mode == 0 ? 0 : 21;
	int cell_number = item_number - offset;

	if(cell_number < 0 || cell_number > 20){
		printf("ERROR: Cell_number out of range %i, offset %i\n", cell_number, offset);
	}
	else{

		int x_min = grid_space_p.Area[cell_number][0];
		int x_max = grid_space_p.Area[cell_number][1];
		int y_min = grid_space_p.Area[cell_number][2];
		int y_max = grid_space_p.Area[cell_number][3];

		printf("DEBUG_INFO LCD_HIGHLIGHT: cell_number %i, offset %i\n", cell_number, (display_mode == 0 ? 0 : 21));
		printf("DEBUG_INFO LCD_HIGHLIGHT: x_min %i, x_max %i, y_min %i, y_max %i\n", x_min, x_max, y_min, y_max);

		if(status == 1){
			//Set highlight to off
			HAL_GPIO_TogglePin(GPIOD, LD4_Pin);

			if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, 
			                   LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0){
				printf("ERROR: Could not clear cell\n");
			}
			if(draw_item(cell_number, offset,
						 LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0) printf("ERROR: Could not redraw symbol\n");

			return 0;
		}
		else if (status == 0){
			//Set highlight to on
			HAL_GPIO_TogglePin(GPIOD, LD4_Pin);

			if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, 
			                   LCD_COLOR_BLACK, LCD_COLOR_YELLOW) != 0){
				printf("ERROR: Could not clear cell\n");
			}
			if(draw_item(cell_number, offset,
						 LCD_COLOR_BLACK, LCD_COLOR_YELLOW) != 0) printf("ERROR: Could not redraw symbol\n");
			return 0;
		}
		else{
			printf("ERROR: Unknown status %i, Cell_number %i, offset %i\n", status, cell_number, offset);
		}
	}
	return 1;
}

//Draw the result of the equation on the screen
int draw_result(int status){
	//If status is 0 print result
	//else print syntax error

	//Draws specific character passed into function
	int x_min = grid_space_p.Area[24][0];
	int x_max = grid_space_p.Area[21][1];
	int y_min = grid_space_p.Area[24][2];
	int y_max = grid_space_p.Area[24][3];

	if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0){
		printf("ERROR: Could not clear cell\n");
	}

	//Find center of cell given
	int x_pos = ((x_max - x_min) / 2.0) + x_min;
	int y_pos = ((y_max - y_min) / 2.0) + y_min;

	if(info.debug == 1)printf("DEBUG_INFO: x_min %i, x_max %i, y_min %i, y_max %i, x_pos %i, y_pos %i\n",
							   x_min, x_max, y_min, y_max, x_pos, y_pos);

	char result_str[13];
	if(status == 0)snprintf(result_str, 10, "%f", output.result);
	else result_str = "Syntax Error"

	if(info.debug == 1)printf("DEBUG_INFO: result_str %s\n", result_str);

	BSP_LCD_SetFont(&Font16);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(x_min + 5, y_pos, (uint8_t*)result_str, LEFT_MODE);

	return 0;
}

//Draw the equation on the screen
int draw_equation(){

	//Draws specific character passed into function
	int x_min = grid_space_p.Area[24][0];
	int x_max = grid_space_p.Area[21][1];
	int y_min = grid_space_p.Area[24][2];
	int y_max = grid_space_p.Area[24][3];

	int offset = 0;

	if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0){
		printf("ERROR: Could not clear cell\n");
	}

	//Find center of cell given
	int x_pos = ((x_max - x_min) / 2.0) + x_min;
	int y_pos = ((y_max - y_min) / 2.0) + y_min;

	if(info.debug == 1)printf("DEBUG_INFO: x_min %i, x_max %i, y_min %i, y_max %i, x_pos %i, y_pos %i\n", 
							   x_min, x_max, y_min, y_max, x_pos, y_pos);

	//Roll around the display
	int LCDResultlen = 20;

	if(equation.pos > LCDResultlen){
		offset = equation.pos - LCDResultlen;
	}

	char *temp_equation = "";
	if(equation.pos == 0){
		temp_equation = (char*) calloc(LCDResultlen, sizeof(char));
		if(output.formula == 0){
			printf("ERROR: Calloc output memory");
			return 1;
		}
		temp_equation[0] = output.formula[0];
	}
	else{
		for(int i=offset; i<equation.pos; i++){
			temp_equation[i-offset] = output.formula[i];
		}
	}

	char *temp = &output.formula[offset];

	BSP_LCD_SetFont(&Font16);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(x_min + 5, y_pos, (uint8_t*)temp, LEFT_MODE);

	return 0;
}

//Clear the equation from the screen
int clear_equation(){

	//Draws specific character passed into function
	int x_min = grid_space_p.Area[24][0];
	int x_max = grid_space_p.Area[21][1];
	int y_min = grid_space_p.Area[24][2];
	int y_max = grid_space_p.Area[24][3];

	if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0){
		printf("ERROR: Could not clear cell\n");
	}

	return 0;
}
