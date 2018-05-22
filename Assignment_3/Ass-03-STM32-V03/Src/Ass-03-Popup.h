//     $Date: 2018-05-10 10:07:15 +1000 (Thu, 10 May 2018) $
// $Revision: 1301 $
//   $Author: Peter $

// Assignment 3 include file

#ifndef ASS_03_POPUP_H_
#define ASS_03_POPUP_H_

#include "Ass-03.h"

typedef struct {
    int position[4];
    int status;
} popup_t;
popup_t popup;

extern void popup_init();
extern int Get_Popup_Position(int element);
extern int Get_Popup_Status();
extern void Set_Popup_Status(int value);

#endif /* ASS_03_POPUP_H_ */
