#include "math.h"
#include "stdio.h"

#define YSIZE 142
#define YOFF 0
#define XOFF 0

#define MAX_INPUT 2  // 4096

int Get_Zoom_Coeff_w      = 1;
int semaphore_state       = 1;
double Window_buffer[250] = {0};
int begin                 = 0;
int win_ptr               = 0;
int Buf_len               = 250;
double max                = 0.0;

double ADC_Value[1000];

FILE* fp;
char xpos_b[10] = "";
char ypos_b[10] = "";

extern void func(void);

void main() {

    fp = fopen("test.csv", "w+");
    for (int j = 0; j < Get_Zoom_Coeff_w; j++) {
        for (int i = 0; i < 1000; i++) {
            ADC_Value[i] = sin(j + i) + 1;
        }

        func();
    }
    fclose(fp);
}

void func() {

    // Wait for first half of buffer
    int first = 1;

    // If these change maybe i should reprint it all
    int Samples = Get_Zoom_Coeff_w * 1000;  // (1 to 10) * 1000hz
    int Bin_len = Samples / Buf_len;

    while (begin < 1000) {
        double avg = 0;
        if (begin > 500) {
            // First semaphore on
            semaphore_state = 0;
        }
        else {
            // Second semaphore on
            semaphore_state = 1;
        }
        for (int i = begin; i < begin + Bin_len; i++) {
            // Store the ADC_Value in the ADC_Buffer
            if ((begin > 500) && semaphore_state == 1) {
                // osSemaphoreWait(myBinarySem05Handle, osWaitForever);
                semaphore_state = 0;
            }
            else if ((begin <= 500) && semaphore_state == 0) {
                // osSemaphoreWait(myBinarySem06Handle, osWaitForever);
                semaphore_state = 1;
            }

            // Add up all of the average values
            avg += ADC_Value[i];  // * ((double)YSIZE / 4096.0);
            printf("avg %lf ADC_Value %lf\n", avg, ADC_Value[i]);
        }

        win_ptr++;
        if (win_ptr >= 250) {
            win_ptr = 0;
        }
        Window_buffer[win_ptr] = YSIZE * ((double) avg / (MAX_INPUT * (double) Bin_len));  // TODO Map to the max
        printf("Win_buf %d is %lf\n", win_ptr, Window_buffer[win_ptr]);

        if (Window_buffer[win_ptr] > max) {
            max = Window_buffer[win_ptr];
        }

        begin += Bin_len;
        // if (begin >= 1000) {
        //     begin = 0;
        //     // safe_printf("1000 Samples\n");
        // }
    }

    printf("Max is %lf\n", max);
    double scale = ((double) (YSIZE) / ((double) max));
    printf("Scale is %lf\n", scale);

    int last_xpos = 0;
    int last_ypos = 0;
    // osMutexWait(myMutex01Handle, osWaitForever);
    for (int i = 0; i < 250; i++) {
        win_ptr++;
        if (win_ptr >= 250) {
            win_ptr = 0;
        }
        // BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        // BSP_LCD_DrawVLine(XOFF + i, YOFF, YSIZE);
        // BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        int ypos = (int) (Window_buffer[win_ptr] * scale);
        printf("px %3d, py %3d, cx %3d, cy %3d\n", XOFF + last_xpos, YOFF + last_ypos, XOFF + i, YOFF + ypos);
        fprintf(fp, "%d, %d\n", i, ypos);
        // BSP_LCD_DrawLine(XOFF + last_xpos, YOFF + last_ypos, XOFF + i, YOFF +
        // ypos);

        last_xpos = i;
        last_ypos = ypos;

        if (ypos > 141) {
            printf("Above me is the magical number\n");
        }
    }
    // osMutexRelease(myMutex01Handle);
}