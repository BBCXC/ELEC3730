#include "Ass-03-Button.h"
#include "Ass-03.h"

void buttonInit() {

    // TODO Populate the position of each button
}


typedef struct {
    char* NameString;
    int position[4];
    int (*Function_p)(int index);
    char* Symbol;
} button_s;
button_s button;

// clang-format off
button_s Button_list[] = {
    {"play",      {0,    0,    0,    0},    &draw_play,       ""},
    {"stop",      {0,    0,    0,    0},    &draw_stop,       ""},
    {"save",      {0,    0,    0,    0},    &draw_blist_item, "save"},
    {"load",      {0,    0,    0,    0},    &draw_blist_item, "load"},
    {"zoom_in",   {0,    0,    0,    0},    &draw_blist_item, "+"},
    {"zoom_out",  {0,    0,    0,    0},    &draw_blist_item, "-"},
    {"reset",     {0,    0,    0,    0},    &draw_blist_item, "x"},
    {"up",        {0,    0,    0,    0},    &draw_up,         ""},
    {"down",      {0,    0,    0,    0},    &draw_down,       ""},
    {NULL,        {NULL, NULL, NULL, NULL}, NULL,             NULL}};
// clang-format on

// clang-format off
button_s Popup_list[] = {
    {"new",       {0,    0,    0,    0},    &draw_plist_item, "new"},
    {"overwrite", {0,    0,    0,    0},    &draw_plist_item, "overwrite"},
    {"cancel",    {0,    0,    0,    0},    &draw_plist_item, "cancel"},
    {NULL,        {NULL, NULL, NULL, NULL}, NULL,             NULL}};
// clang-format on

// Return the item touched on the screen
int get_touch_pos(int display_x, int display_y, int popup) {
    if (popup == 0) {
        while (Button_list[i].NameString != NULL) {
            if ((display_x >= Button_list[i].position[0]) && (display_x <= Button_list[i].position[1])
                && (display_y >= Button_list[i].position[2]) && (display_y <= Button_list[i].position[3])) {
                return ();
            }
        }
    }
    else if (popup == 1) {
        while (Popup_list[i].NameString != NULL) {
            if ((display_x >= Popup_list[i].position[0]) && (display_x <= Popup_list[i].position[1])
                && (display_y >= Popup_list[i].position[2]) && (display_y <= Popup_list[i].position[3])) {
                return ();
            }
        }
    }
    return NULL;
}

// If a command is called function decides which function pointer to return
int Draw_Button_list() {
    int i = 0;
    // While we haven't checked the whole list
    while (Button_list[i].NameString != NULL) {
        // If we find the function we want, call it
        // Draws all of the buttons
        if (Button_list[i].Function_p(i) == 0) {
            return 0;
        }
        else {
            return -1;
        }
        i++;
    }
    return -1;
}

// If a command is called function decides which function pointer to return
int Draw_Popup_list() {
    int i = 0;
    // While we haven't checked the whole list
    while (Popup_list[i].NameString != NULL) {
        // If we find the function we want, call it
        // Draws all of the popup menu
        if (Popup_list[i].Function_p(i) == 0) {
            return 0;
        }
        else {
            return -1;
        }
        i++;
    }
    return -1;
}

int Draw_Popup_Window() {

    // TODO Draw Rectangle

    // TODO Write current time stamp in position

    if (Draw_Popup_list() == 0) {
        safe_printf("%sERROR:%s Popup buttons failed to draw\n", ERROR_M, DEFAULT_COLOUR_M);
    }
}

int draw_blist_item(index) {
    // TODO Mutex on button
    int x_min  = Button_list[index].position[0];
    int x_max  = Button_list[index].position[1];
    int y_min  = Button_list[index].position[2];
    int y_max  = Button_list[index].position[3];
    char* item = Button_list[index].Symbol;
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) item, CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_plist_item(index) {
    // TODO Mutex on button
    int x_min  = Popup_list[index].position[0];
    int x_max  = Popup_list[index].position[1];
    int y_min  = Popup_list[index].position[2];
    int y_max  = Popup_list[index].position[3];
    char* item = Popup_list[index].Symbol;
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) item, CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_play(index) {
    // TODO Mutex on button
    int x_min = Button_list[index].position[0];
    int x_max = Button_list[index].position[1];
    int y_min = Button_list[index].position[2];
    int y_max = Button_list[index].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word

    // TODO Mutex off LCD
}
int draw_stop(index) {
    // TODO Mutex on button
    int x_min = Button_list[index].position[0];
    int x_max = Button_list[index].position[1];
    int y_min = Button_list[index].position[2];
    int y_max = Button_list[index].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word

    // TODO Mutex off LCD
}
int draw_up(index) {
    // TODO Mutex on button
    int x_min = Button_list[index].position[0];
    int x_max = Button_list[index].position[1];
    int y_min = Button_list[index].position[2];
    int y_max = Button_list[index].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word

    // TODO Mutex off LCD
}
int draw_down(index) {
    // TODO Mutex on button
    int x_min = Button_list[index].position[0];
    int x_max = Button_list[index].position[1];
    int y_min = Button_list[index].position[2];
    int y_max = Button_list[index].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word

    // TODO Mutex off LCD
}