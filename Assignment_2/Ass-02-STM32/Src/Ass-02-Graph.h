#ifndef ASS_02_GRAPH_H_
#define ASS_02_GRAPH_H_

#include "Ass-02.h"

#define MAX_AXIS_NUM 6
#define CHAR_HEIGHT 5
#define DEFAULT_X_MIN -10
#define DEFAULT_X_MAX 10
#define DEFAULT_Y_MIN -10
#define DEFAULT_Y_MAX 10

typedef struct {
  char *formula;
  int pos;
  double result;
  double prev_ans;
  int x_min;
  int x_max;
  int y_min;
  int y_max;
  int delta;
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



#endif /* ASS_02_GRAPH_H_ */
