//     $Date: 2018-05-22 06:24:02 +1000 (Tue, 22 May 2018) $
// $Revision: 1330 $
//   $Author: Peter $

// TODO MACRO/Function that takes a sting and arguments and if debug is on prints the debug statement

#include "Ass-03.h"

// This is the console task. Can use your code from the previous assignment
// and expand.
//
// The example code is very basic, showing access to the SD card. Note that
// if the SD card is access from both this task and the pulse rate monitor
// task, a MUTEX will be required.
//
// *** MAKE UPDATES TO THE CODE AS REQUIRED ***
//

uint8_t myReadFile();
uint8_t myWriteFile();

FIL MyFile;
FIL MyFile2, MyFile3;
FRESULT Status;

void Ass_03_Task_01(void const* argument) {

    uint32_t loop = 0;
    uint8_t ts[100];
    uint16_t i;
    Coordinate display;
    char c;

    // printf(RESET_M);
    // printf(CLEAR_M);
    printf(KNRM);

    safe_printf("Hello from Task 1 - Console (serial input)\n");
    safe_printf("INFO: Initialise LCD and TP first...\n");

    // STEPIEN: Initialize and turn on LCD and calibrate the touch panel
    BSP_LCD_Init();
    BSP_LCD_DisplayOn();
    BSP_TP_Init();
    //

    BSP_LCD_Clear(LCD_COLOR_WHITE);

    // Initilise structs here
    debug_init();
    window_init();
    // button_init();
    popup_init();
    Equation_Init();

    safe_printf("All structures initilised\n");

    // Signal other tasks to start
    osSignalSet(myTask02Handle, 1);
    osSignalSet(myTask03Handle, 1);
    osSignalSet(myTask04Handle, 1);

    // Display welcome message
    osMutexWait(myMutex01Handle, osWaitForever);
    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font12);
    // BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    // BSP_LCD_DisplayStringAt(5, 5, (uint8_t*) "ELEC3730 Assignment 3 (v03 $Rev: 1330 $)", LEFT_MODE);
    // BSP_LCD_DisplayStringAt(5, 20, (uint8_t*) "This demonstration code can be used", LEFT_MODE);
    // BSP_LCD_DisplayStringAt(5, 35, (uint8_t*) "as a starting point for the assignment.", LEFT_MODE);
    osMutexRelease(myMutex01Handle);

    while (1) {
        c = getchar();
        safe_printf("Task 1: %d (got '%c')\n", loop, c);
        loop++;
        myReadFile();
        // myWriteFile();


        CommandLineParserProcess();
    }
}

uint8_t myReadFile() {
#define READ_FILE "Hello.txt"
#define BUFF_SIZE 256
    uint8_t rtext[BUFF_SIZE];
    FRESULT res;
    uint32_t bytesread;

    // Open file Hello.txt
    if ((res = f_open(&MyFile, READ_FILE, FA_READ)) != FR_OK) {
        safe_printf("ERROR: Opening '%s'\n", READ_FILE);
        return 1;
    }
    safe_printf("Task 1: Opened file '%s'\n", READ_FILE);

    // Read data from file
    if ((res = f_read(&MyFile, rtext, BUFF_SIZE - 1, &bytesread)) != FR_OK) {
        safe_printf("ERROR: Reading '%s'\n", READ_FILE);
        f_close(&MyFile);
        return 1;
    }

    //    TCHAR* buff;
    //	UINT len = 100;
    //    res = getcwd(&buff, len);

    rtext[bytesread] = '\0';
    safe_printf("Task 1: Read: '%s'\n", rtext);

    // Close file
    f_close(&MyFile);

    return 0;
}

uint8_t myWriteFile() {
#define WRITE_FILE "There.txt"
    FRESULT res;
    UINT byteswritten;

    // Open file There.txt
    if ((res = f_open(&MyFile, WRITE_FILE, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK) {
        safe_printf("ERROR: Opening '%s'\n", WRITE_FILE);
        return 1;
    }
    safe_printf("Task 1: Opened file '%s'\n", WRITE_FILE);

    // Write to file
    if ((res = f_write(&MyFile, "Hello", 6, &byteswritten)) != FR_OK) {
        safe_printf("ERROR: Writing '%s'\n", WRITE_FILE);
        f_close(&MyFile);
        return 1;
    }
    safe_printf("Task 1: Written: %d bytes\n", byteswritten);

    // Close file
    f_close(&MyFile);


    return 0;
}

void CommandLineParserProcess(void) {
    char c;
    static int i = 0;
    static char command_line[101];

    if (Get_First_Time() == 1) {
        Set_First_Time(0);
        printf("--> Enter text: ");
    }
    if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK) {
        // printf("%c", c); TODO Should be fflush

        HAL_GPIO_TogglePin(GPIOD, LD4_Pin);  // Toggle LED4
        command_line[i] = c;
        i++;

        // If we get a return character then process the string
        if (c == '\r' || i > 101) {
            printf("\n");
            command_line[i - 1] = '\0';
            if (StringProcess(&command_line, i) != 0) {
                printf("%sERROR:%s Could not process string\n", ERROR_M, DEFAULT_COLOUR_M);
            }
            i = 0;
            Set_First_Time(1);
        }
    }
}

int StringProcess(char* command_line, int i) {
    static double prev_ans = 0;
    char** array_of_words_p;

    char** path_p;
    int path_count = 0;

    int word_count = string_parser(command_line, &array_of_words_p, ' ');

    if (1) {
        for (int i = 0; i < word_count; i++) {
            printf("%sSYSTEM_INFO:%s Word %i: %s\n", SYS_M, DEFAULT_COLOUR_M, i, array_of_words_p[i]);
        }
    }
    int mode = -1;
    mode     = command_parser(&array_of_words_p, word_count, &path_p, &path_count);

    if (mode == -1) {
        printf("%sERROR:%s Unknown Operation\n", ERROR_M, DEFAULT_COLOUR_M);
    }
    else if (mode == 0) { /* insert debug, analog function etc */
        myReadFile();
    }
}

// Takes input string, splits into words
int string_parser(char* inp, char** array_of_words_p[], char delim) {

    char curr_char;
    char prev_char = delim;
    int num_words  = 0;
    int len_word   = 0;
    int characters = 0;
    int i;

    // Iterate through string and count the numbr of words
    for (i = 0; i < strlen(inp); i++) {
        curr_char = inp[i];

        if (curr_char == '\0')
            return 0;                                         // No words in string
        else if (curr_char == delim && prev_char == delim) {  // Multiple spaces in a row
        }
        else if (curr_char != delim && prev_char == delim) {  // Found new word
            num_words++;
            characters++;
        }
        else if (curr_char == delim && prev_char != delim) {
        }
        else {  // Currently in word, don't do anything
            characters++;
        }
        prev_char = curr_char;
    }

    // Allocate enough memory to store a pointer to each word
    *array_of_words_p = (char**) calloc(num_words, sizeof(char*));
    if (array_of_words_p == 0) {  // If malloc fails returns NULL ptr
        printf("%sERROR:%s Memory allocation failed\n", ERROR_M,
               DEFAULT_COLOUR_M);  // Log
        return -1;                 // Return Failed
    }
    // Set pointer to first word
    char* word_array = (char*) calloc((characters + num_words), sizeof(char));
    if (word_array == 0) {  // If malloc fails returns NULL ptr
        printf("%sERROR:%s Memory allocation failed\n", ERROR_M,
               DEFAULT_COLOUR_M);  // Log
        free(*array_of_words_p);
        return -1;  // Return Failed
    }

    // Reset variables
    prev_char  = delim;
    curr_char  = delim;
    num_words  = 0;
    characters = 0;

    // Iterate over string again
    for (i = 0; i < strlen(inp); i++) {
        curr_char = inp[i];

        if (curr_char == '\0')
            return 0;                                         // No words in string
        else if (curr_char == delim && prev_char == delim) {  // Multiple spaces in a row
        }
        else if (curr_char != delim && prev_char == delim) {  // Found new word
            characters++;
            num_words++;
            len_word                           = 1;
            (*array_of_words_p)[num_words - 1] = &word_array[characters - 1 + (num_words - 1)];
        }
        else if (curr_char == delim && prev_char != delim) {  // Found end of word
            // Copy inp into memory allocation
            strncpy(&word_array[characters - len_word + (num_words - 1)], (inp + (i - len_word)), len_word);
            len_word = 0;
        }
        else {  // Currently in word, don't do anything
            characters++;
            len_word++;
        }
        prev_char = curr_char;
    }
    if (len_word != 0) {
        // Copy inp into memory allocation
        strncpy(&word_array[characters - len_word + (num_words - 1)], (inp + (i - len_word)), len_word);
        len_word = 0;
    }
    return num_words;
}


// clang-format off
/***********************************************************************************************************************
**************************************************Operation Function****************************************************
***********************************************************************************************************************/
// clang-format on

int analog_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    int value_1;
    if (Get_Debug() == 1) printf("%sDEBUG_INFO:%s Analog function detected", DEBUG_M, DEFAULT_COLOUR_M);

    if (word_count == 2) {
        if (sscanf((*array_of_words_p)[1], "%d", &value_1) != 1) {
            printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
            return 1;
        }
        else {
            Set_Analog(value_1);
        }
    }
    else {
        safe_printf("Too many arguments\n");
    }
}

// TODO ls
int ls_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) printf("%sDEBUG_INFO:%s ls function detected", DEBUG_M, DEFAULT_COLOUR_M);

    FRESULT res;
    DIR dir;
    UINT i = 0;
    static FILINFO fno;

    const int buf_len = 50;
    char* Cur_dir     = (char*) calloc(buf_len, sizeof(char));

    char* item[2];

    if (word_count > 1) {
        // get current directory and store it, f_getcwd
        res = f_getcwd(Cur_dir, buf_len);
        if (res != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command\n", ERROR_M, DEFAULT_COLOUR_M);
            return 1;
        }

        res = f_chdir(array_of_words_p[1]);
        if (res != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command\n", ERROR_M, DEFAULT_COLOUR_M);
            return 1;
        }
    }
    // Read the first folder/file
    res = f_readdir(&dir, &fno);

    while (res != FR_OK || fno.fname[0] == 0) {
        sprintf(&item[i][0], "/%s", fno.fname);
        // If the item is a directory
        if (fno.fattrib & AM_DIR) {
            item[i][1] = 1;
        }
        // else the item is a file
        else {
            item[i][1] = 0;
        }
        res = f_readdir(&dir, &fno);
        i++;
    }
    item[i][0] = NULL;

    // Change back to the original directory if there was one
    if (strcmp(Cur_dir, NULL) == 0) {
        // change directory back
        if (f_chdir(Cur_dir) != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command\n", ERROR_M, DEFAULT_COLOUR_M);
            return 1;
        }
    }
    free(Cur_dir);

    // TODO Print the list of items out

    return 0;
}

// TODO cd
int cd_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) printf("%sDEBUG_INFO:%s cd function detected", DEBUG_M, DEFAULT_COLOUR_M);

    FRESULT res;

    if (word_count > 1) {
        // change directory to the path
        res = f_chdir(array_of_words_p[1]);
        if (res != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command\n", ERROR_M, DEFAULT_COLOUR_M);
            return 1;
        }
    }
    else {
        safe_printf("Too many arguments\n");
    }
    return 0;
}

// TODO mkdir
int mkdir_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) printf("%sDEBUG_INFO:%s mkdir function detected", DEBUG_M, DEFAULT_COLOUR_M);

    FRESULT res;

    if (word_count > 1) {
        res = f_mkdir(array_of_words_p[1]);
        if (res != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command\n", ERROR_M, DEFAULT_COLOUR_M);
            return 1;
        }
    }
    else {
        safe_printf("Too many arguments\n");
    }
}

// TODO cp
int cp_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    int value_1;
    if (Get_Debug() == 1) printf("%sDEBUG_INFO:%s cp function detected", DEBUG_M, DEFAULT_COLOUR_M);

    if (word_count == 2) {
        // TODO
        // Declare **word[]
        // Split path variables up into words
        // Do something
        // FRESULT f_rename (const TCHAR* path_old, const TCHAR* path_new);    /* Rename/Move a file or directory */
    }
    else {
        safe_printf("Too many arguments\n");
    }
}

int rm_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) printf("%sDEBUG_INFO:%s rm function detected", DEBUG_M, DEFAULT_COLOUR_M);

    FRESULT res;
    if (word_count > 1) {
        res = f_unlink(array_of_words_p[1]);
        if (res != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command\n", ERROR_M, DEFAULT_COLOUR_M);
            return 1;
        }
    }
    else {
        return 1;
    }
    return 0;
}

// TODO
int Get_Absolute_Path() {
    FRESULT fr;
    const int buf_len = 50;
    char* Cur_dir     = (char*) calloc(buf_len, sizeof(char));

    fr = f_getcwd(Cur_dir, buf_len); /* Get current directory path */

    free(Cur_dir);
}

int expr_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) printf("%sDEBUG_INFO:%s Analog function detected", DEBUG_M, DEFAULT_COLOUR_M);

    if (word_count == 2) {
        Set_Formula(array_of_words_p[1]);
        if (parseFormula() == 0) {
            safe_printf("Answer: %g", Get_Result());
        }
    }
    else {
        safe_printf("Too many arguments\n");
    }
}

// Change between setting on and off
int debug_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) printf("%sDEBUG_INFO:%s Entered Debug Mode\n", DEBUG_M, DEFAULT_COLOUR_M);
    if (word_count > 1) {
        if (strcmp("on", (*array_of_words_p)[1]) == 0) {
            Set_Debug(1);
            printf("Debug ON\n");
        }
        else if (strcmp("off", (*array_of_words_p)[1]) == 0) {
            Set_Debug(0);
            printf("Debug OFF\n");
        }
        else {
            printf("%sERROR:%s Unknown debug command\n", ERROR_M, DEFAULT_COLOUR_M);
            return 1;
        }
    }
    else {
        printf("Debug messages currently %s\n", Get_Debug() == 0 ? "OFF" : "ON");
    }
    return 0;
}

// Change between setting on and off
int system_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) printf("%sDEBUG_INFO:%s Entered System Mode\n", DEBUG_M, DEFAULT_COLOUR_M);
    if (word_count > 1) {
        if (strcmp("on", (*array_of_words_p)[1]) == 0) {
            Set_System(1);
            printf("%sSYSTEM_INFO:%s System ON\n", SYS_M, DEFAULT_COLOUR_M);
        }
        else if (strcmp("off", (*array_of_words_p)[1]) == 0) {
            Set_System(0);
            printf("%sSYSTEM_INFO:%s System OFF\n", SYS_M, DEFAULT_COLOUR_M);
        }
        else {
            printf("%sERROR:%s Unknown system command\n", ERROR_M, DEFAULT_COLOUR_M);
            return 1;
        }
    }
    else {
        printf("%sSYSTEM_INFO:%s System messages currently %s\n",
               SYS_M,
               DEFAULT_COLOUR_M,
               Get_System() == 0 ? "OFF" : "ON");
    }
    return 0;
}

// Clear function, clear terminal screen
int clear_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) printf("%sDEBUG_INFO:%s Entered HELP function\n", DEBUG_M, DEFAULT_COLOUR_M);
    printf(CLEAR_M);
    return 0;
}

// Reset function, reset terminal scrollback
int reset_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) printf("%sDEBUG_INFO:%s Entered HELP function\n", DEBUG_M, DEFAULT_COLOUR_M);
    printf(RESET_M);
    return 0;
}

// Help function, display help messages
int help_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) printf("%sDEBUG_INFO:%s Entered HELP function\n", DEBUG_M, DEFAULT_COLOUR_M);
    if (help_parser(array_of_words_p, word_count) != 0) {
        printf("%sERROR:%s Help Funtion\n", ERROR_M, DEFAULT_COLOUR_M);
    }
    return 0;
}

// clang-format off
/***********************************************************************************************************************
*************************************************Save, Load, Navigate***************************************************
***********************************************************************************************************************/
// clang-format on

// TODO
// Save
// given an array save file with the given name
// needs overwrite function
// needs new file function

// TODO
// Load
// on load read the file into an array and give it to another thread

// TODO
// Navigate
// This will take the current directory and give the list of correct files to the other thread
