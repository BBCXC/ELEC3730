#include "Ass-03-Popup.h"
#include "Ass-03.h"

void popup_init() {
    osMutexWait(popup_Handle, osWaitForever);
    popup.position[0] = (int) (BSP_LCD_GetXSize() / 3);
    popup.position[1] = (int) (BSP_LCD_GetXSize() / 3);
    popup.position[2] = (int) (2 * (BSP_LCD_GetYSize() / 3));
    popup.position[3] = (int) (2 * (BSP_LCD_GetYSize() / 3));
    osMutexRelease(popup_Handle);
}

int Get_Popup_Position(int element) {
    osMutexWait(popup_Handle, osWaitForever);
    int temp = popup.position[element];
    osMutexRelease(popup_Handle);
    return (temp);
}
