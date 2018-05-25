//     $Date: 2018-05-10 10:07:15 +1000 (Thu, 10 May 2018) $
// $Revision: 1301 $
//   $Author: Peter $

// Assignment 3 include file

#ifndef ASS_03_DEBUG_H_
#define ASS_03_DEBUG_H_

#include "Ass-03.h"


typedef struct {
    int State_Thread;
    int debug;
    int system;
    int analog;
    int angle_mode;
    int first_time;
} info_t;
info_t info;

extern void debug_init();
extern int Get_State_Thread();
extern void Set_State_Thread(int Value);
extern int Get_Debug(void);
extern int Get_System(void);
extern void Set_Debug(int Value);
extern void Set_System(int Value);
extern int Get_Analog();
extern void Set_Analog(int Value);

#endif /* ASS_03_DEBUG_H_ */
