#include "Ass-03-Button.h"
#include "Ass-03.h"

void button_init() {

    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    // TODO Populate the position of each button
    if (Populate_Button_Position() == 0) {
        safe_printf("Initilised Buttons\n");
    }
    if (Draw_Button_list() == 0) {
        safe_printf("Initilised Buttons\n");
    }
    if (Draw_Button_boxes() == 0) {
        safe_printf("Initilised Boxes\n");
    }
}

void pbutton_init() {

    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    // TODO Populate the position of each button
    if (Draw_Popup_list() == 0) {
        safe_printf("Initilised Popup\n");
    }
    if (Draw_Popup_boxes() == 0) {
        safe_printf("Initilised Boxes\n");
    }
}

// clang-format off
button_s Button_list[] = {
    {"play",      {0,    66,     0,    60},     &draw_blist_item, "play"},	//&draw_play
    {"stop",      {0,    66,     60,   120},    &draw_blist_item, "stop"},	//&draw_stop
    {"save",      {0,    66,     120,  180},    &draw_blist_item, "save"},
    {"load",      {0,    66,     180,  240},    &draw_blist_item, "load"},
    {"zoom_in",   {66,   129,    120,  180},    &draw_blist_item, "+"},
    {"zoom_out",  {129,  192,    120,  180},    &draw_blist_item, "-"},
    {"reset",     {192,  255,    120,  180},    &draw_blist_item, "x"},
    {"up",        {255,  320,    120,  180},    &draw_blist_item, "up"}, //draw_up
    {"down",      {255,  320,    180,  240},    &draw_blist_item, "down"}, //draw_down
    {NULL,        {NULL, NULL,   NULL, NULL},   NULL,             NULL}};
// clang-format on

// clang-format off
button_s Popup_list[] = {
    {"new",       {54,    125,    140,    200},    &draw_plist_item, "new"},
    {"overwrite", {125,   195,    140,    200},    &draw_plist_item, "overwrite"},
    {"cancel",    {195,   266,    140,    200},    &draw_plist_item, "cancel"},
    {NULL,        {NULL,  NULL,   NULL,   NULL},   NULL,             NULL}};
// clang-format on

int Populate_Button_Position() {
    // Left coloum width = line width - window_x
    // Right coloum width = Left

    // Play min = line width
    // Play max = window height / 2 + line width

    // Stop min = Play max + line width
    // Stop Max = window height + line width

    // TODO
	return 0;
}

char* Get_Button_Name(int element, int status) {
    if (status == 0) {
        return (Button_list[element].NameString);
    }
    else if (status == 1) {
        return (Popup_list[element].NameString);
    }
    return NULL;
}

// Return the item touched on the screen
int get_touch_pos(int display_x, int display_y, int popup) {
    int i = 0;
    if (popup == 0) {
        safe_printf("Searching button list\n");
        // osMutexWait(button_Handle, osWaitForever);
        while (Button_list[i].NameString != NULL) {
            safe_printf("Looking for button touched, %d\n", i);
            if ((display_x >= Button_list[i].position[0]) && (display_x <= Button_list[i].position[1])
                && (display_y >= Button_list[i].position[2]) && (display_y <= Button_list[i].position[3])) {
                safe_printf("Button Touch %s\n", Button_list[i].NameString);
                return (i);
            }
            i++;
        }
        // osMutexRelease(button_Handle);
    }
    else if (popup == 1) {
        safe_printf("Searching popup list\n");
        while (Popup_list[i].NameString != NULL) {
            if ((display_x >= Popup_list[i].position[0]) && (display_x <= Popup_list[i].position[1])
                && (display_y >= Popup_list[i].position[2]) && (display_y <= Popup_list[i].position[3])) {
                return (i);
            }
            i++;
        }
    }
    safe_printf("No button touched\n");
    return (100);
}

// If a command is called function decides which function pointer to return
int Draw_Button_list() {
    int i = 0;
    osMutexWait(button_Handle, osWaitForever);
    // While we haven't checked the whole list
    while (Button_list[i].NameString != NULL) {
        safe_printf("Initilising button %s\n", Button_list[i].NameString);
        // If we find the function we want, call it
        // Draws all of the buttons
        if (Button_list[i].Function_p(i) == 0) {
            safe_printf("Success button %s\n", Button_list[i].NameString);
        }
        else {
            safe_printf("Error button %s\n", Button_list[i].NameString);
            return -1;
        }
        i++;
    }
    osMutexRelease(button_Handle);
    return 0;
}

// If a command is called function decides which function pointer to return
int Draw_Button_boxes() {
    int i = 0;
    osMutexWait(button_Handle, osWaitForever);
    // While we haven't checked the whole list
    while (Button_list[i].NameString != NULL) {
        safe_printf("Initilising button box %s\n", Button_list[i].NameString);
        // If we find the function we want, call it
        // Draws all of the buttons
        BSP_LCD_DrawRect(Button_list[i].position[0],
                         Button_list[i].position[2],
                         Button_list[i].position[1] - Button_list[i].position[0],
                         Button_list[i].position[3] - Button_list[i].position[2]);
        i++;
    }
    osMutexRelease(button_Handle);
    return 0;
}

// If a command is called function decides which function pointer to return
int Draw_Popup_list() {
    int i = 0;
    osMutexWait(popup_Handle, osWaitForever);
    // While we haven't checked the whole list
    while (Popup_list[i].NameString != NULL) {
        safe_printf("Initilising button %s\n", Popup_list[i].NameString);
        // If we find the function we want, call it
        // Draws all of the buttons
        if (Popup_list[i].Function_p(i) == 0) {
        }
        else {
            return -1;
        }
        i++;
    }
    osMutexRelease(popup_Handle);
    return 0;
}

int Draw_Popup_boxes() {
    int i = 0;
    // While we haven't checked the whole list
    while (Popup_list[i].NameString != NULL) {
        safe_printf("Initilising button box %s\n", Popup_list[i].NameString);
        // If we find the function we want, call it
        // Draws all of the buttons
        BSP_LCD_DrawRect(Popup_list[i].position[0],
                         Popup_list[i].position[2],
                         Popup_list[i].position[1] - Popup_list[i].position[0],
                         Popup_list[i].position[3] - Popup_list[i].position[2]);
        i++;
    }
    return 0;
}

int draw_blist_item(int index) {
    // TODO Mutex on button
    int x_min  = Button_list[index].position[0];
    int x_max  = Button_list[index].position[1];
    int y_min  = Button_list[index].position[2];
    int y_max  = Button_list[index].position[3];
    char* item = Button_list[index].Symbol;
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = ((x_max - x_min) / 2) + x_min;
    int y_cen = ((y_max - y_min) / 2) + y_min;

    safe_printf("x y pos of %s is %d, %d\n", item, x_cen, y_cen);
    // TODO Mutex on LCD
    osMutexWait(myMutex01Handle, osWaitForever);
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) item, CENTER_MODE);
    // TODO Mutex off LCD
    osMutexRelease(myMutex01Handle);

    return 0;
}

int draw_plist_item(int index) {
    // TODO Mutex on button
    int x_min  = Popup_list[index].position[0];
    int x_max  = Popup_list[index].position[1];
    int y_min  = Popup_list[index].position[2];
    int y_max  = Popup_list[index].position[3];
    char* item = Popup_list[index].Symbol;
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = ((x_max - x_min) / 2) + x_min;
    int y_cen = ((y_max - y_min) / 2) + y_min;

    // TODO Mutex on LCD
    osMutexWait(myMutex01Handle, osWaitForever);
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) item, CENTER_MODE);
    // TODO Mutex off LCD
    osMutexRelease(myMutex01Handle);

    return 0;
}

int draw_play(int index) {
    // TODO Mutex on button
    osMutexWait(button_Handle, osWaitForever);
    int x_min = Button_list[index].position[0];
    int x_max = Button_list[index].position[1];
    int y_min = Button_list[index].position[2];
    int y_max = Button_list[index].position[3];
    osMutexRelease(button_Handle);
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = ((x_max - x_min) / 2) + x_min;
    int y_cen = ((y_max - y_min) / 2) + y_min;

    // TODO Mutex on LCD
    osMutexWait(myMutex01Handle, osWaitForever);
    // TODO Draw specific symbol / word
    int width = 1;
    for (int row = 0; row < SYMBOL_SIZE; row++) {
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        width < SYMBOL_SIZE / 2 ? width++ : width--;
        BSP_LCD_DrawHLine(x_cen - (SYMBOL_SIZE / 2), y_cen + row, 1);

        if (width < 3) {
            BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
            BSP_LCD_DrawHLine(x_cen - (SYMBOL_SIZE / 2) + 1, y_cen + row, width - 2);
        }
        if (width != 1) {
            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            BSP_LCD_DrawHLine(width + 1, y_cen + row, 1);
        }
    }
    // TODO Mutex off LCD
    osMutexRelease(myMutex01Handle);

    return 0;
}

int draw_stop(int index) {
    // TODO Mutex on button
    osMutexWait(button_Handle, osWaitForever);
    int x_min = Button_list[index].position[0];
    int x_max = Button_list[index].position[1];
    int y_min = Button_list[index].position[2];
    int y_max = Button_list[index].position[3];
    osMutexRelease(button_Handle);
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = ((x_max - x_min) / 2) + x_min;
    int y_cen = ((y_max - y_min) / 2) + y_min;

    // TODO Mutex on LCD
    osMutexWait(myMutex01Handle, osWaitForever);
    // TODO Draw specific symbol / word
    for (int row = 0; row < SYMBOL_SIZE; row++) {
        // If it's the first row, or the last row
        // Only print black pixels
        if (row == 0 || row == SYMBOL_SIZE - 1) {
            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            BSP_LCD_DrawHLine(x_cen - (SYMBOL_SIZE / 2), y_cen + row, SYMBOL_SIZE);
        }

        // Any other draw a black pixel either side of the red
        else {
            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            BSP_LCD_DrawHLine(x_cen - (SYMBOL_SIZE / 2), y_cen + row, 1);
            BSP_LCD_SetTextColor(LCD_COLOR_RED);
            BSP_LCD_DrawHLine(x_cen - (SYMBOL_SIZE / 2) + 1, y_cen + row, SYMBOL_SIZE - 2);
            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            BSP_LCD_DrawHLine(SYMBOL_SIZE, y_cen + row, 1);
        }
    }
    // TODO Mutex off LCD
    osMutexRelease(myMutex01Handle);

    return 0;
}

int draw_up(int index) {
    // TODO Mutex on button
    osMutexWait(button_Handle, osWaitForever);
    int x_min = Button_list[index].position[0];
    int x_max = Button_list[index].position[1];
    int y_min = Button_list[index].position[2];
    int y_max = Button_list[index].position[3];
    osMutexRelease(button_Handle);
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = ((x_max - x_min) / 2) + x_min;
    int y_cen = ((y_max - y_min) / 2) + y_min;

    // TODO Mutex on LCD
    osMutexWait(myMutex01Handle, osWaitForever);
    // TODO Draw specific symbol / word

    // TODO Mutex off LCD
    osMutexRelease(myMutex01Handle);

    return 0;
}
int draw_down(int index) {
    // TODO Mutex on button
    osMutexWait(button_Handle, osWaitForever);
    int x_min = Button_list[index].position[0];
    int x_max = Button_list[index].position[1];
    int y_min = Button_list[index].position[2];
    int y_max = Button_list[index].position[3];
    osMutexRelease(button_Handle);
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = ((x_max - x_min) / 2) + x_min;
    int y_cen = ((y_max - y_min) / 2) + y_min;

    // TODO Mutex on LCD
    osMutexWait(myMutex01Handle, osWaitForever);
    // TODO Draw specific symbol / word

    // TODO Mutex off LCD
    osMutexRelease(myMutex01Handle);

    return 0;
}
