#include "Ass-03-File-w.h"
#include "Ass-03.h"

void filew_init() {
    osMutexWait(file_Handle, osWaitForever);
    filew.dir_chg = 0;
    filew.index   = 0;
    filew.num     = 1;
    filew.pos_x   = 70;
    filew.pos_y   = 220;
    filew.name = (char**) calloc(10, sizeof(char*));
    filew.name[0] = "No Files";
    osMutexRelease(file_Handle);
}

int Get_Dir_Chg() {
    osMutexWait(file_Handle, osWaitForever);
    int temp = filew.dir_chg;
    osMutexRelease(file_Handle);
    return temp;
}

void Set_Dir_Chg(int Value) {
    osMutexWait(file_Handle, osWaitForever);
    filew.dir_chg = Value;
    osMutexRelease(file_Handle);
}

int Get_File_Index() {
    osMutexWait(file_Handle, osWaitForever);
    int temp = filew.index;
    osMutexRelease(file_Handle);
    return temp;
}

void Set_File_Index(int Value) {
    osMutexWait(file_Handle, osWaitForever);
    filew.index = Value;
    osMutexRelease(file_Handle);
}

void Get_File_Name(char** temp, int index) {
    DEBUG_P
    osMutexWait(file_Handle, osWaitForever);
    DEBUG_P
	safe_printf("%s\n", filew.name[0]);
    DEBUG_P
    *temp =  filew.name[index];

    osMutexRelease(file_Handle);
    DEBUG_P
}

void Set_File_Name(int index, char* name) {
    osMutexWait(file_Handle, osWaitForever);
    filew.name[index] = name;
    osMutexRelease(file_Handle);
}

int Get_File_Num() {
    osMutexWait(file_Handle, osWaitForever);
    int temp = filew.num;
    osMutexRelease(file_Handle);
    return temp;
}

void Set_File_Num(int index) {
    osMutexWait(file_Handle, osWaitForever);
    filew.num = index;
    osMutexRelease(file_Handle);
}

int Get_File_Window_y() {
    return filew.pos_y;
}

int Get_File_Window_x() {
    return filew.pos_x;
}
