//     $Date: 2018-05-22 06:24:02 +1000 (Tue, 22 May 2018) $
// $Revision: 1330 $
//   $Author: Peter $

// TODO MACRO/Function that takes a sting and arguments and if debug is on prints the debug statement
// TODO Figure out what the tab character is to tab complete
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

void Ass_03_Task_01(void const* argument) {

    uint32_t loop = 0;
    uint8_t ts[100];
    uint16_t i;
    Coordinate display;
    char c;

    safe_printf(RESET_M);
    safe_printf(CLEAR_M);
    safe_printf(KNRM);

    safe_printf("Task 1 Initialize\n");

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
    filew_init();

    safe_printf("Structures initilised\n");

    // Signal other tasks to start
    osSignalSet(myTask02Handle, 1);
    osSignalSet(myTask03Handle, 1);
    osSignalSet(myTask04Handle, 1);

    // myReadFile();

    while (1) {
        if (FileProcess() != 0) {
            safe_printf("%sERROR:%s Could not process string\n", ERROR_M, DEFAULT_COLOUR_M);
        }

        char c;
        static int i = 0;
        static char command_line[101];

        if (Get_First_Time() == 1) {
            Set_First_Time(0);
            safe_printf("--> Enter text: ");
            // char* path = Get_Absolute_Path();
            // safe_printf("%s", path);
            fflush(stdout);
        }
        // if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK) {
        c = getchar();
        safe_printf("%c", c);
        fflush(stdout);

        HAL_GPIO_TogglePin(GPIOD, LD4_Pin);  // Toggle LED4
        command_line[i] = c;
        i++;

        // If we get a return character then process the string
        if (c == '\r' || i > 101) {
            safe_printf("\n");
            command_line[i - 1] = '\0';
            if (StringProcess(&command_line, i) != 0) {
                safe_printf("%sERROR:%s Could not process string\n", ERROR_M, DEFAULT_COLOUR_M);
            }
            i = 0;
            Set_First_Time(1);
        }
        //}
    }
}

// void CommandLineParserProcess(void) {
//     char c;
//     static int i = 0;
//     static char command_line[101];
//     FRESULT res;
//     int buf_len = 50;
//     char Cur_dir[buf_len];

//     if (Get_First_Time() == 1) {

//         Set_First_Time(0);

//         res = f_getcwd(Cur_dir, buf_len);
//         if (res != FR_OK) {
//             safe_printf("%sERROR:%s Function f_getcwd failed %d\n", ERROR_M, DEFAULT_COLOUR_M, res);
//         }
//         safe_printf("%s\n", Cur_dir);
//         fflush(stdout);
//     }
//     if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK) {
//         safe_printf("%c", c);
//         fflush(stdout);
//         HAL_GPIO_TogglePin(GPIOD, LD4_Pin);  // Toggle LED4
//         command_line[i] = c;
//         i++;

//         // If we get a return character then process the string
//         if (c == '\r' || i > 101) {
//             safe_printf("\n");
//             command_line[i - 1] = '\0';
//             if (StringProcess(&command_line, i) != 0) {
//                 safe_printf("%sERROR:%s Could not process string\n", ERROR_M, DEFAULT_COLOUR_M);
//             }
//             i = 0;
//             Set_First_Time(1);
//         }
//     }
// }

int StringProcess(char* command_line, int i) {
    static double prev_ans = 0;
    char** array_of_words_p;

    char** path_p;
    int path_count = 0;

    int word_count = string_parser(command_line, &array_of_words_p, ' ');

    if (1) {
        for (int i = 0; i < word_count; i++) {
            if (Get_Debug() == 1) {
                safe_printf("%sSYSTEM_INFO:%s Word %i: %s\n", SYS_M, DEFAULT_COLOUR_M, i, array_of_words_p[i]);
            }
        }
    }
    int mode = -1;
    mode     = command_parser(&array_of_words_p, word_count, &path_p, &path_count);

    if (mode == -1) {
        safe_printf("%sERROR:%s Unknown Operation\n", ERROR_M, DEFAULT_COLOUR_M);
        return -1;
    }
    return 0;
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
        safe_printf("%sERROR:%s Memory allocation failed\n", ERROR_M,
                    DEFAULT_COLOUR_M);  // Log
        return -1;                      // Return Failed
    }
    // Set pointer to first word
    char* word_array = (char*) calloc((characters + num_words), sizeof(char));
    if (word_array == 0) {  // If malloc fails returns NULL ptr
        safe_printf("%sERROR:%s Memory allocation failed\n", ERROR_M,
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
    if (Get_Debug() == 1) {
        safe_printf("%sDEBUG_INFO:%s Analog function detected\n", DEBUG_M, DEFAULT_COLOUR_M);
    }
    if (word_count == 2) {
        if (sscanf((*array_of_words_p)[1], "%d", &value_1) != 1) {
            safe_printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
            return 1;
        }
        else {
            Set_Analog(value_1);
            safe_printf("Set analog to %d\n", value_1);
        }
    }
    else {
        safe_printf("Argument Error\n");
    }
    return 0;
}

int ls_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {

    if (Get_Debug() == 1) {
        safe_printf("%sDEBUG_INFO:%s ls function detected\n", DEBUG_M, DEFAULT_COLOUR_M);
    }

    FRESULT res;
    DIR dir;
    UINT i = 0;
    static FILINFO fno;
    char* prev_fno = "";

    const int buf_len = 50;
    char* Cur_dir     = (char*) calloc(buf_len, sizeof(char));
    char* Save_dir    = (char*) calloc(buf_len, sizeof(char));
    // TODO Check calloc

    char** name;
    char** type;
    int chgdir_flag = 0;


    res = f_getcwd(Save_dir, buf_len);
    if (res != FR_OK) {
        safe_printf("%sERROR:%s Function f_getcwd failed %d\n", ERROR_M, DEFAULT_COLOUR_M, res);
        return 1;
    }

    if (word_count > 1) {
        // get current directory and store it, f_getcwd
        chgdir_flag = 1;

        res = f_chdir((*array_of_words_p)[1]);
        if (res != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command f_chdir %d\n", ERROR_M, DEFAULT_COLOUR_M, res);
            return 1;
        }

        res = f_getcwd(Cur_dir, buf_len);
        if (res != FR_OK) {
            safe_printf("%sERROR:%s Function f_getcwd failed %d\n", ERROR_M, DEFAULT_COLOUR_M, res);
            return 1;
        }
    }
    res = f_opendir(&dir, Cur_dir); /* Open the directory */
    while (res == FR_OK) {
        res = f_readdir(&dir, &fno); /* Read a directory item */
        // safe_printf("%s\n", fno.fname);/* It is a file. */
        if (fno.fattrib & AM_DIR) {
            safe_printf("%s%s%s\n", KMAG, fno.fname, KNRM); /* It is a file. */
        }
        else {
            safe_printf("%s%s%s\n", KCYN, fno.fname, KNRM); /* It is a file. */
        }
        if (strcmp(fno.fname, prev_fno) == 0) {
            break;
        }
    }

    // Change back to the original directory if there was one
    if (chgdir_flag == 1) {
        // change directory back
        if (f_chdir(Save_dir) != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command %d\n", ERROR_M, DEFAULT_COLOUR_M, res);
            return 1;
        }
    }
    free(Cur_dir);
    free(Save_dir);

    return 0;
}

int cd_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) safe_printf("%sDEBUG_INFO:%s cd function detected\n", DEBUG_M, DEFAULT_COLOUR_M);

    FRESULT res;

    if (word_count < 3) {
        if (word_count < 2) {
            (*array_of_words_p)[1] = "/";
        }
        // change directory to the path
        res = f_chdir((*array_of_words_p)[1]);
        if (res != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command %d\n", ERROR_M, DEFAULT_COLOUR_M, res);
            return 1;
        }
    }
    else {
        safe_printf("Argument Error\n");
    }
    return 0;
}

int mkdir_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) safe_printf("%sDEBUG_INFO:%s mkdir function detected\n", DEBUG_M, DEFAULT_COLOUR_M);

    FRESULT res;

    if (word_count > 1) {
        safe_printf("Making folder %s\n", (*array_of_words_p)[1]);
        res = f_mkdir((*array_of_words_p)[1]);
        if (res != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command %d\n", ERROR_M, DEFAULT_COLOUR_M, res);
            return 1;
        }
    }
    else {
        safe_printf("Argument Error\n");
    }
    return 0;
}

int mv_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {

    if (Get_Debug() == 1) safe_printf("%sDEBUG_INFO:%s mv function detected\n", DEBUG_M, DEFAULT_COLOUR_M);
    FRESULT res;
    if (word_count == 3) {
        res = f_rename((*array_of_words_p)[1], (*array_of_words_p)[2]);
        if (res != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command %d\n", ERROR_M, DEFAULT_COLOUR_M, res);
            return 1;
        }
    }
    // TODO this moves the file not copy
    else {
        safe_printf("Argument Error\n");
    }
    return 0;
}


int cp_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {

    if (Get_Debug() == 1) safe_printf("%sDEBUG_INFO:%s cp function detected\n", DEBUG_M, DEFAULT_COLOUR_M);
    FRESULT res;
    if (word_count == 3) {
        FIL fsrc, fdst;    /* File objects */
        BYTE buffer[4096]; /* File copy buffer */
        FRESULT fr;        /* FatFs function common result code */
        UINT br, bw;       /* File read/write count */

        /* Open source file on the drive 1 */
        fr = f_open(&fsrc, (*array_of_words_p)[1], FA_READ);
        if (fr) return (int) fr;

        /* Create destination file on the drive 0 */
        fr = f_open(&fdst, (*array_of_words_p)[2], FA_WRITE | FA_CREATE_ALWAYS);
        if (fr) return (int) fr;

        /* Copy source to destination */
        for (;;) {
            fr = f_read(&fsrc, buffer, sizeof buffer, &br); /* Read a chunk of source file */
            if (fr || br == 0) break;                       /* error or eof */
            fr = f_write(&fdst, buffer, br, &bw);           /* Write it to the destination file */
            if (fr || bw < br) break;                       /* error or disk full */
        }

        /* Close open files */
        f_close(&fsrc);
        f_close(&fdst);

        return (int) fr;
    }
    return -1;
}


int rm_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) safe_printf("%sDEBUG_INFO:%s rm function detected\n", DEBUG_M, DEFAULT_COLOUR_M);

    FRESULT res;
    if (word_count > 1) {
        res = f_unlink((*array_of_words_p)[1]);
        if (res != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command %d\n", ERROR_M, DEFAULT_COLOUR_M, res);
            return 1;
        }
    }
    else {
        return 1;
    }
    return 0;
}

// TODO Remove if unused
char* Get_Absolute_Path() {
    FRESULT fr;
    const int buf_len = 50;
    char* Cur_dir;

    fr = f_getcwd(Cur_dir, buf_len); /* Get current directory path */

    return Cur_dir;
}

int expr_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) safe_printf("%sDEBUG_INFO:%s Analog function detected\n", DEBUG_M, DEFAULT_COLOUR_M);

    if (word_count == 2) {
        Set_Formula((*array_of_words_p)[1]);
        if (parseFormula() == 0) {
            safe_printf("Answer: %g\n", Get_Result());
        }
    }
    else {
        safe_printf("Argument Error\n");
    }
    return 0;
}

int cat_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) safe_printf("%sDEBUG_INFO:%s cat function detected\n", DEBUG_M, DEFAULT_COLOUR_M);

    if (word_count == 2) {
        Read_CSV((*array_of_words_p)[1]);
    }
    else {
        safe_printf("Argument Error\n");
    }
    return 0;
}

int record_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) safe_printf("%sDEBUG_INFO:%s Record function detected\n", DEBUG_M, DEFAULT_COLOUR_M);
    Set_State_Thread(3);
    if (word_count >= 2) {
        if (word_count == 4) {
            if (strcmp("delay", (*array_of_words_p)[2]) == 0) {
                int value_1;
                if (sscanf((*array_of_words_p)[1], "%d", &value_1) != 1) {
                    printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
                    return 1;
                }
                Set_Delay_Time(value_1);
            }
        }
        else {
            Set_Delay_Time(0);
        }
        int value_1;
        if (sscanf((*array_of_words_p)[1], "%d", &value_1) != 1) {
            printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
            return 1;
        }
        Set_Record_Time(value_1);
    }
    else {
        safe_printf("Argument Error\n");
    }
    return 0;
}

// Change between setting on and off
int debug_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) {
        safe_printf("%sDEBUG_INFO:%s Entered Debug Mode\n", DEBUG_M, DEFAULT_COLOUR_M);
    }
    if (word_count > 1) {
        if (strcmp("on", (*array_of_words_p)[1]) == 0) {
            Set_Debug(1);
            safe_printf("Debug ON\n");
        }
        else if (strcmp("off", (*array_of_words_p)[1]) == 0) {
            Set_Debug(0);
            safe_printf("Debug OFF\n");
        }
        else {
            safe_printf("%sERROR:%s Unknown debug command\n", ERROR_M, DEFAULT_COLOUR_M);
            return 1;
        }
    }
    else {
        safe_printf("Debug messages currently %s\n", Get_Debug() == 0 ? "OFF" : "ON");
    }
    return 0;
}

// Change between setting on and off
int system_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) safe_printf("%sDEBUG_INFO:%s Entered System Mode\n", DEBUG_M, DEFAULT_COLOUR_M);
    if (word_count > 1) {
        if (strcmp("on", (*array_of_words_p)[1]) == 0) {
            Set_System(1);
            safe_printf("%sSYSTEM_INFO:%s System ON\n", SYS_M, DEFAULT_COLOUR_M);
        }
        else if (strcmp("off", (*array_of_words_p)[1]) == 0) {
            Set_System(0);
            safe_printf("%sSYSTEM_INFO:%s System OFF\n", SYS_M, DEFAULT_COLOUR_M);
        }
        else {
            safe_printf("%sERROR:%s Unknown system command\n", ERROR_M, DEFAULT_COLOUR_M);
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
    if (Get_Debug() == 1) safe_printf("%sDEBUG_INFO:%s Entered HELP function\n", DEBUG_M, DEFAULT_COLOUR_M);
    safe_printf(CLEAR_M);
    return 0;
}

// Reset function, reset terminal scrollback
int reset_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) safe_printf("%sDEBUG_INFO:%s Entered HELP function\n", DEBUG_M, DEFAULT_COLOUR_M);
    safe_printf(RESET_M);
    return 0;
}

// Help function, display help messages
int help_function(char** array_of_words_p[], int word_count, char** path_p[], int path_count) {
    if (Get_Debug() == 1) safe_printf("%sDEBUG_INFO:%s Entered HELP function\n", DEBUG_M, DEFAULT_COLOUR_M);
    if (help_parser(array_of_words_p, word_count) != 0) {
        safe_printf("%sERROR:%s Help Funtion\n", ERROR_M, DEFAULT_COLOUR_M);
    }
    return 0;
}

int FileProcess() {
    if (Get_Debug() == 1) {
        safe_printf("%sDEBUG_INFO:%s FileProcess function detected\n", DEBUG_M, DEFAULT_COLOUR_M);
    }


    FRESULT res;
    DIR dir;
    static FILINFO fno;
    char* empty_fno = "";

    const int buf_len     = 50;
    char* Save_dir        = (char*) calloc(buf_len, sizeof(char));
    static char* Prev_dir = "";
    // TODO Check calloc

    res = f_getcwd(Save_dir, buf_len);
    if (res != FR_OK) {
        safe_printf("%sERROR:%s Function f_getcwd failed %d\n", ERROR_M, DEFAULT_COLOUR_M, res);
        return 1;
    }

    if (strcmp(Save_dir, Prev_dir) == 0) {
        return 0;
    }

    res = f_findfirst(&dir, &fno, Save_dir, "*.csv");
    if (res != FR_OK) {
        safe_printf("%sERROR:%s Unknown system command f_findfirst %d\n", ERROR_M, DEFAULT_COLOUR_M, res);
        return 1;
    }
    else if (strcmp(fno.fname, empty_fno) != 0) {
        safe_printf("First file %d found %s\n", Get_File_Num(), fno.fname);
        Set_File_Name(Get_File_Num(), fno.fname);
        Set_File_Num(1);
    }

    while (res == FR_OK) {
        res = f_findnext(&dir, &fno); /* Read a directory item */
        if (res != FR_OK) {
            safe_printf("%sERROR:%s Unknown system command f_findnext %d\n", ERROR_M, DEFAULT_COLOUR_M, res);
            return 1;
        }

        if (strcmp(fno.fname, empty_fno) == 0) {
            break;
        }
        Set_File_Name(Get_File_Num(), fno.fname);
        Set_File_Num(Get_File_Num() + 1);
        safe_printf("Next file %d found %s\n", Get_File_Num(), fno.fname);

        // TODO Remove test code
        char* test;
        Get_File_Name(&test, Get_File_Num() - 1);
        safe_printf("This is in test %s\n", test);
        // TODO Remove test code
    }

    Prev_dir = Save_dir;


    Set_Dir_Chg(1);
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

// Write CSV
int Write_CSV(char* FILE_NAME, int save_state, int* data, int data_len) {
    // Do we want to write to a new file or overwrite?

    FIL fp;
    UINT byteswritten;
    FRESULT res;

    if (save_state == 0) {
        // Create file pointer
        // Open file with that name
        if ((res = f_open(&fp, FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK) {
            safe_printf("ERROR: Opening '%s'\n", FILE_NAME);
            return 1;
        }
        safe_printf("Task 1: Opened New file '%s'\n", FILE_NAME);
    }
    else {
        // We must be overwriting a file
        // Get name of file to overwrite and delete that file
        // Create file pointer
        // Open file with that name
        if ((res = f_open(&fp, FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK) {
            safe_printf("ERROR: Opening '%s'\n", FILE_NAME);
            return 1;
        }
        safe_printf("Task 1: Overwriting file '%s'\n", FILE_NAME);
    }

    for (int i = 0; i < data_len; i++) {
        safe_printf("Writing data '%d'", i);
        if ((res = f_write(&fp, &i, sizeof(int), &byteswritten)) != FR_OK) {
            safe_printf("ERROR: Writing '%s'\n", FILE_NAME);
            f_close(&fp);
            return 1;
        }
        if ((res = f_write(&fp, ",", sizeof(char), &byteswritten)) != FR_OK) {
            safe_printf("ERROR: Writing '%s'\n", FILE_NAME);
            f_close(&fp);
            return 1;
        }
        safe_printf(", '%d'\n", data[i]);
        if ((res = f_write(&fp, &data[i], sizeof(int), &byteswritten)) != FR_OK) {
            safe_printf("ERROR: Writing '%s'\n", FILE_NAME);
            f_close(&fp);
            return 1;
        }
        if ((res = f_write(&fp, "\n", sizeof(char), &byteswritten)) != FR_OK) {
            safe_printf("ERROR: Writing '%s'\n", FILE_NAME);
            f_close(&fp);
            return 1;
        }
    }

    if ((res = f_close(&fp)) != FR_OK) {
        safe_printf("ERROR: Writing '%s'\n", FILE_NAME);
        f_close(&fp);
        return 1;
    }

    safe_printf("Completed writing to file -> %s\n", FILE_NAME);
    return 0;
}


int Read_CSV(char* FILE_NAME) {
    FRESULT res;
    uint32_t bytesread;
    int temp_i   = 0;
    char* temp_s = "";
    FIL fp;

    // Open file Hello.txt
    if ((res = f_open(&fp, FILE_NAME, FA_READ)) != FR_OK) {
        safe_printf("ERROR: Opening '%s'\n", FILE_NAME);
        return 1;
    }
    safe_printf("Task 1: Opened file '%s'\n", FILE_NAME);

    for (int i = 0; i < 250; i++) {
        if ((res = f_read(&fp, &temp_i, sizeof(int), &bytesread)) != FR_OK) {
            safe_printf("ERROR: Writing '%s'\n", FILE_NAME);
            f_close(&fp);
            return 1;
        }
        safe_printf("X pos %3d", temp_i);
        if (bytesread == 0) break;
        if ((res = f_read(&fp, &temp_s, sizeof(char), &bytesread)) != FR_OK) {
            safe_printf("ERROR: Writing '%s'\n", FILE_NAME);
            f_close(&fp);
            return 1;
        }
        safe_printf("%s ", temp_s);
        if (bytesread == 0) break;
        if ((res = f_read(&fp, &temp_i, sizeof(int), &bytesread)) != FR_OK) {
            safe_printf("ERROR: Writing '%s'\n", FILE_NAME);
            f_close(&fp);
            return 1;
        }
        safe_printf("Value %d", temp_i);
        if (bytesread == 0) break;
        if ((res = f_read(&fp, &temp_s, sizeof(char), &bytesread)) != FR_OK) {
            safe_printf("ERROR: Writing '%s'\n", FILE_NAME);
            f_close(&fp);
            return 1;
        }
        safe_printf("%s\n", temp_s);
        if (bytesread == 0) break;
    }

    // Close file
    f_close(&fp);

    return 0;
}
