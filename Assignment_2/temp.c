//https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm

//char* input = ("1", "add", "2", "multi", "4", "add", "(","5", "sub", "3", ")");
#include <stdio.h>
#include <string.h>

char* input;
char** input_S;

double result;

double parseSub();
double parseFactor();
double parseSub_S();
double parseFactor_S();

double parseSub(){
	double sub_1 = parseFactor();
	printf("Sub_1 1: %lf\n", sub_1);
	while(*input == '-'){
		++input;
		printf("Input 1: %c\n", *input);
		double sub_2 = parseFactor();
		printf("Sub_2 1: %lf\n", sub_2);
		sub_1 = sub_1 - sub_2;
		printf("Sub_1 2: %lf\n", sub_1);
	}
	return sub_1;
}

double parseFactor(){

	if(*input >= '0' && *input <= '9'){
		printf("Input 2: %c\n", *input);
		return *input++ - '0';
	}
	else{
		printf("Unknown symbol %c\n", *input);
	}
	return 0;
}

double parseSub_S(){
	double sub_1 = parseFactor();
	printf("Sub_1 1: %lf\n", sub_1);
	while(*input_S == '-'){
		++input_S;
		printf("Input 1: %c\n", *input_S);
		double sub_2 = parseFactor();
		printf("Sub_2 1: %lf\n", sub_2);
		sub_1 = sub_1 - sub_2;
		printf("Sub_1 2: %lf\n", sub_1);
	}
	return sub_1;
}

double parseFactor_S(){

	if(strcmp(**input_S,"-") != 0){
		printf("Input 2: %d\n", **input_S);
		return atoi(**input_S++);
	}
	else{
		printf("Unknown symbol %c\n", *input_S);
	}
	return 0;
}

int main(){
	input = "9-1-3-2";
	result = parseSub();	//Lowest precedent 
	printf("Result %lf\n\n\n", result);

	**input_S = {"9", "-", "1", "-", "3", "-", "2"};
	result = parseSub();	//Lowest precedent 
	printf("Result %lf\n", result);

	return 0;
}