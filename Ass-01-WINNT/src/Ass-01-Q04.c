//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

#include "Ass-01.h"


/*
 * string with words seperated by space char
 * allocate memory for pointer size of number of words 		(char*) malloc(sizeof(char*) * num_words);
 * allocate memory for each word 							(char) malloc(sizeof(word) + char)
 *
 * array_of_words_p[word[0], word[1], word[2]]
 * char* word[0] = malloc(sizeof(char) * wordlen[0]);
 *
 * return max index of word[index]
 *
 * case - 1 word
 * case - no words - space
 * 				   - null
 * case - multiple spaces
 *
 *
 */

/*for (two times){
 * if second time(){malloc array of words}
 * 	token = strchr(inp, delim)
 * 	if token == prevtoken +1 //only moved by one must be another space
 * 	if token > prevtoken+1 {length of word = token - previous token
 * 							numwords++
 * 							if second time { arrayofwords[numwords] = malloc length of word }
 * 							}
 *
 * 	}
 */


int string_parser(char *inp, char **array_of_words_p[]){
	char Current_char;
	char Previous_char = ' ';
	int Num_words = 0;
	char* Word_temp;
	int Length_word = 0;
	int Index = 0;

	printf("\n");

	//printf("Printing here %s\n", *array_of_words_p[0]);

	for(int i=0; i<strlen(inp); i++){
		Current_char = inp[i];
		if(Current_char == '\0') return 0;	//No words in string
		else if(Current_char == ' ' && Previous_char == ' '){
			//Multiple spaces in a row
		}
		else if(Current_char != ' ' && Previous_char == ' '){
			//Found new word
			Num_words++;
			printf("Found new word\n");
		}
		else{
			//Currently in word, don't do anything
		}
		Previous_char = Current_char;
	}


//	*array_of_words_p = (char**) malloc(sizeof(char*) * Num_words);
//	for(int j = 0; j < Num_words; j++){
//		array_of_words_p[j] = NULL;
//	}
	char* str = (char*) malloc(sizeof(char*));
	str = "My String Is This";
	//array_of_words_p[0] = &str;
	//array_of_words_p[1] = &str;
	printf("str : %s\n", str);

	array_of_words_p = (char***) calloc(Num_words, sizeof(char*));

	for(int i=0; i<3; i++){
		array_of_words_p[i] = malloc(sizeof(char*));
		*array_of_words_p[i] = str;
		printf("Printing here, %i, %s\n", i, *array_of_words_p[i]);
	}
	//printf("Printing here %s\n", *array_of_words_p[0]);
	//*array_of_words_p[0] = str;
	//*array_of_words_p[1] = str;
	//printf("Printing here 2 %s\n", *array_of_words_p[0]);





//	Previous_char = ' ';
//	Num_words = 0;
//
//	for(int i=0; i<strlen(inp); i++){
//		Current_char = inp[i];
//				if(Current_char == '\0') return Num_words;
//				else if(Current_char == ' ' && Previous_char == ' '){
//					//Multiple spaces in a row
//				}
//				else if(Current_char != ' ' && Previous_char == ' '){
//					//Found new word
//					Num_words++;
//					Length_word = 1;
//					//printf("Found new word\n");
//				}
//				else if(Current_char == ' ' && Previous_char != ' '){
//					//Found end of word
//					Word_temp = (char*) malloc(sizeof(char) * (Length_word + 1));
//					Index = 0;
//					for(int k=(i-Length_word); k<i; k++){
//						Word_temp[Index] = inp[k];
//						Index++;
//					}
//					Word_temp[Index] = '\0';
//					//array_of_words_p[Num_words] = &Word_temp;
//					printf("Found Whole Word :%s\n", Word_temp);
//				}
//				else{
//					//Currently in word
//					Length_word++;
//				}
//				Previous_char = Current_char;
//				//No words in string
//	}

	return Num_words;
}



/*int string_parser(char *inp, char **array_of_words_p[]){
	int num_words = 0;
	char character;
	int character_count = 0;
	char* Words;
	int prev_c_space = 0;

	for(int i=0; i<strlen(inp); i++){
		//Go through and find how many words
		character = inp[i];
		if(character == '\0'){
			//No word in input, end function
			return 0;
		}
		if(isspace(character) !=0){
			//Character is white space
			//3 conditions, previous character was white space => still whitespace
			//				previous character was letter => end of word
			//				??
			prev_c_space = 1;
			if(prev_c_space == 0){
				//if previous character was not space
				//Found end of word.
				Words[num_words] = (char*) malloc(sizeof(char) * character_count);
				character_count = 0;
			}
		}
		else{
			//Character is a character
			if(prev_c_space == 1){
				//Previous was WS => new word
				num_words++;	//New word
				character_count++;	//New character of current word
				prev_c_space = 0; //Reset
			}
			else{
				//Currently in word
				character_count++;
			}
		}
	}

	for(int i=0; i<strlen(inp); i++){
		//Go through and find the word string and store
		character = inp[i];
		if(character == '\0'){
			//No word in input, end function
			return 0;
		}
		if(isspace(character) !=0){
			//Character is white space
			//3 conditions, previous character was white space => still whitespace
			//				previous character was letter => end of word
			//				??
			if(prev_c_space == 0){
				//if previous character was not space
				//Found end of word.
				character_count = 0;
			}
		}
		else{
			//Character is a character
			if(prev_c_space == 1){
				//Previous was WS => new word
				Words[character_count] = character;
				num_words++;	//New word
				character_count++;	//New character of current word
			}
			else{
				//Currently in word
				Words[character_count] = character;
				character_count++;
			}
		}
	}
}*/


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

/*int string_parser(char *inp, char **array_of_words_p[]){
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
							*array_of_words_p[No_words] = inp;
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
}*/
