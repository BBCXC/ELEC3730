// TODO Implement backspace handler

/*
 * Author      : Mitchell Marotta C3258958
 *               Taylor Young C3206230
 * Date        : 4 May 2018
 * Description : Q1
 *               Command line parser
 *               Works in both windows and in putty
 */

#include "Ass-02.h"
#ifdef STM32F407xx
#include "usart.h"
#endif

// Constant strings used for identifying the function requested
const char *ADDstr = "add";
const char *SUBstr = "sub";
const char *MULstr = "mul";
const char *DIVstr = "div";
const char *SINstr = "sin";
const char *COSstr = "cos";
const char *TANstr = "tan";
const char *ASINstr = "asin";
const char *ACOSstr = "acos";
const char *ATANstr = "atan";
const char *POWstr = "pow";
const char *SQRTstr = "sqrt";
const char *LNstr = "ln";
const char *LOGstr = "log";
const char *EXPstr = "exp";
const char *FORMULAstr = "formula";
const char *DEBUGstr = "debug";
const char *HELPstr = "help";

const char *RADstr = "rad";
const char *DEGstr = "deg";
const char *PIstr = "pi";
const char *ANSstr = "ans";

// clang-format off
/***********************************************************************************************************************
************************************************Command Line Parser Init************************************************
***********************************************************************************************************************/
// clang-format on

void CommandLineParserInit(void) {
  // Print welcome message
  printf(RESET_M);
  printf(CLEAR_M);
  printf(KNRM);
  printf("ELEC3730 Assignment 2\n");
  printf("Command Line Parser\n");
}

// clang-format off
/***********************************************************************************************************************
**********************************************Command Line Parser Process***********************************************
***********************************************************************************************************************/
// clang-format on

void CommandLineParserProcess(void) {
  char c;
  static int i = 0;
  static char command_line[101];

// Check for input and echo back
#ifdef STM32F407xx

  if (Get_First_Time() == 1) {
    Set_First_Time(0);
    printf("--> Enter text: ");
  }
  if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK) {
    printf("%c", c);
    HAL_GPIO_TogglePin(GPIOD, LD4_Pin);  // Toggle LED4

    command_line[i] = c;
    i++;

    // If we get a return character then process the string
    if (c == '\r' || i > 101) {
      printf("\n");
      command_line[i - 1] = '\0';
      if (StringProcess(&command_line, i) != 0)
        printf("%sERROR:%s Could not process string\n", ERROR_M,
               DEFAULT_COLOUR_M);
      i = 0;
      Set_First_Time(1);
    }
  }

#else

  // Get one line of input
  printf("--> Enter text: ");
  i = 0;
  c = getchar();

  // If we get a new line character then process the string
  while (c != '\n' || i < 101) {
    command_line[i] = c;
    i++;
    c = getchar();
  }
  command_line[i] = 0;
  if (StringProcess(&command_line, i) != 0)
    printf("%sERROR:%s Could not process string\n", ERROR_M, DEFAULT_COLOUR_M);
  i = 0;

#endif
}

int StringProcess(char *command_line, int i) {
  static double prev_ans = 0;
  char **array_of_words_p;

  int word_count = string_parser(command_line, &array_of_words_p);

  if (Get_System() == 1) {
    for (int i = 0; i < word_count; i++) {
      printf("%sSYSTEM_INFO:%s Word %i: %s\n", SYS_M, DEFAULT_COLOUR_M, i,
             array_of_words_p[i]);
    }
  }

  // If words were detected, and we are using the normal mode
  if (word_count > 0 && Get_Formula_Mode() == 0) {
    if (Get_System() == 1)
      printf("%sSYSTEM_INFO:%s Word Count = %i\n", SYS_M, DEFAULT_COLOUR_M,
             word_count);

    char dtos[50];
    // Check the 2 value parameters for pi
    for (int j = 1; j < word_count; j++) {
      if ((strcmp("PI", array_of_words_p[j]) == 0) ||
          (strcmp("pi", array_of_words_p[j]) == 0) ||
          (strcmp("Pi", array_of_words_p[j]) == 0)) {
        if (Get_Debug() == 1)
          printf("%sDEBUG_INFO:%s Constant PI found\n", DEBUG_M,
                 DEFAULT_COLOUR_M);
        snprintf(dtos, 50, "%lf", M_PI);
        array_of_words_p[j] = dtos;
      }
    }

    // Check the 2 value parameters for ans
    for (int j = 1; j < word_count; j++) {
      if ((strcmp("ANS", array_of_words_p[j]) == 0) ||
          (strcmp("ans", array_of_words_p[j]) == 0)) {
        if (Get_Debug() == 1)
          printf("%sDEBUG_INFO:%s Constant ANS found\n", DEBUG_M,
                 DEFAULT_COLOUR_M);
        snprintf(dtos, 50, "%lf", prev_ans);
        array_of_words_p[j] = dtos;
      }
    }

    // Check if command parser returned a valid function
    int mode = -1;
    mode = command_parser(&array_of_words_p, word_count, &prev_ans);
    if (mode == -1) {
      printf("%sERROR:%s Unknown Operation\n", ERROR_M, DEFAULT_COLOUR_M);
    } else if (mode == 0) {
      Set_Result(prev_ans);
      Set_Prev_ans(prev_ans);
      // printf("The result is %s%g%s\n", KGRN, Get_Result(), KNRM);
      printf("Result %g\n", Get_Prev_ans());
      if (Get_Debug() == 1) printf("Ans %g\n", Get_Prev_ans());
    } else {
      // Must have changed debug, formula or help
    }

    free(array_of_words_p[0]);
    free(array_of_words_p);

    if (Get_Debug() == 1)
      printf("%sDEBUG_INFO:%s Arrays have been freed\n", DEBUG_M,
             DEFAULT_COLOUR_M);
  }

  // Formula mode ON
  else if (Get_Formula_Mode() == 1) {
    if (command_parser(&array_of_words_p, word_count, &prev_ans) == 1) {
    } else {
      Set_Formula(array_of_words_p[0]);
      Set_Result(0);

      // Call the recursive decent parser
      if (parseFormula() == 0) {
        Set_Prev_ans(Get_Result());
        printf("The result is %g\n", Get_Result());
        if (Get_Debug() == 1) printf("Answer stored is %lf\n", Get_Prev_ans());
      }
    }

    free(array_of_words_p[0]);
    free(array_of_words_p);

    if (Get_Debug() == 1)
      printf("%sDEBUG_INFO:%s Arrays have been freed\n", DEBUG_M,
             DEFAULT_COLOUR_M);

  } else {
    printf("%sERROR:%s No strings detected\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  return 0;
}

// clang-format off
/***********************************************************************************************************************
**************************************************Operation Function****************************************************
***********************************************************************************************************************/
// clang-format on

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int add_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered ADD function\n", DEBUG_M, DEFAULT_COLOUR_M);
  double value_1 = 0;
  double value_2 = 0;
  // For each number perform the functions calculation
  for (int i = 1; i < word_count; i++) {
    // Read in next parameter, used as number for calculation
    if (sscanf((*array_of_words_p)[i], "%lf", &value_2) != 1) {
      printf("%sERROR:%s Found unknown argument %s\n", ERROR_M,
             DEFAULT_COLOUR_M, (*array_of_words_p)[i]);
      return 1;
    }
    value_1 += value_2;
  }
  *result = value_1;
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int sub_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered SUB function\n", DEBUG_M, DEFAULT_COLOUR_M);
  double value_1 = 0;
  double value_2 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 3) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  // Read in next parameter, used as number for calculation
  if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
    printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  // Read in next parameter, used as number for calculation
  if (sscanf((*array_of_words_p)[2], "%lf", &value_2) != 1) {
    printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  *result = value_1 - value_2;
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int mul_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered MUL function\n", DEBUG_M, DEFAULT_COLOUR_M);
  double value_1 = 1;
  double value_2 = 0;
  // For each number perform the functions calculation
  for (int i = 1; i < word_count; i++) {
    // Read in next parameter, used as number for calculation
    if (sscanf((*array_of_words_p)[i], "%lf", &value_2) != 1) {
      printf("%sERROR:%s Found unknown argument %s\n", ERROR_M,
             DEFAULT_COLOUR_M, (*array_of_words_p)[i]);
      return 1;
    }
    value_1 = value_1 * value_2;
  }
  *result = value_1;
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int div_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered DIV function\n", DEBUG_M, DEFAULT_COLOUR_M);
  double value_1 = 0;
  double value_2 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 3) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  // Read in next parameter, used as number for calculation
  if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
    printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  // Read in next parameter, used as number for calculation
  if (sscanf((*array_of_words_p)[2], "%lf", &value_2) != 1) {
    printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  *result = value_1 / value_2;
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int sin_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered SIN function\n", DEBUG_M, DEFAULT_COLOUR_M);
  double value_1 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 3) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  if (word_count > 1) {
    // Read in next parameter, used as number for calculation
    if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
      printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
      return 1;
    }
  } else {
    printf("%sERROR:%s Insufficient Arguments\n", ERROR_M, DEFAULT_COLOUR_M);
  }
  if (word_count > 2) {
    // If radians don't need to alter number
    if (strcmp(RADstr, (*array_of_words_p)[2]) == 0) {
    }
    // If degrees, number needs to be converted to radians for function
    else if (strcmp(DEGstr, (*array_of_words_p)[2]) == 0) {
      value_1 = value_1 * M_PI / 180;
    }
  } else {
    // Assume radians
  }
  *result = sin(value_1);
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int cos_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered COS function\n", DEBUG_M, DEFAULT_COLOUR_M);
  double value_1 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 3) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  if (word_count > 1) {
    // Read in next parameter, used as number for calculation
    if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
      printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
      return 1;
    }
  } else {
    printf("%sERROR:%s Insufficient Arguments\n", ERROR_M, DEFAULT_COLOUR_M);
  }
  if (word_count > 2) {
    // If radians don't need to alter number
    if (strcmp(RADstr, (*array_of_words_p)[2]) == 0) {
    }
    // If degrees, number needs to be converted to radians for function
    else if (strcmp(DEGstr, (*array_of_words_p)[2]) == 0) {
      value_1 = value_1 * M_PI / 180;
    }
  } else {
    // Assume radians
  }
  *result = cos(value_1);
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int tan_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered TAN function\n", DEBUG_M, DEFAULT_COLOUR_M);
  double value_1 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 3) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  if (word_count > 1) {
    // Read in next parameter, used as number for calculation
    if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
      printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
      return 1;
    }
  } else {
    printf("%sERROR:%s Insufficient Arguments\n", ERROR_M, DEFAULT_COLOUR_M);
  }
  if (word_count > 2) {
    // If radians don't need to alter number
    if (strcmp(RADstr, (*array_of_words_p)[2]) == 0) {
    }
    // If degrees, number needs to be converted to radians for function
    else if (strcmp(DEGstr, (*array_of_words_p)[2]) == 0) {
      value_1 = value_1 * M_PI / 180;
    }
  } else {
    // Assume radians
  }
  *result = tan(value_1);
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int asin_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered ARCSIN function\n", DEBUG_M,
           DEFAULT_COLOUR_M);
  double value_1 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 3) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  if (word_count > 1) {
    // Read in next parameter, used as number for calculation
    if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
      printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
      return 1;
    }
  } else {
    printf("%sERROR:%s Insufficient Arguments\n", ERROR_M, DEFAULT_COLOUR_M);
  }
  if (word_count > 2) {
    // If radians don't need to alter number
    if (strcmp(RADstr, (*array_of_words_p)[2]) == 0) {
    }
    // If degrees, number needs to be converted to radians for function
    else if (strcmp(DEGstr, (*array_of_words_p)[2]) == 0) {
      value_1 = value_1 * M_PI / 180;
    }
  } else {
    // Assume radians
  }
  *result = asin(value_1);
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int acos_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered ARCCOS function\n", DEBUG_M,
           DEFAULT_COLOUR_M);
  double value_1 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 3) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  if (word_count > 1) {
    // Read in next parameter, used as number for calculation
    if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
      printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
      return 1;
    }
  } else {
    printf("%sERROR:%s Insufficient Arguments\n", ERROR_M, DEFAULT_COLOUR_M);
  }
  if (word_count > 2) {
    // If radians don't need to alter number
    if (strcmp(RADstr, (*array_of_words_p)[2]) == 0) {
    }
    // If degrees, number needs to be converted to radians for function
    else if (strcmp(DEGstr, (*array_of_words_p)[2]) == 0) {
      value_1 = value_1 * M_PI / 180;
    }
  } else {
    // Assume radians
  }
  *result = acos(value_1);
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int atan_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered ARCTAN function\n", DEBUG_M,
           DEFAULT_COLOUR_M);
  double value_1 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 3) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  if (word_count > 1) {
    // Read in next parameter, used as number for calculation
    if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
      printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
      return 1;
    }
  } else {
    printf("%sERROR:%s Insufficient Arguments\n", ERROR_M, DEFAULT_COLOUR_M);
  }
  if (word_count > 2) {
    // If radians don't need to alter number
    if (strcmp(RADstr, (*array_of_words_p)[2]) == 0) {
    }
    // If degrees, number needs to be converted to radians for function
    else if (strcmp(DEGstr, (*array_of_words_p)[2]) == 0) {
      value_1 = value_1 * M_PI / 180;
    }
  } else {
    // Assume radians
  }
  *result = atan(value_1);
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int pow_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered POWER function\n", DEBUG_M,
           DEFAULT_COLOUR_M);
  double value_1 = 0;
  double value_2 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 3) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  // Read in next parameter, used as number for calculation
  if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
    printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  // Read in next parameter, used as number for calculation
  if (sscanf((*array_of_words_p)[2], "%lf", &value_2) != 1) {
    printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  *result = pow(value_1, value_2);
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int sqrt_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered SQUARE ROOT function\n", DEBUG_M,
           DEFAULT_COLOUR_M);
  double value_1 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 2) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  // Read in next parameter, used as number for calculation
  if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
    printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  *result = sqrt(value_1);
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int ln_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered NATURAL LOG function\n", DEBUG_M,
           DEFAULT_COLOUR_M);
  double value_1 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 2) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  // Read in next parameter, used as number for calculation
  if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
    printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  *result = log(value_1);
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int log_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered LOG 10 function\n", DEBUG_M,
           DEFAULT_COLOUR_M);
  double value_1 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 2) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  // Read in next parameter, used as number for calculation
  if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
    printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  *result = log10(value_1);
  return 0;
}

// Calculate function with given numbers
// Returns 0 on success
// Returns 1 on failure
int exp_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered EXPONENTIAL function\n", DEBUG_M,
           DEFAULT_COLOUR_M);
  double value_1 = 0;
  // From given number(s) calculate the function and store the result
  if (word_count > 2) {
    printf("%sERROR:%s Too many arguments\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  // Read in next parameter, used as number for calculation
  if (sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1) {
    printf("%sERROR:%s Found unknown argument\n", ERROR_M, DEFAULT_COLOUR_M);
    return 1;
  }
  *result = exp(value_1);
  return 0;
}

// Change between setting on and off
int formula_function(char **array_of_words_p[], int word_count,
                     double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered Formula Mode\n", DEBUG_M, DEFAULT_COLOUR_M);
  if (word_count > 1) {
    if (strcmp("on", (*array_of_words_p)[1]) == 0) {
      Set_Formula_Mode(1);
      printf("Formula Mode ON\n");
    } else if (strcmp("off", (*array_of_words_p)[1]) == 0) {
      Set_Formula_Mode(0);
      printf("Formula Mode OFF\n");
    } else {
      printf("%sERROR:%s Unknown formula command\n", ERROR_M, DEFAULT_COLOUR_M);
      return 1;
    }
  } else {
    printf("Formula mode currently %s\n",
           Get_Formula_Mode() == 0 ? "OFF" : "ON");
  }
  return 0;
}

// Change between setting on and off
int debug_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered Debug Mode\n", DEBUG_M, DEFAULT_COLOUR_M);
  if (word_count > 1) {
    if (strcmp("on", (*array_of_words_p)[1]) == 0) {
      Set_Debug(1);
      printf("Debug ON\n");
    } else if (strcmp("off", (*array_of_words_p)[1]) == 0) {
      Set_Debug(0);
      printf("Debug OFF\n");
    } else {
      printf("%sERROR:%s Unknown debug command\n", ERROR_M, DEFAULT_COLOUR_M);
      return 1;
    }
  } else {
    printf("Debug messages currently %s\n", Get_Debug() == 0 ? "OFF" : "ON");
  }
  return 0;
}

// Change between setting on and off
int system_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered System Mode\n", DEBUG_M, DEFAULT_COLOUR_M);
  if (word_count > 1) {
    if (strcmp("on", (*array_of_words_p)[1]) == 0) {
      Set_System(1);
      printf("%sSYSTEM_INFO:%s System ON\n", SYS_M, DEFAULT_COLOUR_M);
    } else if (strcmp("off", (*array_of_words_p)[1]) == 0) {
      Set_System(0);
      printf("%sSYSTEM_INFO:%s System OFF\n", SYS_M, DEFAULT_COLOUR_M);
    } else {
      printf("%sERROR:%s Unknown system command\n", ERROR_M, DEFAULT_COLOUR_M);
      return 1;
    }
  } else {
    printf("%sSYSTEM_INFO:%s System messages currently %s\n", SYS_M,
           DEFAULT_COLOUR_M, Get_System() == 0 ? "OFF" : "ON");
  }
  return 0;
}

// Clear function, clear terminal screen
int clear_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered HELP function\n", DEBUG_M,
           DEFAULT_COLOUR_M);
  printf(CLEAR_M);
  return 0;
}

// Reset function, reset terminal scrollback
int reset_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered HELP function\n", DEBUG_M,
           DEFAULT_COLOUR_M);
  printf(RESET_M);
  return 0;
}

// Help function, display help messages
int help_function(char **array_of_words_p[], int word_count, double *result) {
  if (Get_Debug() == 1)
    printf("%sDEBUG_INFO:%s Entered HELP function\n", DEBUG_M,
           DEFAULT_COLOUR_M);
  if (help_parser(array_of_words_p, word_count) != 0) {
    printf("%sERROR:%s Help Funtion\n", ERROR_M, DEFAULT_COLOUR_M);
  }
  return 0;
}

// clang-format off
/***********************************************************************************************************************
*****************************************************String Parser******************************************************
***********************************************************************************************************************/
// clang-format on

int string_parser(char *inp, char **array_of_words_p[]) {
  const char delim = ' ';
  char curr_char;
  char prev_char = delim;
  int num_words = 0;
  int len_word = 0;
  int characters = 0;
  int i;

  // Iterate through string and count the numbr of words
  for (i = 0; i < strlen(inp); i++) {
    curr_char = inp[i];

    if (curr_char == '\0')
      return 0;  // No words in string
    else if (curr_char == delim &&
             prev_char == delim) {  // Multiple spaces in a row
    } else if (curr_char != delim && prev_char == delim) {  // Found new word
      num_words++;
      characters++;
    } else if (curr_char == delim && prev_char != delim) {
    } else {  // Currently in word, don't do anything
      characters++;
    }
    prev_char = curr_char;
  }

  // Allocate enough memory to store a pointer to each word
  *array_of_words_p = (char **)calloc(num_words, sizeof(char *));
  if (array_of_words_p == 0) {  // If malloc fails returns NULL ptr
    printf("%sERROR:%s Memory allocation failed\n", ERROR_M,
           DEFAULT_COLOUR_M);  // Log
    return -1;                 // Return Failed
  }
  // Set pointer to first word
  char *word_array = (char *)calloc((characters + num_words), sizeof(char));
  if (word_array == 0) {  // If malloc fails returns NULL ptr
    printf("%sERROR:%s Memory allocation failed\n", ERROR_M,
           DEFAULT_COLOUR_M);  // Log
    free(*array_of_words_p);
    return -1;  // Return Failed
  }

  // Reset variables
  prev_char = delim;
  curr_char = delim;
  num_words = 0;
  characters = 0;

  // Iterate over string again
  for (i = 0; i < strlen(inp); i++) {
    curr_char = inp[i];

    if (curr_char == '\0')
      return 0;  // No words in string
    else if (curr_char == delim &&
             prev_char == delim) {  // Multiple spaces in a row
    } else if (curr_char != delim && prev_char == delim) {  // Found new word
      characters++;
      num_words++;
      len_word = 1;
      (*array_of_words_p)[num_words - 1] =
          &word_array[characters - 1 + (num_words - 1)];
    } else if (curr_char == delim && prev_char != delim) {  // Found end of word
      // Copy inp into memory allocation
      strncpy(&word_array[characters - len_word + (num_words - 1)],
              (inp + (i - len_word)), len_word);
      len_word = 0;
    } else {  // Currently in word, don't do anything
      characters++;
      len_word++;
    }
    prev_char = curr_char;
  }
  if (len_word != 0) {
    // Copy inp into memory allocation
    strncpy(&word_array[characters - len_word + (num_words - 1)],
            (inp + (i - len_word)), len_word);
    len_word = 0;
  }
  return num_words;
}
