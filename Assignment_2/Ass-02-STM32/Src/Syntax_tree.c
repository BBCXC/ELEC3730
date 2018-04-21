#include <stdio.h>
#include <string.h>

char* input_string[] = {"1", "add", "2", "multi", "4", "add", "(","5", "sub", "3", ")", NULL};
char* input;

double result;

double parseFormula();
double parseSub();
double parseSum();
double parsePro();
double parseDiv();
double parseFactor();
double parseNumber();

double parseFormula(){
	result = parseSub();
	if(*input == '\0'){
		return result;
	}
	// printf("Expected end of input but found %c\n", *input);
	printf("Syntax Error\n");
	return 0;
}

double parseSub(){
	double sub_1 = parseSum();
	while(*input == '-'){
		++input;
		double sub_2 = parseSum();
		sub_1 = sub_1 - sub_2;
	}
	return sub_1;
}

double parseSum(){
	double sum_1 = parsePro();
	while(*input == '+'){
		++input;
		double sum_2 = parsePro();
		sum_1 = sum_1 + sum_2;
	}
	return sum_1;
}

double parsePro(){
	double pro_1 = parseDiv();
	while(*input == '*'){
		++input;
		double pro_2 = parseDiv();
		pro_1 = pro_1 * pro_2;
	}
	return pro_1;
}

double parseDiv(){
	double div_1 = parseFactor();
	while(*input == '/'){
		++input;
		double div_2 = parseFactor();
		div_1 = div_1 / div_2;
	}
	return div_1;
}

double parseFactor(){

	if(*input >= '0' && *input <= '9'){
		return parseNumber();
	}
	else if(*input == '-'){
		return parseNumber();
	}
	else if(*input == '('){
		++input;
		double temp = parseSub();
		++input;
		return temp;
	}
	else{
		printf("Syntax Error\n");
		printf("Unknown symbol %c", *input);
	}
	return 0;
}

double parseNumber(){

	double number = 0;
	int neg_flag = 1;
	//TODO check the first character is a number or a minus
	if(*input >= '0' && *input <= '9'){
	}
	else if(*input == '-'){
		neg_flag = -1;
		++input;
	}
	else{
		printf("Syntax Error");
	}


	while(*input >= '0' && *input <= '9'){
		number = number * 10;
		number = number + *input - '0';
		++input;
	}

	if(*input == '.'){
		++input;

		//Check the next character is a number, else error

		if(*input >= '0' && *input <= '9'){
			double weight = 1;
			while(*input >= '0' && *input <= '9'){
				weight = weight / 10.0;
				double scaled = (*input - '0') * weight;
				number = number + scaled;
				++input;
			}
		}
		else{
			printf("Syntax Error");
		}
	}


	// else if(*input == ')'){
	// 	return number;
	// }
	// else{
	// 	printf("ERROR : No right bracket");
	// }
	return (number * neg_flag);
}

int main(){
	
	// for(int i=0; i<strlen(*input_string); i++){
	// 	strcat(input, input_string[i]);
	// }

	input = "5+3*9-(-4)";
	result = parseFormula();	//Lowest precedent 
	printf("Result %lf", result);
	return 0;
}