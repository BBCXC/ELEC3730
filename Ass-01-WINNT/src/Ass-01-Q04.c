//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

#include "Ass-01.h"

typedef struct{
	unsigned char num_words;
	char** str;
} string_t;

string_t tokeniser(char* str, char delim, unsigned char index){
	char* token = strchr(str, delim);
	if(token) *token = '\0';
	string_t mem = (token == NULL)
				 ? (string_t){index + 1, malloc((index + 1) * sizeof(char*))}
				 : tokeniser(token+1, delim, index+1);
	mem.str[index] = str;
	return mem;
}

int string_parser(char *inp, char **array_of_words_p[]){

//	printf("\nCODE TO BE WRITTEN...\n");
//
//	//char prev_space = 0;
//	//char word_start = 1;
//	int No_words = 0;
//	//char current_position = 0;
//
//	//char Next_char;
//	char* token;
//	char str[] = "Hello this is my string -ish";
//	char delim = ' ';
//	const char *result = inp;
//
//
//	while((result = strchr(result, delim)) != NULL) {
//	    printf("Found '%c' starting at '%s'\n", delim, result);
//	    ++result; // Increment result, otherwise we'll find target at the same location
//	}
//
//	//*array_of_words_p = (char**) malloc(No_words * sizeof(char));
//
//	for(const char* ptr = inp; ;) {
//		const char* begin = ptr;
//		const char* end = strchr(ptr, delim);
//
//		ptr = end;
//	}
//
//	printf("inp %s\n", inp);
//	token = strchr(str, delim);
//	printf("token %s\n", token);
//	token = strchr(inp, delim);

	string_t result = tokeniser(inp, ' ', 0);
	*array_of_words_p = result.str;
	return result.num_words;
}
