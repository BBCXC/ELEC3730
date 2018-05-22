#include "Ass-03-Popup.h"
#include "Ass-03.h"

void popup_init() {
    osMutexWait(popup_Handle, osWaitForever);
    popup.position[0] = (int) (BSP_LCD_GetXSize() / 6);
    popup.position[1] = (int) (BSP_LCD_GetYSize() / 6);
    popup.position[2] = (int) (5 * (BSP_LCD_GetXSize() / 6));
    popup.position[3] = (int) (5 * (BSP_LCD_GetYSize() / 6));

    popup.status = 0;
    osMutexRelease(popup_Handle);
}

int Get_Popup_Position(int element) {
    osMutexWait(popup_Handle, osWaitForever);
    int temp = popup.position[element];
    osMutexRelease(popup_Handle);
    return (temp);
}

int Get_Popup_Status() {
    osMutexWait(popup_Handle, osWaitForever);
    int temp = popup.status;
    osMutexRelease(popup_Handle);
    return (temp);
}


void Set_Popup_Status(int value) {
    osMutexWait(popup_Handle, osWaitForever);
    popup.status = value;
    osMutexRelease(popup_Handle);
}
