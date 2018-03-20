//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

#include "Ass-01.h"



int string_parser(char *inp, char **array_of_words_p[]){
    char curr_char;
    char prev_char = ' ';
    int num_words = 0;
    int len_word = 0;

    for(int i=0; i<strlen(inp); i++){
        curr_char = inp[i];

        if(curr_char == '\0') return 0;  //No words in string
        else if(curr_char == ' ' && prev_char == ' '){
            //Multiple spaces in a row
        }
        else if(curr_char != ' ' && prev_char == ' '){
            //Found new word
            num_words++;
        }
        else{
            //Currently in word, don't do anything
        }
        prev_char = curr_char;
    }

    *array_of_words_p = (char**) calloc(num_words, sizeof(char*));

    prev_char = ' ';
    num_words     = 0;
    for (int i = 0; i < strlen(inp);) {
        curr_char = inp[i];

        if (curr_char == '\0' && len_word == 0){
            return num_words;
        }
        else if (curr_char == ' ' && prev_char == ' ') {
            // Multiple spaces in a row
        }
        else if (curr_char != ' ' && prev_char == ' ') {
            // Found new word
            num_words++;
            len_word = 1;
            // printf("Found new word\n");
        }
        else if (curr_char == ' ' && prev_char != ' ') {
            // Found end of word
            (*array_of_words_p)[num_words - 1] = (char*) calloc(len_word + 1, sizeof(char));	// Allocate enough memory for each word
            strncpy((*array_of_words_p)[num_words - 1], (inp + (i - len_word)), len_word);	// Copy inp into memory allocation
            len_word = 0;
        }
        else {
            // Currently in word
            len_word++;
        }
        prev_char = curr_char;
        // No words in string
        i++;
    }
    if(len_word != 0){
         (*array_of_words_p)[num_words - 1] = (char*) calloc(len_word + 1, sizeof(char));	// Allocate enough memory for each word
         strncpy((*array_of_words_p)[num_words - 1], (inp + (strlen(inp) - len_word)), len_word);	// Copy inp into memory allocation
         len_word = 0;
    }
    return num_words;
}



int string_parser_2(char *inp, char **array_of_words_p[]){
    char curr_char;
    char prev_char = ' ';
    int num_words = 0;
    int len_word = 0;
    int characters = 0;

    for(int i=0; i<strlen(inp); i++){
        curr_char = inp[i];

        if(curr_char == '\0') return 0;  //No words in string
        else if(curr_char == ' ' && prev_char == ' '){
            //Multiple spaces in a row
        }
        else if(curr_char != ' ' && prev_char == ' '){
            //Found new word
            num_words++;
            characters++;
        }
        else{
            //Currently in word, don't do anything
            characters++;
        }
        prev_char = curr_char;
    }

    *array_of_words_p = (char**) calloc(num_words, sizeof(char*)); 
    char* word_array = (char*) calloc((characters + num_words), sizeof(char));
    (*array_of_words_p)[num_words] = word_array[characters + num_words];

    prev_char = ' ';
    num_words     = 0;
    characters = 0;
    for (int i = 0; i < strlen(inp);) {
        curr_char = inp[i];

        if (curr_char == '\0' && len_word == 0){
            return num_words;
        }
        else if (curr_char == ' ' && prev_char == ' ') {
            // Multiple spaces in a row
        }
        else if (curr_char != ' ' && prev_char == ' ') {
            // Found new word
            num_words++;
            characters++;
            len_word = 1;
            // printf("Found new word\n");
        }
        else if (curr_char == ' ' && prev_char != ' ') {
            // Found end of word

            strncpy(word_array[characters - len_word], (inp + (i - len_word)), len_word);  // Copy inp into memory allocation
            len_word = 0;
            characters++;
            word_array[characters] = NULL;
        }
        else {
            // Currently in word
            len_word++;
        }
        prev_char = curr_char;
        // No words in string
        i++;
    }
    if(len_word != 0){
        strncpy(word_array[characters - len_word], (inp + (i - len_word)), len_word);  // Copy inp into memory allocation
        len_word = 0;
        characters++;
    }
    return num_words;
}


/*typedef struct{
	unsigned char num_words;
	char** str;
} string_t;

char* spacefinder(char* str2, char delim){
	int k;
	if(strlen(str2) < 1){
		return 0;
	}
	for(int i=0; i<strlen(str2)-1; i++){
		if(str2[i] == delim && str2[i+1] != delim){
			//a
			if(i>1) return &str2[i];
			else return &str2[i+1];

			for(k=i; k<strlen(str2); k++){
				//' '
				if(str2[k] == delim){
					for(int j=k; j<strlen(str2); j++){
						if(str2[j] != delim){
							return &str2[j-1];
						}
					}
				}
			}
		}
		else{

		}
	}
	return NULL; //This probably needs to be somethig
}

string_t tokeniser(char* str, char delim, unsigned char index){
	char* token = spacefinder(str, delim);
	//if(token) *token = '\0';
	string_t mem = (token == NULL)
				 ? (string_t){index + 1, malloc((index + 1) * sizeof(char*))}
				 : tokeniser(token+1, delim, index+1);
	mem.str[index] = str;
	return mem;
}

int string_parser(char *inp, char **array_of_words_p[]){

	string_t result = tokeniser(inp, ' ', 0);
	*array_of_words_p = result.str;
	return result.num_words;
}*/
