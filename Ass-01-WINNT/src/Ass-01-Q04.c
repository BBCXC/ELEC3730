//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

#include "Ass-01.h"

int string_parser(char *inp, char **array_of_words_p[]){

	printf("\nCODE TO BE WRITTEN...\n");

	//read each character in string
	    char prev_space = 0;
	    char word_start = 1;
	    int No_words = 0;
	    char current_position = 0;

	    char Next_char;

        for(int i=0; i<20; ++i){
            printf("%c\n", inp[i]);

	        Next_char = inp[i];

	        if(Next_char == 0){    //NULL
	            if(No_words == 0){
	                printf("No words in string\n");
	                return No_words; //?
	            }
	            else{
	                //Words were found
	                return 0;
	            }
	        }
	        else if(isspace(Next_char) != 0){
	            prev_space = 1;
	            //Read next character
	            current_position++;
	            printf("Found a space\n");
	            word_start = 1;
	            if(No_words<0){
	                printf("Full word found %d\n", No_words);
	            }
	        }

	        //If i get to here must be a character
	        else{
	            if(word_start == 1){
	                //Store number of words
	                No_words++;
	                current_position++;
	                printf("Start of word found\n");
	                word_start = 0;

	                //Store stream position
	                //array_of_words_p = ;

	                prev_space = 0;
	            }
	            else{
	                //currently in a word already, do nothing
	                current_position++;
	                prev_space = 0;
	                word_start = 0;
	            }
	        }
	    }
    printf("Success");
	return No_words;
}
