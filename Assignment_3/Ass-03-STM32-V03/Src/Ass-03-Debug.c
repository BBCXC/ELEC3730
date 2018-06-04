#include "Ass-03-Debug.h"
#include "Ass-03.h"

void debug_init() {
    osMutexWait(debug_Handle, osWaitForever);
    info.State_Thread = 1;
    info.debug        = 0;
    info.system       = 0;
    info.first_time   = 1;
    info.analog       = 10;
    osMutexRelease(debug_Handle);
}

int Get_State_Thread() {
    osMutexWait(debug_Handle, osWaitForever);
    int temp = info.State_Thread;
    osMutexRelease(debug_Handle);
    return temp;
}

void Set_State_Thread(int Value) {
    osMutexWait(debug_Handle, osWaitForever);
    info.State_Thread = Value;
    osMutexRelease(debug_Handle);
}

int Get_Debug(void) {
    osMutexWait(debug_Handle, osWaitForever);
    int temp = info.debug;
    //safe_printf("Getting debug -> %d\n", info.debug);
    osMutexRelease(debug_Handle);
    return (temp);
}
int Get_System(void) {
    osMutexWait(debug_Handle, osWaitForever);
    int temp = info.system;
    osMutexRelease(debug_Handle);
    return (temp);
}
void Set_Debug(int Value) {
    osMutexWait(debug_Handle, osWaitForever);
    info.debug = Value;
    osMutexRelease(debug_Handle);
}
void Set_System(int Value) {
    osMutexWait(debug_Handle, osWaitForever);
    info.system = Value;
    osMutexRelease(debug_Handle);
}

int Get_Analog(void) {
    osMutexWait(debug_Handle, osWaitForever);
    int temp = info.analog;
    osMutexRelease(debug_Handle);
    return (temp);
}
void Set_Analog(int Value) {
    osMutexWait(debug_Handle, osWaitForever);
    info.analog = Value;
    osMutexRelease(debug_Handle);
}

int Get_Angle_Mode(void) {
    osMutexWait(debug_Handle, osWaitForever);
    int temp = info.angle_mode;
    osMutexRelease(debug_Handle);
    return (temp);
}

void Set_Angle_Mode(int Value) {
    osMutexWait(debug_Handle, osWaitForever);
    info.angle_mode = Value;
    osMutexRelease(debug_Handle);
}

int Get_First_Time(void) {
    osMutexWait(debug_Handle, osWaitForever);
    int temp = info.first_time;
    osMutexRelease(debug_Handle);
    return (temp);
}
void Set_First_Time(int Value) {
    osMutexWait(debug_Handle, osWaitForever);
    info.first_time = Value;
    osMutexRelease(debug_Handle);
}
