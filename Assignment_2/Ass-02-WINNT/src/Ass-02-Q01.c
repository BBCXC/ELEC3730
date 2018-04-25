//TODO Implement help help
//TODO Implement debug on debuf off
//TODO Change print statements to closer reflect example
//TODO Command line parser formula mode, allows input of 1+3-7/4 etc


#include "Ass-02.h"
#ifdef STM32F407xx
#include "usart.h"
#endif

const char *ADDstr = "add";
const char *SUBstr = "sub";
const char *PROstr = "pro";
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
const char *RADstr = "rad";
const char *DEGstr = "deg";

const char *HELPstr = "help";
// #define ADDstr  "add"
// #define SUBstr  "sub"
// #define PROstr  "pro"
// #define DIVstr  "div"
// #define SINstr  "sin"
// #define COSstr  "cos"
// #define TANstr  "tan"
// #define ASINstr  "asin"
// #define ACOSstr  "acos"
// #define ATANstr  "atan"
// #define POWstr  "pow"
// #define SQRTstr  "sqrt"
// #define LNstr  "ln"
// #define LOGstr  "log"
// #define EXPstr  "exp"
// #define RADstr  "rad"
// #define DEGstr  "deg"

#define debugsys 0

/***********************************************************************************************************************
************************************************Command Line Parser Init************************************************
***********************************************************************************************************************/
void CommandLineParserInit(void){
  // Print welcome message
  printf("\014");
  printf("ELEC3730 Assignment 2\n");
  printf("Command Line Parser Example\n");
}

/***********************************************************************************************************************
**********************************************Command Line Parser Process***********************************************
***********************************************************************************************************************/
void CommandLineParserProcess(void){

  // Check for input and echo back
  #ifdef STM32F407xx
    if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK){
      printf("SERIAL: Got '%c'\n", c);
      HAL_GPIO_TogglePin(GPIOD, LD4_Pin); // Toggle LED4

      int c;
      char si[]="1234";
      int i=111;
      char sf[]="r5b6c7d8";
      float f=2.22;

      printf("TEST: Float printf() test: %f\n", 1.234);
      sscanf(si, "%d", &i);
      c=sscanf(sf, "%f", &f);
      printf("TEST: Input string : '%s'\n", si);
      printf("TEST: Input int    : %d\n", i);
      printf("TEST: Input string : '%s'\n", sf);
      printf("TEST: Input float  : %f\n", f);
      printf("TEST: c=%d\n",c);
    }

  #else

    double result = 0;
    static double prev_ans = 0;
    
    char c;
    int i;
    char command_line[101];
    char **array_of_words_p;

    // Get one line of input
    printf("--> Enter text:\n");
    i=0;
    c=getchar();
    while (c != '\n'){
 	  	command_line[i]=c;
    	i++;
      c=getchar();
    }
    command_line[i]=0;

    int word_count = string_parser(command_line, &array_of_words_p);

    if(debugsys == 1){
    	for(int i=0; i<word_count; i++){
    		printf("Word %i: %s\n", i, array_of_words_p[i]);
    	}
    }

    if(word_count > 0){
    	if(debugsys == 1) printf("Word Count = %i\n", word_count);

      char dtos[50];
      //Check the 2 value parameters for pi
      for(int j=1; j<word_count; j++){
        if((strcmp("PI", array_of_words_p[j]) == 0) ||
           (strcmp("pi", array_of_words_p[j]) == 0) ||
           (strcmp("Pi", array_of_words_p[j]) == 0)){
          if(debugsys == 1) printf("DEBUG_INFO: Constant PI Ffund\n");
          snprintf(dtos, 50, "%f", M_PI);
          array_of_words_p[j] = dtos;
        }
      }

      //Check the 2 value parameters for ans
      for(int j=1; j<word_count; j++){
        if((strcmp("ANS", array_of_words_p[j]) == 0) ||
           (strcmp("ans", array_of_words_p[j]) == 0)){
          if(debugsys == 1) printf("DEBUG_INFO: Constant PI Ffund\n");
          snprintf(dtos, 50, "%f", prev_ans);
          array_of_words_p[j] = dtos;
        }
      }

      //Check for operations
      if(strcmp(ADDstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: add_function operation selected\n");
        if(word_count == 3){
          result = add_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(SUBstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: sub_function operation selected\n");
        if(word_count == 3){
          result = sub_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(PROstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: pro_function operation selected\n");
        if(word_count == 3){
          result = pro_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(DIVstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: div_function operation selected\n");
        if(word_count == 3){
          result = div_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(SINstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: sin_function operation selected\n");
        if(word_count == 3){
          result = sin_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(COSstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: cos_function operation selected\n");
        if(word_count == 3){
          result = cos_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(TANstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: tan_function operation selected\n");
        if(word_count == 3){
          result = tan_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(ASINstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: asin_function operation selected\n");
        if(word_count == 3){
          result = asin_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(ACOSstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: acos_function operation selected\n");
        if(word_count == 3){
          result = acos_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(ATANstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: atan_function operation selected\n");
        if(word_count == 3){
          result = atan_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(POWstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: pow_function operation selected\n");
        if(word_count == 3){
          result = pow_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(SQRTstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: sqrt_function operation selected\n");
        if(word_count == 2){
          result = sqrt_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(LNstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: ln_function operation selected\n");
        if(word_count == 2){
          result = ln_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(LOGstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: log_function operation selected\n");
        if(word_count == 2){
          result = log_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(EXPstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: exp_function operation selected\n");
        if(word_count == 2){
          result = exp_function(&array_of_words_p);
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else if(strcmp(HELPstr, array_of_words_p[0]) == 0){
        if(debugsys == 1) printf("DEBUG_INFO: help_function selected\n");
        if(word_count > 0){
          if(help_function(&array_of_words_p, word_count) != 0) printf("ERROR: Help function not available\n");
        }
        else{
          printf("ERROR: Too few arguements for operation\n");
        }
      }
      else{
        printf("ERROR: Unknown Operation\n");
      }
      if(strcmp(HELPstr, array_of_words_p[0]) != 0) {
        printf("The result is %lf\n", result);
        prev_ans = result;
      }

      free(array_of_words_p[0]);
      free(array_of_words_p);

      if(debugsys == 1) printf("DEBUG_INFO: Arrays have been freed\n");
    }
    else{
      printf("ERROR: No strings detected\n");
    }

  #endif
}

/***********************************************************************************************************************
**************************************************Operation Function****************************************************
***********************************************************************************************************************/
double add_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered ADD function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  double value_2 = atof((*array_of_words_p)[2]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 2: %lf\n", value_2);
  double result = value_1 + value_2;
  return result;
}
double sub_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered SUBTRACT function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  double value_2 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 2: %lf\n", value_2);
  double result = value_1 - value_2;
  return result;
}
double pro_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered PRODUCT function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  double value_2 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 2: %lf\n", value_2);
  double result = value_1 * value_2;
  return result;
}
double div_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered DIVIDE function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  double value_2 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 2: %lf\n", value_2);
  double result = value_1 / value_2;
  return result;
}
double sin_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered SIN function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  if(strcmp(RADstr, (*array_of_words_p)[2]) == 0){
  }
  else if(strcmp(DEGstr, (*array_of_words_p)[2]) == 0){
    value_1 = value_1 * M_PI / 180;
  }
  else{
    printf("ERROR: Unknown Operation\n");
    return 0;
  }
  if(debugsys == 1) printf("DEBUG_INFO: TYPE %s\n", (*array_of_words_p)[2]);
  double result = sin(value_1);
  return result;
}
double cos_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered COS function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  if(strcmp(RADstr, (*array_of_words_p)[2]) == 0){
  }
  else if(strcmp(DEGstr, (*array_of_words_p)[2]) == 0){
    value_1 = value_1 * M_PI / 180;
  }
  else{
    printf("ERROR: Unknown Operation\n");
    return 0;
  }
  if(debugsys == 1) printf("DEBUG_INFO: TYPE %s\n", (*array_of_words_p)[2]);
  double result = cos(value_1);
  return result;
}
double tan_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered TAN function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  if(strcmp(RADstr, (*array_of_words_p)[2]) == 0){
  }
  else if(strcmp(DEGstr, (*array_of_words_p)[2]) == 0){
    value_1 = value_1 * M_PI / 180;
  }
  else{
    printf("ERROR: Unknown Operation\n");
    return 0;
  }
  if(debugsys == 1) printf("DEBUG_INFO: TYPE %s\n", (*array_of_words_p)[2]);
  double result = tan(value_1);
  return result;
}
double asin_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered ARCSIN function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  if(strcmp(RADstr, (*array_of_words_p)[2]) == 0){
  }
  else if(strcmp(DEGstr, (*array_of_words_p)[2]) == 0){
    value_1 = value_1 * M_PI / 180;
  }
  else{
    printf("ERROR: Unknown Operation\n");
    return 0;
  }
  if(debugsys == 1) printf("DEBUG_INFO: TYPE %s\n", (*array_of_words_p)[2]);
  double result = asin(value_1);
  return result;
}
double acos_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered ARCCOS function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  if(strcmp(RADstr, (*array_of_words_p)[2]) == 0){
  }
  else if(strcmp(DEGstr, (*array_of_words_p)[2]) == 0){
    value_1 = value_1 * M_PI / 180;
  }
  else{
    printf("ERROR: Unknown Operation\n");
    return 0;
  }
  if(debugsys == 1) printf("DEBUG_INFO: TYPE %s\n", (*array_of_words_p)[2]);
  double result = acos(value_1);
  return result;
}
double atan_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered ARCTAN function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  if(strcmp(RADstr, (*array_of_words_p)[2]) == 0){
  }
  else if(strcmp(DEGstr, (*array_of_words_p)[2]) == 0){
    value_1 = value_1 * M_PI / 180;
  }
  else{
    printf("ERROR: Unknown Operation\n");
    return 0;
  }
  if(debugsys == 1) printf("DEBUG_INFO: TYPE %s\n", (*array_of_words_p)[2]);
  double result = atan(value_1);
  return result;
}
double pow_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered POWER function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  double value_2 = atof((*array_of_words_p)[2]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 2: %lf\n", value_2);
  double result = pow(value_1, value_2);
  return result;
}
double sqrt_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered SQUARE ROOT function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  double result = sqrt(value_1);
  return result;
}
double ln_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered NATURAL LOG function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  double result = log(value_1);
  return result;
}
double log_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered LOG 10 function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  double result = log10(value_1);
  return result;
}
double exp_function(char **array_of_words_p[]){
  if(debugsys == 1) printf("DEBUG_INFO: Entered EXPONENTIAL function\n");
  double value_1 = atof((*array_of_words_p)[1]);
  if(debugsys == 1) printf("DEBUG_INFO: Value 1: %lf\n", value_1);
  double result = exp(value_1);
  return result;
}

int help_function(char **array_of_words_p[], int word_count){
  if(debugsys == 1) printf("DEBUG_INFO: Entered HELP function\n");
  if(word_count == 2){
    char *option = (*array_of_words_p)[1];
    if(print_help(&option) != 0) printf("ERROR: Could not print help option(s)\n");
  }
  else if(word_count > 2){
    printf("ERROR: Too many arguements for help\n");
    return 1;
  }
  else{
    char *option = "NO_OPTION";
    if(print_help(&option) != 0) printf("ERROR: Could not print help option(s)\n");
  }
  return 0;
}

/***********************************************************************************************************************
*****************************************************Help Printer*******************************************************
***********************************************************************************************************************/
int print_help(char **option){
  if(strcmp("NO_OPTION", *option) == 0){
    //Specify all functions
    printf("please select an operation\n");
    printf("HELP_INFO: add\n");
    printf("HELP_INFO: sub\n");
    printf("HELP_INFO: pro\n");
    printf("HELP_INFO: div\n");
    printf("HELP_INFO: sin\n");
    printf("HELP_INFO: cos\n");
    printf("HELP_INFO: tan\n");
    printf("HELP_INFO: asin\n");
    printf("HELP_INFO: acos\n");
    printf("HELP_INFO: atan\n");
    printf("HELP_INFO: pow\n");
    printf("HELP_INFO: sqrt\n");
    printf("HELP_INFO: ln\n");
    printf("HELP_INFO: log\n");
    printf("HELP_INFO: exp\n");
  }
  else if(strcmp(ADDstr, *option) == 0){
    printf("HELP_INFO: add_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: add\n");
    printf("Arguement 2: value_1\n");
    printf("Arguement 3: value_2\n");
    printf("Result = value_1 + value_2\n");
  }
  else if(strcmp(SUBstr, *option) == 0){
    printf("HELP_INFO: sub_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: sub\n");
    printf("Arguement 2: value_1\n");
    printf("Arguement 3: value_2\n");
    printf("Result = value_1 - value_2\n");
  }
  else if(strcmp(PROstr, *option) == 0){
    printf("HELP_INFO: pro_function operation selected\n");
    printf("This function pro 3 arguements\n");
    printf("Arguement 1: ADD\n");
    printf("Arguement 2: value_1\n");
    printf("Arguement 3: value_2\n");
    printf("Result = value_1 x value_2\n");
  }
  else if(strcmp(DIVstr, *option) == 0){
    printf("HELP_INFO: div_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: div\n");
    printf("Arguement 2: value_1\n");
    printf("Arguement 3: value_2\n");
    printf("Result = value_1 / value_2\n");
  }
  else if(strcmp(SINstr, *option) == 0){
    printf("HELP_INFO: sin_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: sin\n");
    printf("Arguement 2: value_1\n");
    printf("Arguement 3: type (rad, deg)\n");
  }
  else if(strcmp(COSstr, *option) == 0){
    printf("HELP_INFO: cos_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: cos\n");
    printf("Arguement 2: value_1\n");
    printf("Arguement 3: type (rad, deg)\n");
  }
  else if(strcmp(TANstr, *option) == 0){
    printf("HELP_INFO: tan_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: tan\n");
    printf("Arguement 2: value_1\n");
    printf("Arguement 3: type (rad, deg)\n");
  }
  else if(strcmp(ASINstr, *option) == 0){
    printf("HELP_INFO: asin_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: asin\n");
    printf("Arguement 2: value_1\n");
    printf("Arguement 3: type (rad, deg)\n");
  }
  else if(strcmp(ACOSstr, *option) == 0){
    printf("HELP_INFO: acos_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: acos\n");
    printf("Arguement 2: value_1\n");
    printf("Arguement 3: type (rad, deg)\n");
  }
  else if(strcmp(ATANstr, *option) == 0){
    printf("HELP_INFO: atan_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: atan\n");
    printf("Arguement 2: value_1\n");
    printf("Arguement 3: type (rad, deg)\n");
  }
  else if(strcmp(POWstr, *option) == 0){
    printf("HELP_INFO: pow_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: pow\n");
    printf("Arguement 2: value_1\n");
    printf("Arguement 3: value_2\n");
    printf("Result = value_1 ^ value_2\n");
  }
  else if(strcmp(SQRTstr, *option) == 0){
    printf("HELP_INFO: sqrt_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: sqrt\n");
    printf("Arguement 2: value_1\n");
  }
  else if(strcmp(LNstr, *option) == 0){
    printf("HELP_INFO: ln_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: ln\n");
    printf("Arguement 2: value_1\n");
  }
  else if(strcmp(LOGstr, *option) == 0){
    printf("HELP_INFO: log_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: log\n");
    printf("Arguement 2: value_1\n");
  }
  else if(strcmp(EXPstr, *option) == 0){
    printf("HELP_INFO: exp_function operation selected\n");
    printf("This function takes 3 arguements\n");
    printf("Arguement 1: exp\n");
    printf("Arguement 2: value_1\n");
  }
  else{
	  printf("ERROR: Could not display help information\n");
	  return 1;
  }
  return 0;
}

/***********************************************************************************************************************
*****************************************************String Parser******************************************************
***********************************************************************************************************************/
int string_parser(char *inp, char **array_of_words_p[]){
    const char delim = ' ';
    char curr_char;
    char prev_char = delim;
    int num_words = 0;
    int len_word = 0;
    int characters = 0;
    int i;

    //Iterate through string and count the numbr of words
    for(i=0; i<strlen(inp); i++){
        curr_char = inp[i];

        if(curr_char == '\0') return 0;  //No words in string
        else if(curr_char == delim && prev_char == delim){ //Multiple spaces in a row
        }
        else if(curr_char != delim && prev_char == delim){ //Found new word
            num_words++;
            characters++;
        }
        else if (curr_char == delim && prev_char != delim) {}
        else{ //Currently in word, don't do anything
            characters++;
        }
        prev_char = curr_char;
    }

    //Allocate enough memory to store a pointer to each word
    *array_of_words_p = (char**) calloc(num_words, sizeof(char*)); 
    //Set pointer to first word
    char* word_array = (char*) calloc((characters + num_words), sizeof(char));

    //Reset variables
    prev_char = delim;
    curr_char = delim;
    num_words = 0;
    characters = 0;

    //Iterate over string again
    for (i = 0; i < strlen(inp); i++) {
        curr_char = inp[i];

        if(curr_char == '\0') return 0;  //No words in string
        else if(curr_char == delim && prev_char == delim){ //Multiple spaces in a row
        }
        else if(curr_char != delim && prev_char == delim){ //Found new word
            characters++;
            num_words++;
            len_word = 1;
            (*array_of_words_p)[num_words - 1] = &word_array[characters - 1 + (num_words - 1)];
        }
        else if (curr_char == delim && prev_char != delim) { //Found end of word
            //Copy inp into memory allocation
            strncpy(&word_array[characters - len_word + (num_words - 1)], (inp + (i - len_word)), len_word);
            len_word = 0;
        }
        else{ //Currently in word, don't do anything
            characters++;
            len_word++;
        }
        prev_char = curr_char;
    }
    if(len_word != 0){
        //Copy inp into memory allocation
      strncpy(&word_array[characters - len_word + (num_words - 1)], (inp + (i - len_word)), len_word);
      len_word = 0;
    }
    for(int i=0; i<num_words; i++){
      if(debugsys == 1) printf("DEBUG_INFO: String Parser words are %s\n", (*array_of_words_p)[i]);
    }
    return num_words;
}
