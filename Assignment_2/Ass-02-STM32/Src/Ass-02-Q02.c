//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

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

#define debugsys 0

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

  //Set basic layout
  if(calculator_layout() != 0){
	  printf("SYSTEM ERROR : unable to draw layout");
  }

  //Initilise numpab loyout
  if(draw_numpad() != 0){
 	  printf("SYSTEM ERROR : unable to draw numpad");
  }

  //Calloc enough memory for the input string pointers
  grid_space_p.input = (char**) calloc(1, sizeof(char*));
  if(grid_space_p.input == 0){
	  printf("ERROR: Calloc input memory");
  }

  printf("exiting init\n");
}


int calculator_layout(){

  //Calculate grid spacing based off given width and height
  int num_Vline = 5;
  int num_Hline = 5;

  int display_height = BSP_LCD_GetYSize();
  int display_width = BSP_LCD_GetXSize();

  int cell_width = display_width / num_Vline;
  int cell_height = display_height / num_Hline;

  if(debugsys == 1) printf("DEBUGSYS 1: cew %i, ceh %i\n", cell_width, cell_height);

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

    if(debugsys == 1) printf("DEBUGSYS 2: pw %i, ph %i, cw %i, ch %i\n", prev_width, prev_height, curr_width, curr_height);

    for(int j=0; j<num_Vline; j++){
     	prev_width = curr_width;
      curr_width = prev_width - cell_width;

      grid_space_p.Area[temp][0] = curr_width;
      grid_space_p.Area[temp][1] = prev_width;
      grid_space_p.Area[temp][2] = curr_height;
      grid_space_p.Area[temp][3] = prev_height;

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
			grid_space_p.Area[i][4] = i - 0;
			if(debugsys == 1) printf("DEBUGSYS numpad: item %s, cell_number %i, grid_space %i\n", grid_space_p.items[i], i, grid_space_p.Area[i][4]);

		}
  }
  return 0;
}


int draw_sym(){

  //Draw symbol screen (, ), % etc
  //Draw common symbols, AC del = etc

  for(int i=0; i<21; i++){
  	if(draw_item(i, 21) == 0){
  		grid_space_p.Area[i][4] = i+21;
  		if(debugsys == 1) printf("DEBUGSYS: item %s, cell_number %i, grid_space %i\n", grid_space_p.items[i], i, grid_space_p.Area[i][4]);

  	}
  }
  return 0;
}


int draw_item(int cell_number, int offset){
    
  //Draws specific character passed into function
  int x_min = grid_space_p.Area[cell_number][0];
  int x_max = grid_space_p.Area[cell_number][1];
  int y_min = grid_space_p.Area[cell_number][2];
  int y_max = grid_space_p.Area[cell_number][3];

  if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0) printf("ERROR CLEARING CELL\n");

  //Find center of cell given
  int x_pos = ((x_max - x_min) / 2.0) + x_min;
  int y_pos = ((y_max - y_min) / 2.0) + y_min;

	BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(x_pos, y_pos, (uint8_t*)grid_space_p.items[cell_number + offset], CENTER_MODE);

  return 0;
}


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
void CalculatorProcess(){

	static int button_debounce = 0;
	static int display_mode = 0;
	static double prev_ans = 0;
	int num_char = grid_space_p.num_char;

	int button_highlight = 0;
	static int touch_pos = 0;

  char output[50];

	if (BSP_TP_GetDisplayPoint(&display) == 0 && button_debounce >= 1000 && button_highlight == 0){
		//Given grid struct and position touched, returns area touched

		button_debounce = 0;

		touch_pos = get_touch_pos(display.x, display.y);
		printf("touch_pos %i, position touched %i, %i\n", touch_pos, display.x, display.y);
		if(touch_pos != 100){

			printf("Selected %s\n", grid_space_p.items[touch_pos]);

      if(LCD_Cell_Highlight(button_highlight, touch_pos) != 0) printf("ERROR: Could not highlight cell\n");

      else button_highlight = 1;

			//Do something with selected item
			if(strcmp(grid_space_p.items[touch_pos], "DEL") == 0){
				//Remove previous item from string

				if(num_char > 0){
					grid_space_p.num_char--;
					grid_space_p.input[num_char] = '\0';
				}
				else{
					printf("Whole string deleted");
				}
			}

			else if(strcmp(grid_space_p.items[touch_pos], "AC") == 0){
				//Clear All items

				//Clear LCD
				if(clear_equation() != 0) printf("ERROR: Could not clear Equation\n");

        for(int i=0; i<grid_space_p.num_char; i++){
				  free(grid_space_p.input[0]);
        }

				grid_space_p.input = (char**) realloc(1, sizeof(char*));
				if(grid_space_p.input == 0){
					printf("ERROR: Realloc input memory");
				}
				grid_space_p.num_char = 0;
			}

			else if(strcmp(grid_space_p.items[touch_pos], "<") == 0){ //strcmp(items[touch_pos], "<") == 0 TODO
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

			else if(strcmp(grid_space_p.items[touch_pos], "=") == 0){
				//Equate equation
				//Call function return answer
				//prev_ans = parseFormula(input);
				//Save answer


				for(int num_str=0; num_str < num_char; num_str++){
					printf("%i String of %i stings: string %s, length %i\n", num_str + 1, num_char, grid_space_p.input[num_str], strlen(grid_space_p.input[num_str]));
				}

				grid_space_p.formula = grid_space_p.input[0];
				for(int len=1; len<(grid_space_p.num_char); len++){
					strcat(grid_space_p.formula, grid_space_p.input[len]);
				}

				printf("Formula Expected: %s\n", grid_space_p.input[0]);
				printf("Formula Passed: %s\n", grid_space_p.formula);
				grid_space_p.result = parseFormula(); //Lowest precedent
				printf("Result %lf\n", grid_space_p.result);

				//Now do something here
				//Print answer to screen etc
				if(clear_equation() != 0) printf("ERROR: Could not clear Equation\n");

				if(draw_result() != 0){
					printf("ERROR: Could not print result\n");
				}

				for(int i=0; i<grid_space_p.num_char; i++){
          free(grid_space_p.input[0]);
        }

				grid_space_p.input = (char**) realloc(1, sizeof(char*));
				if(grid_space_p.input == 0){
					printf("ERROR: Calloc input memory");
				}

				grid_space_p.num_char = 0;
			}

			else if(strcmp(grid_space_p.items[touch_pos], "ANS") == 0){
				//Insert previous answer into equation as number
				//TODO somehow make the double a string
				printf("Writing String %s\n", grid_space_p.items[touch_pos]);
				snprintf(output, 50, "%f", prev_ans);

        if(Input_append(output) != 0) printf("ERROR\n");
			}

			else{
				//Append symbol(s) to string
				printf("Writing String %s\n", grid_space_p.items[touch_pos]);

				if(Input_append(grid_space_p.items[touch_pos]) != 0) printf("ERROR\n");
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

  int num_char = grid_space_p.num_char;
  char* new_string = (char*) calloc((strlen(item) + 1), sizeof(char));
  printf("strlen %i\n", strlen(item));
  strncpy(&new_string[0], item, strlen(item));

  grid_space_p.input = (char**) realloc(num_char*sizeof(char*));
  if(grid_space_p.input == 0){
    printf("ERROR: Calloc input memory");
  }

  grid_space_p.input[num_char] = &new_string[0];
  printf("Item expected %s\n", item);
  printf("String copied %s\n", new_string);
  printf("String stored %s\n", (grid_space_p.input[num_char]));
  printf("num_char %i\n", num_char);

  grid_space_p.num_char++;

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

  int x_min = grid_space_p.Area[cell_number][0];
  int x_max = grid_space_p.Area[cell_number][1];
  int y_min = grid_space_p.Area[cell_number][2];
  int y_max = grid_space_p.Area[cell_number][3];
  int item_num = grid_space_p.Area[cell_number][4];

  if(status == 0){
    //Set highlight to off
    if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0) printf("ERROR CLEARING CELL\n");
    if(draw_item(item_num, 0) != 0) printf("ERROR: Could not redraw symbol\n");

    return 0;
  }
  else if (status == 1){
    //Set highlight to on
    if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_YELLOW) != 0) printf("ERROR CLEARING CELL\n");
    if(draw_item(item_num, 0) != 0) printf("ERROR: Could not redraw symbol\n");

    return 0;
  }

  return 1;
}

int draw_result(){

  //Draws specific character passed into function
  int x_min = grid_space_p.Area[24][0];
  int x_max = grid_space_p.Area[21][1];
  int y_min = grid_space_p.Area[24][2];
  int y_max = grid_space_p.Area[24][3];

  if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0) printf("ERROR CLEARING CELL\n");

  //Find center of cell given
  int x_pos = ((x_max - x_min) / 2.0) + x_min;
  int y_pos = ((y_max - y_min) / 2.0) + y_min;

  printf("x_min %i, x_max %i, y_min %i, y_max %i, x_pos %i, y_pos %i\n", x_min, x_max, y_min, y_max, x_pos, y_pos);

  char *result_str[10];
  snprintf(result_str, 10, "%f", grid_space_p.result);

  printf("result_str %s\n", result_str);

	BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(x_min + 5, y_pos, (uint8_t*)result_str, LEFT_MODE);

  return 0;
}

int draw_equation(int cell_number, int offset){

  //Draws specific character passed into function
  int x_min = grid_space_p.Area[cell_number][0];
  int x_max = grid_space_p.Area[cell_number][1];
  int y_min = grid_space_p.Area[cell_number][2];
  int y_max = grid_space_p.Area[cell_number][3];

  if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0) printf("ERROR CLEARING CELL\n");

  //Find center of cell given
  int x_pos = ((x_max - x_min) / 2.0) + x_min;
  int y_pos = ((y_max - y_min) / 2.0) + y_min;

	BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(x_pos, y_pos, (uint8_t*)grid_space_p.items[cell_number + offset], CENTER_MODE);

  return 0;
}

int clear_equation(){

	//Draws specific character passed into function
	int x_min = grid_space_p.Area[24][0];
	int x_max = grid_space_p.Area[21][1];
	int y_min = grid_space_p.Area[24][2];
	int y_max = grid_space_p.Area[24][3];

  if(LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK, LCD_COLOR_WHITE) != 0) printf("ERROR CLEARING CELL\n");

  return 0;
}

#include "Syntax_tree.h"

double parseFormula(){
  grid_space_p.result = parseSub();
  if(*grid_space_p.input == '\0'){
    return grid_space_p.result;
  }
  // printf("Expected end of grid_space_p.input but found %c\n", *grid_space_p.input);
  printf("Syntax Error\n");
  return 0;
}

double parseSub(){
  double sub_1 = parseSum();
  while(*grid_space_p.input == '-'){
    ++grid_space_p.input;
    double sub_2 = parseSum();
    sub_1 = sub_1 - sub_2;
  }
  return sub_1;
}

double parseSum(){
  double sum_1 = parsePro();
  while(*grid_space_p.input == '+'){
    ++grid_space_p.input;
    double sum_2 = parsePro();
    sum_1 = sum_1 + sum_2;
  }
  return sum_1;
}

double parsePro(){
  double pro_1 = parseDiv();
  while(*grid_space_p.input == '*'){
    ++grid_space_p.input;
    double pro_2 = parseDiv();
    pro_1 = pro_1 * pro_2;
  }
  return pro_1;
}

double parseDiv(){
  double div_1 = parsePow();
  while(*grid_space_p.input == '/'){
    ++grid_space_p.input;
    double div_2 = parsePow();
    div_1 = div_1 / div_2;
  }
  return div_1;
}

double parsePow(){
  double pow_1 = parseFactor();
  while(*grid_space_p.input == '^'){
    ++grid_space_p.input;
    double pow_2 = parseFactor();
    pow_1 = pow(pow_1, pow_2);
  }
  return pow_1;
}

double parseFactor(){

  if(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
    return parseNumber();
  }
  else if(*grid_space_p.input == '-'){
    return parseNumber();
  }
  else if(*grid_space_p.input == '('){
    ++grid_space_p.input;
    double temp = parseSub();
    ++grid_space_p.input;
    return temp;
  }
  //PI
  else if(*grid_space_p.input == 'p'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 'i'){
      ++grid_space_p.input;
      return M_PI;
    }
  }
  //sin sqrt
  else if(*grid_space_p.input == 's'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 'i'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'n'){
        ++grid_space_p.input;
        if(*grid_space_p.input == '('){
          ++grid_space_p.input;
          double temp = parseSub();
          temp = sin(temp*M_PI/180);
          ++grid_space_p.input;
          return temp;
        }

      }
    }
    else if(*grid_space_p.input == 'q'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'r'){
        ++grid_space_p.input;
        if(*grid_space_p.input == 't'){
          ++grid_space_p.input;
          if(*grid_space_p.input == '('){
            ++grid_space_p.input;
            double temp = parseSub();
            temp = sqrt(temp);
            ++grid_space_p.input;
            return temp;
          }

        }
      }
    }
  }
  //cos
  else if(*grid_space_p.input == 'c'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 'o'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 's'){
        ++grid_space_p.input;
        if(*grid_space_p.input == '('){
          ++grid_space_p.input;
          double temp = parseSub();
          temp = cos(temp*M_PI/180);
          ++grid_space_p.input;
          return temp;
        }

      }
    }
  }
  //tan
  else if(*grid_space_p.input == 't'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 'a'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'n'){
        ++grid_space_p.input;
        if(*grid_space_p.input == '('){
          ++grid_space_p.input;
          double temp = parseSub();
          temp = tan(temp*M_PI/180);
          ++grid_space_p.input;
          return temp;
        }

      }
    }
  }
  //asin acos atan
  else if(*grid_space_p.input == 'a'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 's'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'i'){
        ++grid_space_p.input;
        if(*grid_space_p.input == 'n'){
          ++grid_space_p.input;
          if(*grid_space_p.input == '('){
            ++grid_space_p.input;
            double temp = parseSub();
            temp = asin(temp*M_PI/180);
            ++grid_space_p.input;
            return temp;
          }

        }
      }
    }
    else if(*grid_space_p.input == 'c'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'o'){
        ++grid_space_p.input;
        if(*grid_space_p.input == 's'){
          ++grid_space_p.input;
          if(*grid_space_p.input == '('){
            ++grid_space_p.input;
            double temp = parseSub();
            temp = acos(temp*M_PI/180);
            ++grid_space_p.input;
            return temp;
          }

        }
      }
    }
    else if(*grid_space_p.input == 't'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'a'){
        ++grid_space_p.input;
        if(*grid_space_p.input == 'n'){
          ++grid_space_p.input;
          if(*grid_space_p.input == '('){
            ++grid_space_p.input;
            double temp = parseSub();
            temp = atan(temp*M_PI/180);
            ++grid_space_p.input;
            return temp;
          }

        }
      }
    }
  }
  //exp
  else if(*grid_space_p.input == 'e'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 'x'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'p'){
        ++grid_space_p.input;
        if(*grid_space_p.input == '('){
          ++grid_space_p.input;
          double temp = parseSub();
          temp = exp(temp);
          ++grid_space_p.input;
          return temp;
        }

      }
    }
  }
  //ln log10
  else if(*grid_space_p.input == 'l'){
    ++grid_space_p.input;
    if(*grid_space_p.input == 'n'){
      ++grid_space_p.input;
      if(*grid_space_p.input == '('){
        ++grid_space_p.input;
        double temp = parseSub();
        temp = log(temp);
        ++grid_space_p.input;
        return temp;
      }
    }
    else if(*grid_space_p.input == 'o'){
      ++grid_space_p.input;
      if(*grid_space_p.input == 'g'){
        ++grid_space_p.input;
        if(*grid_space_p.input == '('){
          ++grid_space_p.input;
          double temp = parseSub();
          temp = log10(temp);
          ++grid_space_p.input;
          return temp;
        }

      }
    }
  }
  else{
    printf("Syntax Error\n");
    printf("Unknown symbol %c", *grid_space_p.input);
  }
  return 0;
}

double parseNumber(){

  double number = 0;
  int neg_flag = 1;
  //TODO check the first character is a number or a minus
  if(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
  }
  else if(*grid_space_p.input == '-'){
    neg_flag = -1;
    ++grid_space_p.input;
  }
  else{
    printf("Syntax Error");
  }


  while(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
    number = number * 10;
    number = number + *grid_space_p.input - '0';
    ++grid_space_p.input;
  }

  if(*grid_space_p.input == '.'){
    ++grid_space_p.input;

    //Check the next character is a number, else error

    if(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
      double weight = 1;
      while(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
        weight = weight / 10.0;
        double scaled = (*grid_space_p.input - '0') * weight;
        number = number + scaled;
        ++grid_space_p.input;
      }
    }
    else{
      printf("Syntax Error");
    }
  }

  return (number * neg_flag);
}