#include "Ass-03-Button.h"
#include "Ass-03.h"

void button_init() {

    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    if (Populate_Button_Position() == 0) {
        safe_printf("Initilised Button Positions\n");
    }
    if (Draw_Button_list() == 0) {
        safe_printf("Initilised Button List\n");
    }
    if (Draw_Button_boxes() == 0) {
        safe_printf("Initilised Button Boxes\n");
    }
}

void pbutton_init() {

    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    if (Draw_Popup_list() == 0) {
        safe_printf("Initilised Popup\n");
    }
    if (Draw_Popup_boxes() == 0) {
        safe_printf("Initilised Boxes\n");
    }
}

// clang-format off
button_s Button_list[] = {
    {"play",      {0,    66,     0,    60},     &draw_play, "play"},
    {"stop",      {0,    66,     60,   120},    &draw_stop, "stop"},
    {"save",      {0,    66,     120,  180},    &draw_blist_item, "save"},
    {"load",      {0,    66,     180,  240},    &draw_blist_item, "load"},
    {"zoom_in",   {66,   129,    120,  180},    &draw_blist_item, "+"},
    {"zoom_out",  {129,  192,    120,  180},    &draw_blist_item, "-"},
    {"reset",     {192,  255,    120,  180},    &draw_blist_item, "x"},
    {"up",        {255,  320,    120,  180},    &draw_blist_item, "up"},
    {"down",      {255,  320,    180,  240},    &draw_blist_item, "down"},
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
    int line_width    = 2;
    int display_x     = BSP_LCD_GetXSize();
    int display_y     = BSP_LCD_GetYSize();
    int window_width  = Get_Window_Width();
    int window_height = Get_Window_Height();
    // Play, stop, save, load xpos
    for (int i = 0; i < 4; i++) {
        Button_list[i].position[0] = line_width;
        Button_list[i].position[1] = display_x - window_width - 2 * line_width;
    }
    // Play ypos
    Button_list[0].position[2] = line_width;
    Button_list[0].position[3] = (window_height - line_width) / 2 + line_width;
    // Stop ypos
    Button_list[1].position[2] = Button_list[0].position[3] + line_width;
    Button_list[1].position[3] = window_height + line_width;
    // Save ypos
    Button_list[2].position[2] = Button_list[1].position[3] + line_width;
    Button_list[2].position[3] =
        ((display_y - Button_list[2].position[2] - 2 * line_width) / 2) + Button_list[2].position[2];
    // Load ypos
    Button_list[3].position[2] = Button_list[2].position[3] + line_width;
    Button_list[3].position[3] = display_y - line_width;
    // Zoom in xpos
    Button_list[4].position[0] = Button_list[0].position[1] + line_width;
    Button_list[4].position[1] =
        (Button_list[7].position[0] - line_width - Button_list[4].position[0] - 2 * line_width) / 3
        + Button_list[4].position[0];
    // Zoom out xpos
    Button_list[5].position[0] = Button_list[4].position[1] + line_width;
    Button_list[5].position[1] =
        (Button_list[7].position[0] - line_width - Button_list[4].position[0] - 2 * line_width) / 3
        + Button_list[5].position[0];
    ;
    // Reset xpos
    Button_list[6].position[0] = Button_list[5].position[1] + line_width;
    Button_list[6].position[1] = Button_list[7].position[0] - line_width;
    // Zoom in, zoom out, reset ypos
    for (int i = 0; i < 3; i++) {
        Button_list[4 + i].position[2] = Button_list[2].position[2];
        Button_list[4 + i].position[3] = Button_list[2].position[3];
    }
    // Up ypos
    Button_list[7].position[2] = Button_list[2].position[2];
    Button_list[7].position[3] = Button_list[2].position[3];
    // Down ypos
    Button_list[8].position[2] = Button_list[3].position[2];
    Button_list[8].position[3] = Button_list[3].position[3];
    // Up, down xpos
    for (int i = 0; i < 2; i++) {
        Button_list[7 + i].position[1] = display_x - line_width;
        Button_list[7 + i].position[0] =
            Button_list[7 + i].position[1] - (Button_list[0].position[1] - Button_list[0].position[0]);
    }

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
    int x_min  = Button_list[index].position[0];
    int x_max  = Button_list[index].position[1];
    int y_min  = Button_list[index].position[2];
    int y_max  = Button_list[index].position[3];
    char* item = Button_list[index].Symbol;

    int x_cen = ((x_max - x_min) / 2) + x_min;
    int y_cen = ((y_max - y_min) / 2) + y_min;

    // safe_printf("x y pos of %s is %d, %d\n", item, x_cen, y_cen);
    osMutexWait(myMutex01Handle, osWaitForever);
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) item, CENTER_MODE);
    osMutexRelease(myMutex01Handle);

    return 0;
}

int draw_plist_item(int index) {
    int x_min  = Popup_list[index].position[0];
    int x_max  = Popup_list[index].position[1];
    int y_min  = Popup_list[index].position[2];
    int y_max  = Popup_list[index].position[3];
    char* item = Popup_list[index].Symbol;

    int x_cen = ((x_max - x_min) / 2) + x_min;
    int y_cen = ((y_max - y_min) / 2) + y_min;

    osMutexWait(myMutex01Handle, osWaitForever);
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) item, CENTER_MODE);
    osMutexRelease(myMutex01Handle);

    return 0;
}

int draw_play(int index) {
    int x_min = Button_list[index].position[0];
    int x_max = Button_list[index].position[1];
    int y_min = Button_list[index].position[2];
    int y_max = Button_list[index].position[3];

    int x_cen = ((x_max - x_min) / 2) + x_min;  // - (SYMBOL_SIZE / 2);
    // safe_printf("x_min %d, x_max %d, x_cen %d\n", x_min, x_max, x_cen);
    int y_cen = ((y_max - y_min) / 2) + y_min - (SYMBOL_SIZE / 2);

    osMutexWait(myMutex01Handle, osWaitForever);
    int chg_dir = 0;
    int width   = 0;
    for (int row = 0; row < SYMBOL_SIZE - 1; row++) {
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        if ((width < SYMBOL_SIZE) && chg_dir == 0) {
            width += 2;
        }
        else {
            chg_dir = 1;
            width -= 2;
        }
        // safe_printf("Width %d, row %d", width, row);
        BSP_LCD_DrawHLine(x_cen - (SYMBOL_SIZE / 2), y_cen + row, 1);
        // safe_printf(" Black Line x %d, y %d, len %d", x_cen - (SYMBOL_SIZE / 2), y_cen + row, 1);

        if (width > 2) {
            BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
            BSP_LCD_DrawHLine(x_cen - (SYMBOL_SIZE / 2) + 1, y_cen + row, width - 2);
            // safe_printf(" Green Line x %d, y %d, len %d", x_cen - (SYMBOL_SIZE / 2) + 1, y_cen + row, width - 2);
        }
        if (width > 1) {
            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            BSP_LCD_DrawHLine(x_cen - (SYMBOL_SIZE / 2) + 1 + width - 1, y_cen + row, 1);
            // safe_printf(" Black Line x %d, y %d, len %d", width + 1, y_cen + row, 1);
        }
        safe_printf("\n");
    }
    osMutexRelease(myMutex01Handle);

    return 0;
}

int draw_stop(int index) {
    int x_min = Button_list[index].position[0];
    int x_max = Button_list[index].position[1];
    int y_min = Button_list[index].position[2];
    int y_max = Button_list[index].position[3];

    int x_cen = ((x_max - x_min) / 2) + x_min;  // - (SYMBOL_SIZE / 2);
    int y_cen = ((y_max - y_min) / 2) + y_min - (SYMBOL_SIZE / 2);

    osMutexWait(myMutex01Handle, osWaitForever);
    for (int row = 0; row < SYMBOL_SIZE - 1; row++) {
        // If it's the first row, or the last row
        // Only print black pixels
        if (row == 0 || row == SYMBOL_SIZE - 2) {
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
            BSP_LCD_DrawHLine(x_cen - (SYMBOL_SIZE / 2) + SYMBOL_SIZE - 1, y_cen + row, 1);
        }
    }
    osMutexRelease(myMutex01Handle);

    return 0;
}

int draw_up(int index) {
    osMutexWait(button_Handle, osWaitForever);
    int x_min = Button_list[index].position[0];
    int x_max = Button_list[index].position[1];
    int y_min = Button_list[index].position[2];
    int y_max = Button_list[index].position[3];
    osMutexRelease(button_Handle);

    int x_cen = ((x_max - x_min) / 2) + x_min;
    int y_cen = ((y_max - y_min) / 2) + y_min;

    osMutexWait(myMutex01Handle, osWaitForever);

    osMutexRelease(myMutex01Handle);

    return 0;
}
int draw_down(int index) {
    osMutexWait(button_Handle, osWaitForever);
    int x_min = Button_list[index].position[0];
    int x_max = Button_list[index].position[1];
    int y_min = Button_list[index].position[2];
    int y_max = Button_list[index].position[3];
    osMutexRelease(button_Handle);

    int x_cen = ((x_max - x_min) / 2) + x_min;
    int y_cen = ((y_max - y_min) / 2) + y_min;

    osMutexWait(myMutex01Handle, osWaitForever);

    osMutexRelease(myMutex01Handle);

    return 0;
}
