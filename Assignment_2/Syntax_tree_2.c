//https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm

//char* input = ("1", "add", "2", "multi", "4", "add", "(","5", "sub", "3", ")");
#include <stdio.h>

// char* input[] = {"3", "+", "(", "9", "/", "3", ")", "+", "(", "3", "*", "8", ")"};
char* input[] = {"3", "-", "2"};

double result = 0.0;
index = 0;

double parseSub();
double parseFactor();

double parseSub(){
	double sub_1 = parseFactor();
	printf("sub_1 %lf, index %d\n", sub_1, index);
	while(input[index] == "-"){
		++index;
		input[index];
		double sub_2 = parseFactor();
		printf("sub_1 %lf, index %d\n", sub_2, index);
		sub_1 = sub_1 - sub_2;
		printf("sub_1 %lf, index %d\n", sub_1, index);
	}
	return sub_1;
}

double parseFactor(){

	if(*input[index] >= '0' && *input[index] <= '9'){
		++index;
		return (input[index] - '0');
	}
	else if(*input == "("){
		++index;
		input[index];
		double temp = parseSub();
		printf("temp %lf, index %d\n", temp), index;
		++index;
		input[index];
		return temp;	
		//printf("Currently in development");
	}
	else{
		printf("Unknown symbol %c", *input[index]);
	}
	return 0;
}

int main(){
	result = parseSub();	//Lowest precedent 
	printf("Result %lf", result);
	return 0;
}