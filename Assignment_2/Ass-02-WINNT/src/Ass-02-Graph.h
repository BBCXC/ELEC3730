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
extern void GraphInit(void);
extern void GraphProcess(void);

double Map_X_Display(double Input);
double Map_Y_Display(double Input);

#endif /* ASS_02_GRAPH_H_ */
