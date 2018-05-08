#include "Ass-02-Equation.h"

void Equation_Init(void) {
  output.prev_ans = 120.04;
  output.result = 24507.064;
  output.pos = 0;
}

int Get_Size(void) { return (equation.size); }

char **Get_Input(void) { return (equation.input); }

char *Get_Input_Str(int position) { return (equation.input[position]); }

int Get_Pos(void) { return (equation.pos); }

void Set_Size(int Value) { equation.size = Value; }

void Set_Input(char **Value) { equation.input = Value; }

void Set_Input_Str(int position, char *Value) {
  equation.input[position] = Value;
}

void Set_Pos(int Value) { equation.pos = Value; }

void Free_Input_Str(int position) { free(equation.input[position]); }

char Get_Formula_c(void) { return (output.formula[output.pos]); }

char Get_Formula_c_pos(int position) { return (output.formula[position]); }

char *Get_Formula(void) { return (output.formula); }

double Get_Result(void) { return (output.result); }

double Get_Prev_ans(void) { return (output.prev_ans); }

void Set_Formula(char *Value) {
  output.formula = Value;
  //printf("Formula Expected %s\n", output.formula);
}

void Set_Formula_c(int position, char Value) {
  output.formula[position] = Value;
  //printf("Formula Expected %s\n", output.formula);
}

void Set_Result(double Value) { output.result = Value; }

void Set_Prev_ans(double Value) { output.prev_ans = Value; }

int Increment_Formula(int Direction) {
  return (output.pos = output.pos + Direction);
}

int Increment_Pos(int Direction) {
  return (equation.pos = equation.pos + Direction);
}

void Reset_Formula_pos(void) { output.pos = 0; }

// Allocate memory for input string
int allocate_memory() {
  // Calloc enough memory for the input string pointers
  // Allocate arbituary amount of memory for initial max number of strings (10)

  equation.input = (char **)calloc(MemExpand, sizeof(char *));
  if (equation.input == 0) {
    printf("%sERROR:%s Calloc input memory\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  equation.size = MemExpand;

  return 0;
}

// Expand previously allocated memory for input string
int reallocate_memory() {
  // Takes previously allocated section,
  // Allocates new sizeof(new_chunk + old_chunk)
  // Shifts old_chunk into new chunck

  for (int i = 0; i < equation.pos; i++) {
    if (Get_Debug() == 1)
      printf("%sDEBUG_INFO:%s Equation.Input[%i] Before %s length %i\n",
             DEBUG_M, DEFAULT_COLOUR_M, i, equation.input[i],
             strlen(equation.input[i]));
  }

  equation.input = (char **)realloc(
      equation.input, (equation.size + MemExpand) * sizeof(char *));
  if (equation.input == 0) {
    printf("%sERROR:%s Calloc input memory\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  equation.size = equation.size + MemExpand;

  for (int i = 0; i < equation.pos; i++) {
    if (Get_Debug() == 1)
      printf("%sDEBUG_INFO:%s Equation.Input[%i] After %s  length %i\n",
             DEBUG_M, DEFAULT_COLOUR_M, i, equation.input[i],
             strlen(equation.input[i]));
  }

  for (int i = equation.pos; i < equation.size; i++) {
    equation.input[i] = "\0";
  }

  for (int i = 0; i < equation.size; i++) {
    if (Get_Debug() == 1)
      printf("%sDEBUG_INFO:%s Equation.Input[%i] After %s  length %i\n",
             DEBUG_M, DEFAULT_COLOUR_M, i, equation.input[i],
             strlen(equation.input[i]));
  }

  return 0;
}

// Append equation strings into a single string
int Input_append(char *item) {
  int num_char = equation.pos;
  char *new_string = (char *)calloc((strlen(item) + 1), sizeof(char));

  if (new_string == 0) {
    printf("%sERROR:%s Could not create memory for new_string\n", ERROR_M,
           DEFAULT_COLOUR_M);
  }
  strncpy(&new_string[0], item, strlen(item));

  equation.input[num_char] = &new_string[0];
  if (Get_Debug() == 1) {
    printf("%sDEBUG_INFO:%s Item expected %s\n", DEBUG_M, DEFAULT_COLOUR_M,
           item);
    printf("%sDEBUG_INFO:%s String copied %s\n", DEBUG_M, DEFAULT_COLOUR_M,
           new_string);
    printf("%sDEBUG_INFO:%s String stored %s\n", DEBUG_M, DEFAULT_COLOUR_M,
           (equation.input[num_char]));
    printf("%sDEBUG_INFO:%s Num_char %i\n", DEBUG_M, DEFAULT_COLOUR_M,
           num_char);
  }

  if (strcmp(new_string, (equation.input[num_char])) != 0) {
    printf("%sERROR:%s String copied not equal to string stored\n", ERROR_M,
           DEFAULT_COLOUR_M);
  }

  // Check that memory has been allocated
  if (equation.pos == 0) {
    output.formula = (char *)calloc(MemExpand, sizeof(char));
    if (output.formula == 0) {
      printf("%sERROR:%s Calloc output memory\n", ERROR_M, DEFAULT_COLOUR_M);
      return 1;
    }
    strncpy(output.formula, equation.input[0], strlen(equation.input[0]));
    if (Get_Debug() == 1 || Get_System() == 1)
      printf("%sDEBUG_INFO:%s formula contains %s\n", DEBUG_M, DEFAULT_COLOUR_M,
             output.formula);
  }

  else {
    // Reallocate memory if needed
    strcat(output.formula, equation.input[equation.pos]);
    if (Get_Debug() == 1 || Get_System() == 1)
      printf("%sDEBUG_INFO:%s formula contains %s\n", DEBUG_M, DEFAULT_COLOUR_M,
             output.formula);
  }
  // Call function to display equation on screen
// TODO if (clear_equation() != 0)
//    printf("%sERROR:%s Could not clear equation\n", ERROR_M, DEFAULT_COLOUR_M);
  if (draw_equation() != 0)
    printf("%sERROR:%s Could not draw equation\n", ERROR_M, DEFAULT_COLOUR_M);
  equation.pos++;

  return 0;
}

// Draw the equation on the screen
int draw_equation() {
  // Draws specific character passed into function
  int x_min = Get_Area(24, 0);
  int x_max = Get_Area(21, 1);
  int y_min = Get_Area(24, 2);
  int y_max = Get_Area(24, 3);

  int offset = 0;

// TODO if (LCD_Cell_Colour(x_min, x_max, y_min, y_max, LCD_COLOR_BLACK,
//                      LCD_COLOR_WHITE) != 0) {
//    printf("%sERROR:%s Could not clear cell\n", ERROR_M, DEFAULT_COLOUR_M);
//  }

  // Find center of cell given
  int x_pos = ((x_max - x_min) / 2.0) + x_min;
  int y_pos = ((y_max - y_min) / 2.0) + y_min;

  if (Get_Debug() == 1)
    printf(
        "%sDEBUG_INFO:%s x_min %i, x_max %i, y_min %i, y_max %i, x_pos %i, "
        "y_pos %i\n",
        DEBUG_M, DEFAULT_COLOUR_M, x_min, x_max, y_min, y_max, x_pos, y_pos);

  // Roll around the display
  int LCDResultlen = 20;

  if (equation.pos > LCDResultlen) {
    offset = equation.pos - LCDResultlen;
  }

  char *temp_equation = "";
  if (equation.pos == 0) {
    temp_equation = (char *)calloc(LCDResultlen, sizeof(char));
    if (output.formula == 0) {
      printf("%sERROR:%s Calloc output memory", ERROR_M, DEFAULT_COLOUR_M);
      return 1;
    }
    temp_equation[0] = output.formula[0];
  } else {
    for (int i = offset; i < equation.pos; i++) {
      temp_equation[i - offset] = output.formula[i];
    }
  }

  char *temp = &output.formula[offset];

  //TODO BSP_LCD_SetFont(&Font16);
  //TODO BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  //TODO BSP_LCD_DisplayStringAt(x_min + 5, y_pos, (uint8_t *)temp, LEFT_MODE);

  return 0;
}
