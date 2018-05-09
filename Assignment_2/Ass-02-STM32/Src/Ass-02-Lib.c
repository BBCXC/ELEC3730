/*
 * Author      : Mitchell Marotta C3258958
 *               Taylor Young C3206230
 * Date        : 4 May 2018
 * Description : Assignment 2
 *               Code common to Q1 and Q2
 */

#include "Ass-02.h"

// List of function names, pointers to that function, method of use, description
// of function
const animation_s Title_Animation[] = {
    {LCD_COLOR_BLUE},         {LCD_COLOR_GREEN},
    {LCD_COLOR_RED},          {LCD_COLOR_CYAN},
    {LCD_COLOR_MAGENTA},      {LCD_COLOR_YELLOW},
    {LCD_COLOR_LIGHTBLUE},    {LCD_COLOR_LIGHTGREEN},
    {LCD_COLOR_LIGHTRED},     {LCD_COLOR_LIGHTCYAN},
    {LCD_COLOR_LIGHTMAGENTA}, {LCD_COLOR_LIGHTYELLOW},
    {LCD_COLOR_DARKBLUE},     {LCD_COLOR_DARKGREEN},
    {LCD_COLOR_DARKRED},      {LCD_COLOR_DARKCYAN},
    {LCD_COLOR_DARKMAGENTA},  {LCD_COLOR_DARKYELLOW},
    {LCD_COLOR_WHITE},        {LCD_COLOR_LIGHTGRAY},
    {LCD_COLOR_GRAY},         {LCD_COLOR_DARKGRAY},
    {LCD_COLOR_BLACK},        {LCD_COLOR_BROWN},
    {LCD_COLOR_ORANGE},       {NULL}};

// clang-format off
const command_s Command_list[] = {
    {"add",     &add_function,     "add <num 1> . + . <num N>", "Add one or more numbers"},
    {"sub",     &sub_function,     "sub <num 1> - <num 2>",     "Subtract two numbers"},
    {"mul",     &mul_function,     "mul <num 1> . * . <num N>", "Multiply one or more numbers"},
    {"div",     &div_function,     "div <num 1> / <num 2>",     "Divide two numbers"},
    {"sin",     &sin_function,     "sin <num 1> <rad|deg>",     "sin number in radians or degrees"},
    {"cos",     &cos_function,     "cos <num 1> <rad|deg>",     "cos number in radians or degrees"},
    {"tan",     &tan_function,     "tan <num 1> <rad|deg>",     "tan number in radians or degrees"},
    {"asin",    &asin_function,    "asin <num 1> <rad|deg>",    "asin number in radians or degrees"},
    {"acos",    &acos_function,    "acos <num 1> <rad|deg>",    "acos number in radians or degrees"},
    {"atan",    &atan_function,    "atan <num 1> <rad|deg>",    "atan number in radians or degrees"},
    {"pow",     &pow_function,     "pow <num 1> ^ <num 2>",     "Power base, exponent"},
    {"sqrt",    &sqrt_function,    "sqrt <num 1>",              "Add one or more numbers"},
    {"ln",      &ln_function,      "ln <num 1>",                "Add one or more numbers"},
    {"log",     &log_function,     "log <num 1>",               "Add one or more numbers"},
    {"exp",     &exp_function,     "exp <num 1>",               "Add one or more numbers"},
    {"formula", &formula_function, "formula <on|off>",          "Allows Formula Input"},
    {"graph",   &graph_function,   "graph <on|off>",            "Allows graph output on LCD"},
    {"debug",   &debug_function,   "debug <on|off>",            "Display debug messages"},
    {"system",  &system_function,  "system <on|off>",           "Display system messages"},
    {"clear",   &clear_function,   "reset",                     "Clear terminal screen"},
    {"reset",   &reset_function,   "clear",                     "Reset terminal scrollback"},
    {"help",    &help_function,    "help [command]",            "Display help messages"},
    {NULL,      NULL,              NULL,                        NULL}};

// clang-format on

// If a command is called function decides which function pointer to return
int command_parser(char **array_of_words_p[], int word_count,
                   double *prev_ans) {
  double result = 0;
  int i = 0;

  // While we haven't checked the whole list
  while (Command_list[i].NameString != NULL) {
    // If we find the function we want, call it
    if (strcmp((*array_of_words_p)[0], Command_list[i].NameString) == 0) {
      if (Get_Debug() == 1)
        printf("Operation: %s\n", Command_list[i].NameString);
      // If the function we wanted returned success
      if (Command_list[i].Function_p(array_of_words_p, word_count, &result) ==
          0) {
        Set_Result(result);
        *prev_ans = result;
        if (i <= 14)
          return 0;
        else
          return 1;
      } else {
        return -1;
      }
    }
    i++;
  }
  return -1;
}

// If help is called
// Function decides if specific help is to be diplayed or all
int help_parser(char **array_of_words_p[], int word_count) {
  int i = 0;

  printf("%s" HELP_M
         "%s"
         "\n",
         KGRN, "Format", "Description", KNRM);
  if (word_count > 1) {
    // While we haven't checked the whole list
    while (Command_list[i].NameString != NULL) {
      // If we find the function we wanted, display the help message
      if (strcmp((*array_of_words_p)[1], Command_list[i].NameString) == 0) {
        if (Get_Debug() == 1)
          printf("Operation: %s\n", Command_list[i].NameString);
        // Print Specific command list thing
        printf(HELP_M "\n", Command_list[i].HelpString,
               Command_list[i].DescriptionString);
        return 0;
      }
      i++;
    }
  } else if (word_count == 1) {
    if (Get_Debug() == 1)
      printf("%sDEBUG_INFO:%s No Operation selected\n", DEBUG_M,
             DEFAULT_COLOUR_M);
    // Loop through all command list and print
    while (Command_list[i].NameString != NULL) {
      printf(HELP_M "\n", Command_list[i].HelpString,
             Command_list[i].DescriptionString);
      i++;
    }
    return 0;
  }
  return 1;
}

// clang-format off
/**********************************************************************************************************************
************************************************Recursive Desent Parser************************************************
**********************************************************************************************************************/
// clang-format on

// Takes formula string
// Calls next precedent
// Returns answer at end
double parseFormula() {
  if (Get_System() == 1)
    printf("%sSYSTEM_INFO:%s Formula Parsed: %s\n", SYS_M, DEFAULT_COLOUR_M,
           output.formula);
  output.result = parseSub();
  if (*output.formula == '\0') {
    // return output.result;
    return 0;
  }
  printf("Expected end of output.input but found %c\n", *output.formula);
  printf("Syntax Error\n");
  return 1;
}

// Calls next precedent
// Calculates relative function
// Returns answer at end
double parseSub() {
  double sub_1 = parseSum();
  while (*output.formula == '-') {
    ++output.formula;
    double sub_2 = parseSum();
    sub_1 = sub_1 - sub_2;
  }
  return sub_1;
}

// Calls next precedent
// Calculates relative function
// Returns answer at end
double parseSum() {
  double sum_1 = parsePro();
  while (*output.formula == '+') {
    ++output.formula;
    double sum_2 = parsePro();
    sum_1 = sum_1 + sum_2;
  }
  return sum_1;
}

// Calls next precedent
// Calculates relative function
// Returns answer at end
double parsePro() {
  double pro_1 = parseDiv();
  while (*output.formula == 'x' || *output.formula == '*') {
    ++output.formula;
    double pro_2 = parseDiv();
    pro_1 = pro_1 * pro_2;
  }
  return pro_1;
}

// Calls next precedent
// Calculates relative function
// Returns answer at end
double parseDiv() {
  double div_1 = parsePow();
  while (*output.formula == '/') {
    ++output.formula;
    double div_2 = parsePow();
    div_1 = div_1 / div_2;
  }
  return div_1;
}

// Calls next precedent
// Calculates relative function
// Returns answer at end
double parsePow() {
  double pow_1 = parseFactor();
  while (*output.formula == '^') {
    ++output.formula;
    double pow_2 = parseFactor();
    pow_1 = pow(pow_1, pow_2);
  }
  return pow_1;
}

// Decides whether next factor is a
// Number
// Word operation
// Bracket
double parseFactor() {
  if (*output.formula >= '0' && *output.formula <= '9') {
    return parseNumber();
  } else if (*output.formula == '-') {
    return parseNumber();
  } else if (*output.formula == '+') {
    return parseNumber();
  } else if (*output.formula == '(') {
    ++output.formula;
    double temp = parseSub();
    ++output.formula;
    return temp;
  }
  // PI
  else if (*output.formula == 'p') {
    ++output.formula;
    if (*output.formula == 'i') {
      ++output.formula;
      return M_PI;
    }
  }
  // sin sqrt
  else if (*output.formula == 's') {
    ++output.formula;
    if (*output.formula == 'i') {
      ++output.formula;
      if (*output.formula == 'n') {
        ++output.formula;
        if (*output.formula == '(') {
          ++output.formula;
          double temp = parseSub();
          temp = sin(temp * M_PI / 180);
          ++output.formula;
          return temp;
        }
      }
    } else if (*output.formula == 'q') {
      ++output.formula;
      if (*output.formula == 'r') {
        ++output.formula;
        if (*output.formula == 't') {
          ++output.formula;
          if (*output.formula == '(') {
            ++output.formula;
            double temp = parseSub();
            temp = sqrt(temp);
            ++output.formula;
            return temp;
          }
        }
      }
    }
  }
  // cos
  else if (*output.formula == 'c') {
    ++output.formula;
    if (*output.formula == 'o') {
      ++output.formula;
      if (*output.formula == 's') {
        ++output.formula;
        if (*output.formula == '(') {
          ++output.formula;
          double temp = parseSub();
          temp = cos(temp * M_PI / 180);
          ++output.formula;
          return temp;
        }
      }
    }
  }
  // tan
  else if (*output.formula == 't') {
    ++output.formula;
    if (*output.formula == 'a') {
      ++output.formula;
      if (*output.formula == 'n') {
        ++output.formula;
        if (*output.formula == '(') {
          ++output.formula;
          double temp = parseSub();
          temp = tan(temp * M_PI / 180);
          ++output.formula;
          return temp;
        }
      }
    }
  }
  // asin acos atan
  else if (*output.formula == 'a') {
    ++output.formula;
    if (*output.formula == 's') {
      ++output.formula;
      if (*output.formula == 'i') {
        ++output.formula;
        if (*output.formula == 'n') {
          ++output.formula;
          if (*output.formula == '(') {
            ++output.formula;
            double temp = parseSub();
            temp = asin(temp * M_PI / 180);
            ++output.formula;
            return temp;
          }
        }
      }
    } else if (*output.formula == 'c') {
      ++output.formula;
      if (*output.formula == 'o') {
        ++output.formula;
        if (*output.formula == 's') {
          ++output.formula;
          if (*output.formula == '(') {
            ++output.formula;
            double temp = parseSub();
            temp = acos(temp * M_PI / 180);
            ++output.formula;
            return temp;
          }
        }
      }
    } else if (*output.formula == 't') {
      ++output.formula;
      if (*output.formula == 'a') {
        ++output.formula;
        if (*output.formula == 'n') {
          ++output.formula;
          if (*output.formula == '(') {
            ++output.formula;
            double temp = parseSub();
            temp = atan(temp * M_PI / 180);
            ++output.formula;
            return temp;
          }
        }
      }
    } else if (*output.formula == 'n') {
      ++output.formula;
      if (*output.formula == 's') {
        ++output.formula;
        double temp = output.prev_ans;
        return temp;
      }
    }
  }
  // exp
  else if (*output.formula == 'e') {
    ++output.formula;
    if (*output.formula == 'x') {
      ++output.formula;
      if (*output.formula == 'p') {
        ++output.formula;
        if (*output.formula == '(') {
          ++output.formula;
          double temp = parseSub();
          temp = exp(temp);
          ++output.formula;
          return temp;
        }
      }
    }
  }
  // ln log10
  else if (*output.formula == 'l') {
    ++output.formula;
    if (*output.formula == 'n') {
      ++output.formula;
      if (*output.formula == '(') {
        ++output.formula;
        double temp = parseSub();
        temp = log(temp);
        ++output.formula;
        return temp;
      }
    } else if (*output.formula == 'o') {
      ++output.formula;
      if (*output.formula == 'g') {
        ++output.formula;
        if (*output.formula == '(') {
          ++output.formula;
          double temp = parseSub();
          temp = log10(temp);
          ++output.formula;
          return temp;
        }
      }
    }
  } else if (Get_Graph_Mode() == 1) {
    if (*output.formula == 'X') {
      ++output.formula;
      return (Get_Graph_Increment());
    }
  } else {
    printf("Syntax Error\n");
    printf("Unknown symbol %c\n", *output.formula);
  }
  return 0;
}

// If the current factor is a number
// Decide if it is nugative or positive
// Find any decimals
// Return number as double
double parseNumber() {
  double number = 0;
  int neg_flag = 1;
  if (*output.formula >= '0' && *output.formula <= '9') {
  } else if (*output.formula == '-') {
    neg_flag = -1;
    ++output.formula;
  } else if (*output.formula == '+') {
    neg_flag = 1;
    ++output.formula;
  } else {
    printf("Syntax Error\n");
  }

  while (*output.formula >= '0' && *output.formula <= '9') {
    number = number * 10;
    number = number + (int)(*output.formula - '0');
    ++output.formula;
  }

  if (*output.formula == '.') {
    ++output.formula;

    // Check the next character is a number, else error

    if (*output.formula >= '0' && *output.formula <= '9') {
      double weight = 1;
      while (*output.formula >= '0' && *output.formula <= '9') {
        weight = weight / 10.0;
        double scaled = (int)(*output.formula - '0') * weight;
        number = number + scaled;
        ++output.formula;
      }
    } else {
      printf("Syntax Error\n");
    }
  }
  if (Get_System() == 1)
    printf("%sSYSTEM_INFO:%s Number Found: %lf\n", SYS_M, DEFAULT_COLOUR_M,
           (number * neg_flag));
  return (number * neg_flag);
}

// clang-format off
/***********************************************************************************************************************
 ****************************************************Title Animation****************************************************
 **********************************************************************************************************************/
// clang-format on

int title_animation() {
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  int j = 0;
  static int button_debounce = 0;
  static int off_debounce = 0;

  // Increment through the lst of colours
  while (Title_Animation[j].BGColour != NULL) {
    BSP_LCD_Clear(Title_Animation[j].BGColour);
    for (int i = 0; i < 1500; i++) {
      // Break if the user touches the screen
      if (BSP_TP_GetDisplayPoint(&display) == 0) {
        button_debounce++;
        if (button_debounce >= 50) {
          BSP_LCD_Clear(LCD_COLOR_WHITE);
          return 0;
        }
      } else {
        off_debounce++;
        // User is definately not pressing a button, reset the holding flag
        if (off_debounce > 100) {
          button_debounce = 0;
          off_debounce = 0;
        }
      }
    }
    j++;
  }

  BSP_LCD_Clear(LCD_COLOR_WHITE);

  return 0;
}

// clang-format off
/***********************************************************************************************************************
 *********************************************************Other*********************************************************
 **********************************************************************************************************************/
// clang-format on

// STEPIEN: Added two touch panel functions to make the interface more
//          consistent with the LCD BSP.

#ifdef STM32F407xx
uint8_t BSP_TP_Init(void) {
  // Initialise the interface and calibrate
  TP_Init();  // This is an empty function since done by STM32CubeMX
  TouchPanel_Calibrate();

  return 0;
}

uint8_t BSP_TP_GetDisplayPoint(Coordinate *pDisplay) {
  Coordinate *pScreen;

  pScreen = Read_Ads7846();
  if (pScreen == NULL) {
    return 1;  // Error reading the touch panel
  }
  if (getDisplayPoint(pDisplay, pScreen, &matrix) == DISABLE) {
    return 1;  // Error in LCD
  }
  return 0;
}
#endif
