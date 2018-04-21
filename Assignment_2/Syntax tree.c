//char* input = ("1", "add", "2", "multi", "4", "add", "(","5", "sub", "3", ")");
char* input;

double result;

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
		return *input++ - '0';
	}
	else if(1){
		printf("Currently in development");
	}
	else{
		printf("Unknown symbol %c", *input);
	}
	return 0;
}

int main(){
	*input = "1+2*4";
	result = parseSub();	//Lowest precedent 
	return 0;
}