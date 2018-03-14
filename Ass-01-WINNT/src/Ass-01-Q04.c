//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

#include "Ass-01.h"

/*typedef struct{
	unsigned char num_words;
	char** str;
} string_t;

char* spacefinder(char* str2, char delim){
	int k;

//	for(int i=0; i<strlen(str2); i++){
//		if(str2[i] == delim){
//			for(k=i; k<strlen(str2); k++){
//				if(str2[k] == delim){
//					for(int j=k; j<strlen(str2); j++){
//						if(str2[j] != delim){
//							return &str2[j];
//						}
//					}
//				}
//			}
//		}
//	}

	for(int i=0; i<strlen(str2); i++){
		if(str2[i] != delim){
			for(k=i; k<strlen(str2); k++){
				if(str2[k] == delim){
					for(int j=k; j<strlen(str2); j++){
						if(str2[j] != delim){
							return &str2[j-1];
						}
					}
				}
			}
		}
	}



//	for(k=0; k<strlen(str2); k++){
//			if(str2[k] == delim){
//				for(int j=k; j<strlen(str2); j++){
//					if(str2[j] != delim){
//						return &str2[j];
//					}
//				}
//			}
//		}
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

int string_parser(char *inp, char **array_of_words_p[]){
	printf("\nCODE TO BE WRITTEN...\n");

	//read each character in string
	    char prev_space = 0;
	    char word_start = 0;
	    int No_words = 0;
		int index = 0;
		char mem;
		char Next_char;
		int word_begin = 0;
		int current_position = 0;

	    for(int k=0; k<2; k++){
	    	if(k>0){
	    		*array_of_words_p = (char**) malloc(sizeof(char) * No_words);
	    	}

			for(int i=0; i<20; ++i){
				//printf("%c\n", inp[i]);

				Next_char = inp[i];

				if(Next_char == 0){    //NULL
					if(No_words == 0){
						printf("No words in string\n");
						return No_words; //?
					}
					else{
						//Words were found
						return No_words;
					}
				}
				if(isspace(Next_char) == 0){
					prev_space = 1;

					//printf("Found a space\n");
					word_start = 1;
					if(No_words<1){
						printf("Full word found %d\n", No_words);
					}
					if(k>0){
						for(int j=word_begin; j<i; j++){
							*array_of_words_p[No_words][j] = inp[j];
							printf("inp %s", &inp[j]);
						}
						printf("\n");
					}
				}

				//If i get to here must be a character
				else{
					if(word_start == 1){
						//Store number of words
						No_words++;
						printf("Start of word found, :%s\n", &inp[i]);
						word_start = 0;

						//Store stream position
						if(k>0){
							//*array_of_words_p[No_words] = inp;
						}
						prev_space = 0;
						word_begin = current_position;
						printf("word_begin %i", word_begin);
						current_position++;
					}
					else{
						//currently in a word already, do nothing
						current_position++;
						prev_space = 0;
						word_start = 0;
					}
				}
			}
	    }
    printf("Success");
	return No_words;
}
