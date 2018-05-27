//     $Date: 2018-05-10 10:07:15 +1000 (Thu, 10 May 2018) $
// $Revision: 1301 $
//   $Author: Peter $

// Assignment 3 include file

#ifndef ASS_03_FILEW_H_
#define ASS_03_FILEW_H_

#include "Ass-03.h"

typedef struct {
    int dir_chg;
    int index;
    int num;
    int pos_x;
    int pos_y;
    char** name;
} file_t;
file_t filew;

extern void filew_init();
extern int Get_Dir_Chg();
extern void Set_Dir_Chg(int Value);
extern int Get_File_Index();
extern void Set_File_Index(int Value);
extern char* Get_File_Name(int index);
extern void Set_File_Name(int index, char* name);
extern int Get_File_Num();
extern void Set_File_Num(int index);
extern int Get_File_Window_y();
extern int Get_File_Window_x();

#endif /* ASS_03_FILEW_H_ */
