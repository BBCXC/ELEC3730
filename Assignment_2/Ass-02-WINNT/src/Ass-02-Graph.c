#include "Ass-02-Graph.h"

  int display_height = 256; //TODO HACK
  int display_width = 340; //TODO HACK

// if switched call different mode
void GraphInit() {
  // Initialize and turn on LCD and calibrate the touch panel
// TODO// TODO BSP_LCD_Init();
// TODO//  BSP_LCD_DisplayOn();
// TODO//  BSP_TP_Init();

  // Clear screen
// TODO//TODO  BSP_LCD_Clear(LCD_COLOR_WHITE);
// TODO//  BSP_LCD_SetFont(&Font12);
// TODO//  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  layout.x_axis_min = DEFAULT_X_MIN;
  layout.x_axis_max = DEFAULT_X_MAX;
  layout.y_axis_min = DEFAULT_Y_MIN;
  layout.y_axis_max = DEFAULT_Y_MAX;

  graph.x_min = layout.x_axis_min;
  graph.x_max = layout.x_axis_max;
  graph.y_min = layout.y_axis_min; 
  graph.y_max = layout.y_axis_max;
  graph.delta = DEFAULT_DELTA;

  // Set basic layout of calculator grid
  if (graph_layout() != 0) {
    printf("%sERROR:%s Unable to draw layout", ERROR_M, DEFAULT_COLOUR_M);
  }
  // Initilise numpad symbol layout
  if (draw_axisnum() != 0) {
    printf("%sERROR:%s Unable to draw axis numbers", ERROR_M, DEFAULT_COLOUR_M);
  }
}

void GraphProcess() {
  double x_min = graph.x_min;
  double x_max = graph.x_max;
  int y_min = graph.y_min;
  int y_max = graph.y_max;
  double delta = graph.delta;


  // Store the string
  static char graph_formula[50];
  //printf("Formula Input %s\n", Get_Formula()); //TODO HACK
  strcpy(graph_formula, Get_Formula());
  graph.formula = graph_formula;
  if (parseFormula() == 0) {
	
    Set_Prev_ans(Get_Result());
	
    graph.prev_ans = Get_Result();
	  //printf("1 Result %lf\n", Get_Result());
  }
  //TODO This needs to scale to the screen
  for (double i = x_min + delta; i <= x_max;) {
	  //printf("DELTA: %lf\n", graph.delta);
    Set_Formula(graph_formula);
    Set_Graph_Increment(i);
    //printf("Set graph increment should be: %lf \t but is: %lf\n",i, Get_Graph_Increment());
    if (parseFormula() == 0) {
      Set_Prev_ans(Get_Result());
      // TODOBSP_LCD_DrawLine(i - delta, graph.prev_ans, i, Get_Result());
      printf("Draw Line From point(%lf,%lf) to point(%lf, %lf)\n",i-delta, graph.prev_ans, i, Get_Result());
      graph.prev_ans = Get_Result();
      //printf("%lf Result %lf\n", i, Get_Result());
    }
    i = i + delta;
    //printf("Increment i from %lf to %lf in steps of %lf\n", x_min, x_max, i);

  }
}

int graph_layout() {
  // LCD X and Y Size
  // TODOint display_height = BSP_LCD_GetYSize();
  // TODOint display_width = BSP_LCD_GetXSize();

  int x_axis = display_height / 2;
  int y_axis = display_width / 2;

  // TODOBSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  // TODOBSP_LCD_DrawVLine(y_axis, 0, display_height);
  // TODOBSP_LCD_DrawHLine(0, x_axis, display_width);

  return 0;
}

// Draw numpad layout
int draw_axisnum() {

  // TODOint display_height = BSP_LCD_GetYSize();
  // TODOint display_width = BSP_LCD_GetXSize();

  int x_axis = display_height / 2;
  int y_axis = display_width / 2;
  // Print X AXIS values
  int x_axis_min = layout.x_axis_min;
  int x_axis_max = layout.x_axis_max;

  // Spacing between x axis values
  int x_spacing = display_width / MAX_AXIS_NUM;
  int x_num_increment = (x_axis_max - x_axis_min) / MAX_AXIS_NUM;
  double printnum = 0;
  char num_str[50];
// TODO// TODO BSP_LCD_SetFont(&Font12);
// TODO//  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  for (int i = 0; i <= MAX_AXIS_NUM; i++) {
	
    printnum = x_axis_min + i * x_num_increment;
	
    snprintf(num_str, 50, "%g", printnum);
	
	  printf("Print x axis number %s\n", num_str);
// TODO//  TODO  BSP_LCD_DisplayStringAt((i * x_spacing), x_axis - CHAR_HEIGHT,
//                            (uint8_t *)num_str, CENTER_MODE);
  }
  // Print Y AXIS values
  int y_axis_min = layout.y_axis_min;
  int y_axis_max = layout.y_axis_max;

  // Spacing between x axis values
  int y_spacing = display_height / MAX_AXIS_NUM;
  int y_num_increment = (y_axis_max - y_axis_min) / MAX_AXIS_NUM;

// TODO// TODO  BSP_LCD_SetFont(&Font12);
// TODO//  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  for (int i = 0; i <= MAX_AXIS_NUM; i++) {
    printnum = y_axis_min + i * y_num_increment;
    snprintf(num_str, 50, "%g", printnum);
    printf("Print y axis number %s\n", num_str);
// TODO//  TODO  BSP_LCD_DisplayStringAt(y_axis - CHAR_HEIGHT, (i * y_spacing),
//                            (uint8_t *)num_str, CENTER_MODE);
  }

  return 0;
}

void set_axis_scale(int i, double Value) {
  if(i == 2){
    layout.x_axis_min = Value;
  }
  else if(i == 3){
    layout.x_axis_max = Value;
  }
  else if(i == 4){
    layout.y_axis_min = Value;
  }
  else if(i == 5){
    layout.y_axis_max = Value;
  }
}

double Get_axis_scale(int Value){
  if(Value == 1){
    return(layout.x_axis_min);
  }
  else if(Value == 2){
    return(layout.x_axis_max);
  }
  else if(Value == 3){
    return(layout.y_axis_min);
  }
  else if(Value == 4){
    return(layout.y_axis_max);
  }
  return(0);
}

void rescale_graph() {
  GraphInit();
  if(graph.formula != NULL){
	Set_Formula(graph.formula);
    GraphProcess();
  }
  else{
    printf("%sERROR:%s No formula currently entered to rescale\n", ERROR_M, DEFAULT_COLOUR_M);
  }
}

void reset_scale() {
  
  set_axis_scale(2, DEFAULT_X_MIN);
  set_axis_scale(3, DEFAULT_X_MAX);
  set_axis_scale(4, DEFAULT_Y_MIN);
  set_axis_scale(5, DEFAULT_Y_MAX);
  GraphInit();
  GraphProcess();
}

void graph_help() {
  printf("NOTE: This is unfinished\n");
  printf("reset_scale: resets scale to default, redraws graph\n");
  printf("rescale: redraws current graph with current scale\n");
  printf("scale: sets scale to <x_min> <x_max> <y_min> <y_max>\n");
}

int Graph_StringProcess(char *command_line, int i) {
  static double prev_ans = 0;
  char **array_of_words_p;

  int word_count = string_parser(command_line, &array_of_words_p);

  if (Get_System() == 1) {
    for (int i = 0; i < word_count; i++) {
      printf("%sSYSTEM_INFO:%s Word %i: %s\n", SYS_M, DEFAULT_COLOUR_M, i,
             array_of_words_p[i]);
    }
  }

  // Graph mode ON
  if (word_count > 0) {
    if (command_parser(&array_of_words_p, word_count, &prev_ans) == 1) {
    } else {
      Set_Formula(array_of_words_p[0]);
      Set_Result(0);
  
      GraphProcess();
  
    }

    free(array_of_words_p[0]);
    free(array_of_words_p);

    if (Get_Debug() == 1)
      printf("%sDEBUG_INFO:%s Arrays have been freed\n", DEBUG_M,
             DEFAULT_COLOUR_M);

  } else {
    printf("%sERROR:%s No strings detected\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  return 0;
}

void Set_Graph_Increment(double Value){
	graph.increment = Value;
}

double Get_Graph_Increment(){
	return(graph.increment);
}

