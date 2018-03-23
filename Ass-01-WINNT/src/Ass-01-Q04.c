/*
 * Author 	   : Mitchell Marotta C3258958
 * 				 Taylor Young C3206230
 * Date	  	   : 23 March 2018
 * Description : Takes arguments from given string
 * 				 Splits String into individual words, with ' ' as the delim
*/

#include "Ass-01.h"

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
