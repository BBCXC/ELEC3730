//     $Date: 2018-05-10 10:07:15 +1000 (Thu, 10 May 2018) $
// $Revision: 1301 $
//   $Author: Peter $

// Assignment 3 include file

#ifndef ASS_03_INPUT_H_
#define ASS_03_INPUT_H_

#include "Ass-03.h"

typedef struct {

} input_t;
input_t input;

typedef struct {
	int record_time;
	int delay_time;
}record_t;
record_t record;

extern void inputInit();
extern int Get_Record_Time();
extern int Get_Record_Delay();
extern void Set_Delay_Time(int Value);
extern void Set_Record_Time(int Value);


#endif /* ASS_03_INPUT_H_ */
