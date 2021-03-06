#ifndef ASS_02_DEBUG_H_
#define ASS_02_DEBUG_H_

#include "Ass-02.h"

typedef struct {
  int debug;
  int system;
  int formula_mode;
  int first_time;
  int graph_mode;
  int angle_mode;
} sys_t;
sys_t info;

extern void Debug_Init(void);
extern int Get_Debug(void);
extern int Get_System(void);
extern int Get_Formula_Mode(void);
extern int Get_First_Time(void);
extern int Get_Graph_Mode(void);
extern void Set_Debug(int Value);
extern void Set_System(int Value);
extern void Set_Formula_Mode(int Value);
extern void Set_First_Time(int Value);
extern void Set_Graph_Mode(int Value);

#endif /* ASS_02_DEBUG_H_ */
