//TODO Implement help help
//TODO Fix help Function
//Compress sin cos tan asin acos atan to single function
//Compress add mul to single function
//Compress sub div pow to single function
//Compress ln log exp sqrt to single function


#include "Ass-02.h"
#ifdef STM32F407xx
#include "usart.h"
#endif

static int debugsys = 1;
static int formula_mode = 0;

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

    if(word_count > 0 && formula_mode == 0){

    	if(debugsys == 1) printf("Word Count = %i\n", word_count);

      char dtos[50];
      //Check the 2 value parameters for pi
      for(int j=1; j<word_count; j++){
        if((strcmp("PI", array_of_words_p[j]) == 0) ||
           (strcmp("pi", array_of_words_p[j]) == 0) ||
           (strcmp("Pi", array_of_words_p[j]) == 0)){
          if(debugsys == 1) printf("DEBUG_INFO: Constant PI found\n");
          snprintf(dtos, 50, "%lf", M_PI);
          array_of_words_p[j] = dtos;
        }
      }

      //Check the 2 value parameters for ans
      for(int j=1; j<word_count; j++){
        if((strcmp("ANS", array_of_words_p[j]) == 0) ||
           (strcmp("ans", array_of_words_p[j]) == 0)){
          if(debugsys == 1) printf("DEBUG_INFO: Constant ANS found\n");
          snprintf(dtos, 50, "%lf", prev_ans);
          array_of_words_p[j] = dtos;
        }
      }

      int mode = -1;
      mode = command_parser(&array_of_words_p, word_count, debugsys, &prev_ans);
      if(mode == -1){
    	  printf("ERROR: Unknown Operation\n");
      }
      else if(mode == 0){
    	  printf("The result is %lf\n", parser.result);
    	  parser.result = prev_ans;
    	  printf("Ans %lf\n", prev_ans);
      }
      else{
    	  //Must have changed debug, formula or help
      }


      // printf("ERROR: Unknown Operation\n");

      free(array_of_words_p[0]);
      free(array_of_words_p);

      if(debugsys == 1) printf("DEBUG_INFO: Arrays have been freed\n");
    }
    else if(formula_mode == 1){
      if(command_parser(&array_of_words_p, word_count, debugsys, &prev_ans) == 1){
	  }
      else{
		  parser.formula = array_of_words_p[0];
		  parser.result = 0;
		  parser.prev_ans = parseFormula();
		  printf("The result is %lf\n", parser.result);
		  printf("Answer stored is %lf\n", parser.prev_ans);
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
int add_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered ADD function\n");
  double value_1 = 0;
  double value_2 = 0;
  for(int i=1; i<word_count; i++){
	  if(sscanf((*array_of_words_p)[i], "%lf", &value_2) != 1){
		  printf("ERROR: Found unknown argument %s\n", (*array_of_words_p)[i]);
		  return 1;
	  }
	  value_1+=value_2;
  }
  *result = value_1;
  return 0;
}
int sub_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered SUB function\n");
  double value_1 = 0;
  double value_2 = 0;
  if(word_count > 3){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
   if(sscanf((*array_of_words_p)[2], "%lf", &value_2) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  *result = value_1 - value_2;
  return 0;
}
int mul_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered MUL function\n");
  double value_1 = 1;
  double value_2 = 0;
  for(int i=1; i<word_count; i++){
    if(sscanf((*array_of_words_p)[i], "%lf", &value_2) != 1){
      printf("ERROR: Found unknown argument");
      return 1;
    }
    value_1 = value_1 * value_2;
  }
  *result = value_1;
  return 0;
}
int div_function(char **array_of_words_p[], int word_count, double *result){
 if(debugsys == 1) printf("DEBUG_INFO: Entered DIV function\n");
  double value_1 = 0;
  double value_2 = 0;
  if(word_count > 3){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
   if(sscanf((*array_of_words_p)[2], "%lf", &value_2) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  *result = value_1 / value_2;
  return 0;
}
int sin_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered SIN function\n");
  double value_1 = 0;
  if(word_count > 3){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  if(strcmp(RADstr, (*array_of_words_p)[2]) == 0){
  }
  else if(strcmp(DEGstr, (*array_of_words_p)[2]) == 0){
    value_1 = value_1 * M_PI / 180;
  }
  else{
    printf("ERROR: Unknown Operation\n");
    return 1;
  }
  *result = sin(value_1);
  return 0;
}
int cos_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered COS function\n");
  double value_1 = 0;
  if(word_count > 3){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  if(strcmp(RADstr, (*array_of_words_p)[2]) == 0){
  }
  else if(strcmp(DEGstr, (*array_of_words_p)[2]) == 0){
    value_1 = value_1 * M_PI / 180;
  }
  else{
    printf("ERROR: Unknown Operation\n");
    return 1;
  }
  *result = sin(value_1);
  return 0;
}
int tan_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered TAN function\n");
  double value_1 = 0;
  if(word_count > 3){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  if(strcmp(RADstr, (*array_of_words_p)[2]) == 0){
  }
  else if(strcmp(DEGstr, (*array_of_words_p)[2]) == 0){
    value_1 = value_1 * M_PI / 180;
  }
  else{
    printf("ERROR: Unknown Operation\n");
    return 1;
  }
  *result = sin(value_1);
  return 0;
}
int asin_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered ARCSIN function\n");
  double value_1 = 0;
  if(word_count > 3){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  if(strcmp(RADstr, (*array_of_words_p)[2]) == 0){
  }
  else if(strcmp(DEGstr, (*array_of_words_p)[2]) == 0){
    value_1 = value_1 * M_PI / 180;
  }
  else{
    printf("ERROR: Unknown Operation\n");
    return 1;
  }
  *result = sin(value_1);
  return 0;
}
int acos_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered ARCCOS function\n");
  double value_1 = 0;
  if(word_count > 3){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  if(strcmp(RADstr, (*array_of_words_p)[2]) == 0){
  }
  else if(strcmp(DEGstr, (*array_of_words_p)[2]) == 0){
    value_1 = value_1 * M_PI / 180;
  }
  else{
    printf("ERROR: Unknown Operation\n");
    return 1;
  }
  *result = sin(value_1);
  return 0;
}
int atan_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered ARCTAN function\n");
  double value_1 = 0;
  if(word_count > 3){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  if(strcmp(RADstr, (*array_of_words_p)[2]) == 0){
  }
  else if(strcmp(DEGstr, (*array_of_words_p)[2]) == 0){
    value_1 = value_1 * M_PI / 180;
  }
  else{
    printf("ERROR: Unknown Operation\n");
    return 1;
  }
  *result = sin(value_1);
  return 0;
}
int pow_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered POWER function\n");
  double value_1 = 0;
  double value_2 = 0;
  if(word_count > 3){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
   if(sscanf((*array_of_words_p)[2], "%lf", &value_2) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  *result = pow(value_1, value_2);
  return 0;
}
int sqrt_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered SQUARE ROOT function\n");
  double value_1 = 0;
  if(word_count > 2){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  *result = sqrt(value_1);
  return 0;
}
int ln_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered NATURAL LOG function\n");
  double value_1 = 0;
  if(word_count > 2){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  *result = log(value_1);
  return 0;
}
int log_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered LOG 10 function\n");
  double value_1 = 0;
  if(word_count > 2){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  *result = log10(value_1);
  return 0;
}
int exp_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered EXPONENTIAL function\n");
  double value_1 = 0;
  if(word_count > 2){
    printf("ERROR: Too many arguments\n");
    return 1;
  }
  if(sscanf((*array_of_words_p)[1], "%lf", &value_1) != 1){
    printf("ERROR: Found unknown argument");
    return 1;
  }
  *result = exp(value_1);
  return 0;
}
int formula_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered Formula Mode\n");
  if(word_count > 1){
    if(strcmp("on", (*array_of_words_p)[1]) == 0){
      formula_mode = 1;
      printf("Formula Mode ON\n");
    }
    else if(strcmp("off", (*array_of_words_p)[1]) == 0){
      formula_mode = 0;
      printf("Formula Mode OFF\n");
    }
    else{
      printf("ERROR: Unknown debug command\n");
      return 1;
    }
  }
  return 0;
}
int debug_function(char **array_of_words_p[], int word_count, double *result){
	if(debugsys == 1) printf("DEBUG_INFO: Entered Debug Mode\n");
  if(word_count > 1){
    if(strcmp("on", (*array_of_words_p)[1]) == 0){
      debugsys = 1;
      printf("Debug ON\n");
    }
    else if(strcmp("off", (*array_of_words_p)[1]) == 0){
      debugsys = 0;
      printf("Debug OFF\n");
    }
    else{
      printf("ERROR: Unknown debug command\n");
      return 1;
    }
  }
	return 0;
}

//TODO If help called
//TODO if words > 1
//TODO command list print statement
//TODO else call command list print all
int help_function(char **array_of_words_p[], int word_count, double *result){
  if(debugsys == 1) printf("DEBUG_INFO: Entered HELP function\n");
  if(word_count > 1){
    else{
      printf("ERROR: Unknown debug command\n");
      return 1;
    }
  }
  else{

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
