#include "Ass-03-Button.h"
#include "Ass-03.h"

void buttonInit() {

    // TODO Populate the position of each button
}


typedef struct {
    char* NameString;
    int position[4];
    int (*Function_p)(void);
} button_s;
button_s button;

// clang-format off
button_s Button_list[] = {
    {"play",      {0,    0,    0,    0},    &draw_play,},
    {"stop",      {0,    0,    0,    0},    &draw_stop,},
    {"save",      {0,    0,    0,    0},    &draw_save,},
    {"load",      {0,    0,    0,    0},    &draw_load,},
    {"zoom_in",   {0,    0,    0,    0},    &draw_zoom_in,},
    {"zoom_out",  {0,    0,    0,    0},    &draw_zoom_out,},
    {"reset",     {0,    0,    0,    0},    &draw_reset,},
    {"up",        {0,    0,    0,    0},    &draw_up,},
    {"down",      {0,    0,    0,    0},    &draw_down,},
    {NULL,        {NULL, NULL, NULL, NULL}, NULL}};
// clang-format on

// clang-format off
button_s Popup_list[] = {
    {"new",       {0,    0,    0,    0},    &draw_new,},
    {"overwrite", {0,    0,    0,    0},    &draw_overwrite,},
    {"cancel",    {0,    0,    0,    0},    &draw_cancel,},
    {NULL,        {NULL, NULL, NULL, NULL}, NULL}};
// clang-format on

// If a command is called function decides which function pointer to return
int Draw_Button_list() {
    int i = 0;
    // While we haven't checked the whole list
    while (Command_list[i].NameString != NULL) {
        // If we find the function we want, call it
        if (Command_list[i].Function_p() == 0) {
            return 0;
        }
        else {
            return -1;
        }
        i++;
    }
    return -1;
}

int draw_play() {
    // TODO Mutex on button
    x_min = Button_list[].position[0];
    x_max = Button_list[].position[1];
    y_min = Button_list[].position[2];
    y_max = Button_list[].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    // BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) "play", CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_stop() {
    // TODO Mutex on button
    x_min = Button_list[].position[0];
    x_max = Button_list[].position[1];
    y_min = Button_list[].position[2];
    y_max = Button_list[].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    // BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) "stop", CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_save() {
    // TODO Mutex on button
    x_min = Button_list[].position[0];
    x_max = Button_list[].position[1];
    y_min = Button_list[].position[2];
    y_max = Button_list[].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) "save", CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_load() {
    // TODO Mutex on button
    x_min = Button_list[].position[0];
    x_max = Button_list[].position[1];
    y_min = Button_list[].position[2];
    y_max = Button_list[].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) "load", CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_zoom_in() {
    // TODO Mutex on button
    x_min = Button_list[].position[0];
    x_max = Button_list[].position[1];
    y_min = Button_list[].position[2];
    y_max = Button_list[].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) "zoom_in", CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_zoom_out() {
    // TODO Mutex on button
    x_min = Button_list[].position[0];
    x_max = Button_list[].position[1];
    y_min = Button_list[].position[2];
    y_max = Button_list[].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) "zoom_out", CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_reset() {
    // TODO Mutex on button
    x_min = Button_list[].position[0];
    x_max = Button_list[].position[1];
    y_min = Button_list[].position[2];
    y_max = Button_list[].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) "reset", CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_up() {
    // TODO Mutex on button
    x_min = Button_list[].position[0];
    x_max = Button_list[].position[1];
    y_min = Button_list[].position[2];
    y_max = Button_list[].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) "up", CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_down() {
    // TODO Mutex on button
    x_min = Button_list[].position[0];
    x_max = Button_list[].position[1];
    y_min = Button_list[].position[2];
    y_max = Button_list[].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) "down", CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_new() {
    // TODO Mutex on button
    x_min = Popup_list[].position[0];
    x_max = Popup_list[].position[1];
    y_min = Popup_list[].position[2];
    y_max = Popup_list[].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) "new", CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_overwrite() {
    // TODO Mutex on button
    x_min = Popup_list[].position[0];
    x_max = Popup_list[].position[1];
    y_min = Popup_list[].position[2];
    y_max = Popup_list[].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) "overwrite", CENTER_MODE);
    // TODO Mutex off LCD
}
int draw_cancel() {
    // TODO Mutex on button
    x_min = Popup_list[].position[0];
    x_max = Popup_list[].position[1];
    y_min = Popup_list[].position[2];
    y_max = Popup_list[].position[3];
    // TODO Mutex off button

    // TODO Calculate the position to draw the item
    int x_cen = x_max - x_min;
    int y_cen = y_max - y_min;

    // TODO Mutex on LCD
    // TODO Draw specific symbol / word
    BSP_LCD_DisplayStringAt(x_cen, y_cen, (uint8_t*) "cancel", CENTER_MODE);
    // TODO Mutex off LCD
}