osSemaphoreWait(myBinarySem05Handle, osWaitForever);
osMutexWait(myMutex01Handle, osWaitForever);
for (i = 0; i < 500; i = i + 500) {
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DrawVLine(XOFF + xpos, YOFF, YSIZE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    ypos = (uint16_t)((uint32_t)(ADC_Value[i]) * YSIZE / 4096);
    BSP_LCD_DrawLine(XOFF + last_xpos, YOFF + last_ypos, XOFF + xpos, YOFF + ypos);
    Window_buffer[xpos] = ypos;
    // BSP_LCD_FillRect(xpos,ypos,1,1);
    last_xpos = xpos;
    last_ypos = ypos;
    xpos++;

    // safe_printf("First half, %d, %d, %d\n", i, last_xpos, last_ypos);
}
osMutexRelease(myMutex01Handle);
if (last_xpos >= XSIZE - 1) {
    xpos      = 0;
    last_xpos = 0;
}

// Wait for second half of buffer
osSemaphoreWait(myBinarySem06Handle, osWaitForever);
HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_SET);
osMutexWait(myMutex01Handle, osWaitForever);
for (i = 0; i < 500; i = i + 500) {
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DrawVLine(XOFF + xpos, YOFF, YSIZE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    ypos = (uint16_t)((uint32_t)(ADC_Value[i + 500]) * YSIZE / 4096);
    BSP_LCD_DrawLine(XOFF + last_xpos, YOFF + last_ypos, XOFF + xpos, YOFF + ypos);
    // BSP_LCD_FillCircle(xpos,ypos,2);
    last_xpos = xpos;
    last_ypos = ypos;
    xpos++;

    // safe_printf("Second half, %d, %d, %d\n", i + 500, last_xpos, last_ypos);
}
osMutexRelease(myMutex01Handle);
if (last_xpos >= XSIZE - 1) {
    xpos      = 0;
    last_xpos = 0;
}