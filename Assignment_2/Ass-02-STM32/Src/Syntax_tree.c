#include "Syntax_tree.h"

double parseFormula(){
	grid_space_p.result = parseSub();
	if(*grid_space_p.input == '\0'){
		return grid_space_p.result;
	}
	// printf("Expected end of grid_space_p.input but found %c\n", *grid_space_p.input);
	printf("Syntax Error\n");
	return 0;
}

double parseSub(){
	double sub_1 = parseSum();
	while(*grid_space_p.input == '-'){
		++grid_space_p.input;
		double sub_2 = parseSum();
		sub_1 = sub_1 - sub_2;
	}
	return sub_1;
}

double parseSum(){
	double sum_1 = parsePro();
	while(*grid_space_p.input == '+'){
		++grid_space_p.input;
		double sum_2 = parsePro();
		sum_1 = sum_1 + sum_2;
	}
	return sum_1;
}

double parsePro(){
	double pro_1 = parseDiv();
	while(*grid_space_p.input == '*'){
		++grid_space_p.input;
		double pro_2 = parseDiv();
		pro_1 = pro_1 * pro_2;
	}
	return pro_1;
}

double parseDiv(){
	double div_1 = parseFactor();
	while(*grid_space_p.input == '/'){
		++grid_space_p.input;
		double div_2 = parseFactor();
		div_1 = div_1 / div_2;
	}
	return div_1;
}

double parseFactor(){

	if(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
		return parseNumber();
	}
	else if(*grid_space_p.input == '-'){
		return parseNumber();
	}
	else if(*grid_space_p.input == '('){
		++grid_space_p.input;
		double temp = parseSub();
		++grid_space_p.input;
		return temp;
	}
	else{
		printf("Syntax Error\n");
		printf("Unknown symbol %c", *grid_space_p.input);
	}
	return 0;
}

double parseNumber(){

	double number = 0;
	int neg_flag = 1;
	//TODO check the first character is a number or a minus
	if(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
	}
	else if(*grid_space_p.input == '-'){
		neg_flag = -1;
		++grid_space_p.input;
	}
	else{
		printf("Syntax Error");
	}


	while(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
		number = number * 10;
		number = number + *grid_space_p.input - '0';
		++grid_space_p.input;
	}

	if(*grid_space_p.input == '.'){
		++grid_space_p.input;

		//Check the next character is a number, else error

		if(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
			double weight = 1;
			while(*grid_space_p.input >= '0' && *grid_space_p.input <= '9'){
				weight = weight / 10.0;
				double scaled = (*grid_space_p.input - '0') * weight;
				number = number + scaled;
				++grid_space_p.input;
			}
		}
		else{
			printf("Syntax Error");
		}
	}

	return (number * neg_flag);
}

int main(){
	
	// for(int i=0; i<strlen(*input_string); i++){
	// 	strcat(input, input_string[i]);
	// }

	grid_space_p.input = "5+3*9-(-4)";	
	grid_space_p.result = parseFormula();	//Lowest precedent 
	printf("Result %lf", grid_space_p.result);
	return 0;
}
