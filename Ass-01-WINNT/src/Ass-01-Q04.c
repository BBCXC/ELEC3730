#include "Ass-01.h"

int string_parser(char *inp, char **array_of_words_p[]){
    char curr_char;
    char prev_char = ' ';
    int num_words = 0;
    int len_word = 0;
    int characters = 0;
    int i;

    printf("\n");

    //Iterate through string and count the numbr of words
    for(i=0; i<strlen(inp); i++){
        curr_char = inp[i];

        if(curr_char == '\0') return 0;  //No words in string
        else if(curr_char == ' ' && prev_char == ' '){ //Multiple spaces in a row
        }
        else if(curr_char != ' ' && prev_char == ' '){ //Found new word
            num_words++;
            characters++;
        }
        else if (curr_char == ' ' && prev_char != ' ') {}
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
    prev_char = ' ';
    curr_char = ' ';
    num_words = 0;
    characters = 0;

    //iterate over string again
    for (i = 0; i < strlen(inp); i++) {
        curr_char = inp[i];

        if(curr_char == '\0') return 0;  //No words in string
        else if(curr_char == ' ' && prev_char == ' '){ //Multiple spaces in a row
        }
        else if(curr_char != ' ' && prev_char == ' '){ //Found new word
            characters++;
            num_words++;
            len_word = 1;
            (*array_of_words_p)[num_words - 1] = &word_array[characters - 1 + (num_words - 1)];
        }
        else if (curr_char == ' ' && prev_char != ' ') { // Found end of word
            strncpy(&word_array[characters - len_word + (num_words - 1)], (inp + (i - len_word)), len_word);  // Copy inp into memory allocation
            len_word = 0;
            word_array[characters + (num_words - 1)] = '\0';
        }
        else{ //Currently in word, don't do anything
            characters++;
            len_word++;
        }
        prev_char = curr_char;
    }
    if(len_word != 0){
    	strncpy(&word_array[characters - len_word + (num_words - 1)], (inp + (i - len_word)), len_word);  // Copy inp into memory allocation
    	len_word = 0;
        word_array[characters + (num_words - 1)] = '\0';
    }
    return num_words;
}
