#include "Ass-02-Screen.h"
#include "Ass-02.h"

void Screen_Init(void) {
  static char *item[42] = {
      "=",  "+",     "ANS",   ".",     "0",    ">",   "-",     "3",    "2",
      "1",  "<",     "/",     "6",     "5",    "4",   "AC",    "x",    "9",
      "8",  "7",     "DEL",   "=",     "(",    "ANS", "sqrt(", "^",    ">",
      ")",  "atan(", "acos(", "asin(", "<",    "pi",  "tan(",  "cos(", "sin(",
      "AC", "",      "log(",  "ln(",   "exp(", "DEL"};
  // Store that array of strings in the struct
  grid_space_p.items = item;
}

// Return the value x_min x_max y_min y_max of the given button
// Pos1 button number
// Pos2 parameter x_min x_max y_min y_max
int Get_Area(int pos1, int pos2) { return (grid_space_p.Area[pos1][pos2]); }

// Return the item number of the button given
char *Get_Item(int Item_Value) { return (grid_space_p.items[Item_Value]); }

// Set the value of the given button and given parameter to value
// Pos1 button number
// Pos2 parameter x_min x_max y_min y_max
void Set_Area(int pos1, int pos2, int value) {
  grid_space_p.Area[pos1][pos2] = value;
}

// Set the button item to Value
void Set_Item(int pos, int Item_number) {
  grid_space_p.items[pos] = Item_number;
}

// Return the item touched on the screen
int get_touch_pos(int display_x, int display_y) {
  for (int i = 0; i < 21; i++) {
    if ((display_x >= grid_space_p.Area[i][0]) &&
        (display_x <= grid_space_p.Area[i][1]) &&
        (display_y >= grid_space_p.Area[i][2]) &&
        (display_y <= grid_space_p.Area[i][3])) {
      return (grid_space_p.Area[i][4]);
    }
  }
  // Dummy variable if out of range
  return 100;
}

// Draw specific item in a specific cell
int draw_item(int cell_number, int offset, int text_colour, int cell_colour) {
  // Cell_number : 0 to 21
  // Offset : 0 or 21, depending on symbol or number screen

  // Draws specific character passed into function
  int x_min = grid_space_p.Area[cell_number][0];
  int x_max = grid_space_p.Area[cell_number][1];
  int y_min = grid_space_p.Area[cell_number][2];
  int y_max = grid_space_p.Area[cell_number][3];

  if (LCD_Cell_Colour(x_min, x_max, y_min, y_max, text_colour, cell_colour) !=
      0) {
    printf("%sERROR:%s Could not clear cell\n", ERROR_M, DEFAULT_COLOUR_M);
  }
  // Find center of cell given
  int x_pos = ((x_max - x_min) / 2.0) + x_min;
  int y_pos = ((y_max - y_min) / 2.0) + y_min;

  BSP_LCD_SetFont(&Font16);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(x_pos, y_pos,
                          (uint8_t *)grid_space_p.items[cell_number + offset],
                          CENTER_MODE);

  return 0;
}
