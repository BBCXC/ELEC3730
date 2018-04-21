/*
 * Author 	   : Mitchell Marotta C3258958
 * 				 Taylor Young C3206230
 * Date	  	   : 23 March 2018
 * Description : Takes arguments from given string
 * 				 Splits String into individual words, with ' ' as the delim
*/

#include <stdio.h>
#include <string.h>

/*
 * Input  : Input string
 * 			Pointer to *array of words[]
 * Output : Array of pointers to each word **array_of_words_p
 * 			Success returns Number of words found; Failure returns -1
 */
int string_parser(char *inp, char **array_of_words_p[]){
    const char delim = ' ';
    char curr_char;
    char prev_char = delim;
    int num_words = 0;
    int len_word = 0;
    int characters = 0;
    int i;

    printf("\n");

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
    return num_words;
}


double sum_solver(char **array_of_words_p[], int count){
    double sum_2 = 0.0;
    double sum_1 = atoi((*array_of_words_p)[1]);
    for(int items=2; items<count; items++){
        sum_2 = atoi((*array_of_words_p)[items]);

        sum_1 = sum_1 + sum_2;
    }

    return sum_1;
}

double sub_solver(char **array_of_words_p[], int count){
    double sub_2 = 0.0;
    double sub_1 = atoi((*array_of_words_p)[1]);
    for(int items=2; items<count; items++){
        sub_2 = atoi((*array_of_words_p)[items]);

        sub_1 = sub_1 - sub_2;
    }

    return sub_1;
}

double multi_solver(char **array_of_words_p[], int count){
    double multi_2 = 0.0;
    double multi_1 = atoi((*array_of_words_p)[1]);
    for(int items=2; items<count; items++){
        multi_2 = atoi((*array_of_words_p)[2]);

        multi_1 = multi_1 * multi_2;
    }

    return multi_1;
}

double div_solver(char **array_of_words_p[], int count){
    double div_2 = 0.0;
    double div_1 = atoi((*array_of_words_p)[1]);
    for(int items=2; items<count; items++){
        div_2 = atoi((*array_of_words_p)[items]);

        div_1 = div_1 / div_2;
    }

    return div_1;
}

int main(){

    char *parse_string[] = {"add 1 2", "sub 1 2", "multi 1 2", "div 1 2", NULL};

    char **array_of_words;
    int count;
    printf("-> Question 4...\n");
    int i=0;
    int j=0;
    double result = 0;
    while (parse_string[i] != NULL)
    {
      printf("--> '%17s': ",parse_string[i]);
      count = string_parser(parse_string[i], &array_of_words);
      if (count != 0)
      {
        printf(" count = %d\n", count);
        for (j=0;j<count;j++)
        {
          printf("---> %d: '%s'\n", j+1, (array_of_words)[j]);
        }

        //Do math function

        if(strcmp(array_of_words[0], "add") == 0){    
            result = sum_solver(&array_of_words, count);
        }
        else if(strcmp(array_of_words[0], "sub") == 0){    
            result = sub_solver(&array_of_words, count);
        }
        else if(strcmp(array_of_words[0], "multi") == 0){    
            result = multi_solver(&array_of_words, count);
        }
        else if(strcmp(array_of_words[0], "div") == 0){    
            result = div_solver(&array_of_words, count);
        }

        else{
            printf("Unknown Operation");
        }

        printf("Result %f\n", result);

        free(array_of_words[0]);
        free(array_of_words);
      }
      i++;
    }
}




//TODO Does not work with array of arrays yet. See Syntax_tree.c for implementation using a single string