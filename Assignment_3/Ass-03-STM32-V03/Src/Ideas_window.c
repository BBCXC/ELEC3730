
// Create a buffer for the window
int Buf_len                = 250;
int Window_buffer[Buf_len] = calloc(250, sizeof(int));
static int semaphore_state = 0;
// Create a buffer for the 10,000 samples
uint16_t ADC_Buffer[10000] = calloc(10000, sizeof(uint16_t));

uint16_t ADC_Value[1000];

static int begin = 0;

int Samples = Zoom_Coeff * 1000;

int Bin_len = Samples / Buf_len;


while (1) {
    int avg = 0;

    // If these change maybe i should reprint it all
    Samples = Zoom_Coeff * 1000;
    Bin_len = Samples / Buf_len;

    // Begin
    // Bin_len ~ 40 can change with zoom
    // Max value is 10,000
    if (begin % 1000 > 500) {
        // First semaphore on
        semaphore_state = 0;
    }
    else {
        // Second semaphore on
        semaphore_state = 1;
    }
    for (int i = begin; i < begin + bin_len; i++) {
        // Store the ADC_Value in the ADC_Buffer
        if ((begin % 1000 > 500) && semaphore_state == 1) {
            osSemaphorewait(5);
            semaphore_state = 0;
        }
        else if ((begin % 1000 <= 500) && semaphore_state == 0) {
            osSemaphorewait(6);
            semaphore_state = 1;
        }
        ADC_Buffer[begin] = ADC_Value[begin % 1000];

        // Add up all of the average values
        avg += ADC_Buffer[begin];
    }

    // Store the average in the buffer
    Increment_Win_Ptr();
    Window_buffer[win_ptr] = (int) (avg / ((double) bin_len));

    // Find max
    int Max_Win = 0;
    for (int i = 0; i < 250; i++) {
        if (Window_buffer[i] > Max_Win) {
            Max_Win = Window_buffer[i];
        }
    }

    begin += bin_len;
    if (begin >= SAMPLES) {
        Begin = 0;
    }

    // Draw Dot
    int scale     = ? ? ;
    int win_ptr_s = win_ptr;
    int y_pos     = Window_buffer[win_ptr];
    int x_pos     = win_ptr;
    win_ptr++;
    if (win_ptr >= 250) {
        win_ptr = 0;
    }
    while (win_ptr_s != win_ptr) {
        // TODO scale
        BSP(x_pos, y_pos * scale, win_ptr, Window_buffer[win_ptr] * scale);
        x_pos = win_ptr;
        y_pos = Window_buffer[win_ptr];
        win_ptr++;
        if (win_ptr >= 250) {
            win_ptr = 0;
        }
    }
    win_ptr++;
}