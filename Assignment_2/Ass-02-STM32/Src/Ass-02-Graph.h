#ifndef ASS_02_GRAPH_H_
#define ASS_02_GRAPH_H_

#include "Ass-02.h"

#define MAX_AXIS_NUM 6
#define CHAR_HEIGHT 5
#define DEFAULT_X_MIN 0
#define DEFAULT_X_MAX 5
#define DEFAULT_Y_MIN -10
#define DEFAULT_Y_MAX 10
#define DEFAULT_DELTA 0.01

typedef struct {
  char *formula;
  int pos;
  double result;
  double prev_ans;
  double x_min;
  double x_max;
  double y_min;
  double y_max;
  double delta;
  double increment;
} formula;
formula graph;

typedef struct {
  int x_axis_min;
  int x_axis_max;
  int y_axis_min;
  int y_axis_max;

} layout_t;
layout_t layout;

extern double Get_Graph_Increment(void);
extern int Get_Graph_Mode(void);
extern void GraphInit(void);
extern void GraphProcess(void);
extern int Graph_StringProcess(char *command_line, int i);
extern int graph_layout(void);

extern double Map_X_Display(double Input);
extern double Map_Y_Display(double Input);

extern int draw_axisnum(void);
extern void graph_help(void);
extern void set_axis_scale(int i, double Value);
extern double Get_axis_scale(int Value);
extern void Set_Graph_Increment(double Value);
extern void reset_scale(void);
extern void rescale_graph(void);

#endif /* ASS_02_GRAPH_H_ */
