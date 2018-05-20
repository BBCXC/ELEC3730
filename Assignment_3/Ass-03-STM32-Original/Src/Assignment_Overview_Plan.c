// Files to setup and functions for the
//	- buttons
//	- Window
//	- Input
//	- Screen
//	- Debug

// Start task 1
// Initilise all setup variables
// call task 2 and 3 to initilise
// osSignalWait for all tasks

// While(1) all tasks

void Ass_03_Task_01(void const* argument) {
    debug_init();
    window_init();
    input_init();
    screen_init();
    button_init();


    while (1) {
        // Three tasks for thread 1
        // Task 1 Check Button
        // Task 2 Get input from DMA, Store in Window_buf
        // Task 3 Message Task 2 Current State
    }
}