//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"
#ifdef STM32F407xx
#include "usart.h"
#endif

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE
//

void CommandLineParserInit(void)
{
  // Print welcome message
  printf("\014");
  printf("ELEC3730 Assignment 2\n");
  printf("Command Line Parser Example\n");
}

void CommandLineParserProcess(void)
{
  uint8_t c;

  // Check for input and echo back
#ifdef STM32F407xx
  if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK)
  {
    printf("SERIAL: Got '%c'\n", c);
    HAL_GPIO_TogglePin(GPIOD, LD4_Pin); // Toggle LED4

    // STEPIEN: The following is some test code that can be removed
    //
    {
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
  }
#else
 char Array2[100];
 char buffer[30];
 int info;
 int addstatus;
 char Array1[3];
 char **array_of_words_p;
 char cha =0;
 int i = 0;
 double addition = 0;
 char *ADDSTRING = "add";
  cha = getchar();
 while (cha != '\n') {
   Array2[i]= cha;
  /* printf("Array2 %c \n", Array2[i]); */
   i++;
   cha = getchar();
  }
 /*
Array1[1] = '4';
Array1[2] = '8';
*/

  // printf("Array2 %s\n", Array2);

 info = string_parser(Array2, &array_of_words_p);
//  for(int j=0; j<5; j++){
  //  printf("First word %s\n", array_of_words_p[j]);
    // }
   printf("number of words %i \n", info);
   printf("First word %s\n", array_of_words_p[0]);
   printf("Second word %s\n", array_of_words_p[1]);
   printf("third word %s\n", array_of_words_p[2]);

   for (int j=1; j<3; j++) {
    buffer[j]=atof(array_of_words_p[j]);

/*
  for (int j=0 ; j < 1 ; j++) {
  char t;
  t = Array2[j];
  printf("the character is %c \n", t);
  */
  }

   addstatus = strcmp(ADDSTRING, array_of_words_p[0]);
     if (addstatus==0) {
    printf("add has been detected \n");
    printf("%lf \n", buffer[1]);
    printf("%lf \n", buffer[2]);
    addition = buffer[1] + buffer[2];
    printf("The answer is %lf", addition);
       }

// printf("%i", addstatus);

// printf("%s", y);
// printf("%s", array_of_words_p[0]);



#endif

}

  int string_parser(char *Array1, char **array_of_words_p[]) {
    const char delim = ' ';
        char curr_char;
        char prev_char = delim;
        int num_words = 0;
        int len_word = 0;
        int characters = 0;
        int i;

        printf("\n");

        //Iterate through string and count the number of words
        for(i=0; i<strlen(Array1); i++){
            curr_char = Array1[i];

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
        for (i = 0; i < strlen(Array1); i++) {
            curr_char = Array1[i];

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
                strncpy(&word_array[characters - len_word + (num_words - 1)], (Array1 + (i - len_word)), len_word);
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
          strncpy(&word_array[characters - len_word + (num_words - 1)], (Array1 + (i - len_word)), len_word);
          len_word = 0;
        }
        return num_words;
  }
