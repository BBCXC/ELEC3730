//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"

const command_s Command_list[] = {
  {"add",     &add_function,     "add <num 1> . + . <num N>", "Description: Add one or more numbers"},
  {"sub",     &sub_function,     "sub <num 1> - <num 2>",     "Description: Subtract two numbers"},
  {"mul",     &mul_function,     "mul <num 1> . * . <num N>", "Description: Multiply one or more numbers"},
  {"div",     &div_function,     "div <num 1> / <num 2>",     "Description: Divide two numbers"},
  {"sin",     &sin_function,     "sin <num 1> <rad|deg>",     "Description: sin number in radians or degrees"},
  {"cos",     &cos_function,     "cos <num 1> <rad|deg>",     "Description: cos number in radians or degrees"},
  {"tan",     &tan_function,     "tan <num 1> <rad|deg>",     "Description: tan number in radians or degrees"},
  {"asin",    &asin_function,    "asin <num 1> <rad|deg>",    "Description: asin number in radians or degrees"},
  {"acos",    &acos_function,    "acos <num 1> <rad|deg>",    "Description: acos number in radians or degrees"},
  {"atan",    &atan_function,    "atan <num 1> <rad|deg>",    "Description: atan number in radians or degrees"},
  {"pow",     &pow_function,     "pow <num 1> ^ <num 2>",     "Description: Power base, exponent"},
  {"sqrt",    &sqrt_function,    "sqrt <num 1>",              "Description: Add one or more numbers"},
  {"ln",      &ln_function,      "ln <num 1>",                "Description: Add one or more numbers"},
  {"log",     &log_function,     "log <num 1>",               "Description: Add one or more numbers"},
  {"exp",     &exp_function,     "exp <num 1>",               "Description: Add one or more numbers"},
  {"formula", &formula_function, "formula <on|off>",          "Description: Display help messages"},
  {"debug",   &debug_function,   "debug <on|off>",            "Description: Display help messages"},
  {"help",    &help_function,    "help [command]",            "Description: Display help messages"},
  {NULL,      NULL,              NULL,                        NULL}
};

int command_parser(char **array_of_words_p[], int word_count,int debugsys, double *prev_ans){
  double result = 0;

  for(int i=0; i<18; i++){
	if(strcmp((*array_of_words_p)[0], Command_list[i].NameString) == 0){
	  if(debugsys == 1) printf("Operation: %s\n", Command_list[i].NameString);
	  if(Command_list[i].Function_p(array_of_words_p, word_count, &result) == 0){
		parser.result = result;
		*prev_ans = result;
		if(i <= 14)return 0;
		else return 1;
	  }
	  else{
		  return -1;
	  }
	}
  }
  return -1;
}

/***********************************************************************************************************************
************************************************Recursive Decent Parser*************************************************
***********************************************************************************************************************/
double parseFormula(){
	printf("Formula Parsed: %s\n", parser.formula);
	parser.result = parseSub();
	if(*parser.formula == '\0'){
	  return parser.result;
	}
	// printf("Expected end of parser.input but found %c\n", *parser.input);
	printf("Syntax Error\n");
  return parser.result;
	return 0;
}

double parseSub(){
	double sub_1 = parseSum();
	while(*parser.formula == '-'){
	  ++parser.formula;
	  double sub_2 = parseSum();
	  sub_1 = sub_1 - sub_2;
	}
return sub_1;
}

double parseSum(){
	double sum_1 = parsePro();
	while(*parser.formula == '+'){
	  ++parser.formula;
	  double sum_2 = parsePro();
	  sum_1 = sum_1 + sum_2;
	}
	return sum_1;
}

double parsePro(){
	double pro_1 = parseDiv();
	while(*parser.formula == '*'){
	  ++parser.formula;
	  double pro_2 = parseDiv();
	  pro_1 = pro_1 * pro_2;
	}
	return pro_1;
}

double parseDiv(){
	double div_1 = parsePow();
	while(*parser.formula == '/'){
	  ++parser.formula;
	  double div_2 = parsePow();
	  div_1 = div_1 / div_2;
	}
	return div_1;
}

double parsePow(){
	double pow_1 = parseFactor();
	while(*parser.formula == '^'){
	  ++parser.formula;
	  double pow_2 = parseFactor();
	  pow_1 = pow(pow_1, pow_2);
	}
	return pow_1;
}

double parseFactor(){

	if(*parser.formula >= '0' && *parser.formula <= '9'){
	  return parseNumber();
	}
	else if(*parser.formula == '-'){
	  return parseNumber();
	}
	else if(*parser.formula == '('){
	  ++parser.formula;
	  double temp = parseSub();
	  ++parser.formula;
	  return temp;
	}
	//PI
	else if(*parser.formula == 'p'){
	  ++parser.formula;
	  if(*parser.formula == 'i'){
		++parser.formula;
		return M_PI;
	  }
	}
	//sin sqrt
	else if(*parser.formula == 's'){
	  ++parser.formula;
	  if(*parser.formula == 'i'){
		++parser.formula;
		if(*parser.formula == 'n'){
		  ++parser.formula;
		  if(*parser.formula == '('){
			  ++parser.formula;
			  double temp = parseSub();
			  temp = sin(temp*M_PI/180);
			  ++parser.formula;
			  return temp;
		  }

		}
	  }
	  else if(*parser.formula == 'q'){
		++parser.formula;
		if(*parser.formula == 'r'){
		  ++parser.formula;
		  if(*parser.formula == 't'){
			  ++parser.formula;
			  if(*parser.formula == '('){
			  ++parser.formula;
			  double temp = parseSub();
			  temp = sqrt(temp);
			  ++parser.formula;
			  return temp;
			}

		  }
		}
	  }
	}
	//cos
	else if(*parser.formula == 'c'){
	  ++parser.formula;
	  if(*parser.formula == 'o'){
  		++parser.formula;
  		if(*parser.formula == 's'){
  		  ++parser.formula;
  		  if(*parser.formula == '('){
    			++parser.formula;
    			double temp = parseSub();
    			temp = cos(temp*M_PI/180);
    			++parser.formula;
    			return temp;
  		  }
  		}
	  }
	}
	//tan
	else if(*parser.formula == 't'){
	  ++parser.formula;
	  if(*parser.formula == 'a'){
  		++parser.formula;
  		if(*parser.formula == 'n'){
  		  ++parser.formula;
  		  if(*parser.formula == '('){
    			++parser.formula;
    			double temp = parseSub();
    			temp = tan(temp*M_PI/180);
    			++parser.formula;
    			return temp;
  		  }
  		}
	  }
	}
	//asin acos atan
	else if(*parser.formula == 'a'){
	  ++parser.formula;
	  if(*parser.formula == 's'){
  		++parser.formula;
  		if(*parser.formula == 'i'){
  		  ++parser.formula;
  		  if(*parser.formula == 'n'){
    			++parser.formula;
    			if(*parser.formula == '('){
    			  ++parser.formula;
    			  double temp = parseSub();
    			  temp = asin(temp*M_PI/180);
    			  ++parser.formula;
    			  return temp;
    			}
  		  }
  		}
	  }
	  else if(*parser.formula == 'c'){
  		++parser.formula;
  		if(*parser.formula == 'o'){
  		  ++parser.formula;
  		  if(*parser.formula == 's'){
    			++parser.formula;
    			if(*parser.formula == '('){
    			  ++parser.formula;
    			  double temp = parseSub();
    			  temp = acos(temp*M_PI/180);
    			  ++parser.formula;
    			  return temp;
    			}
  		  }
  		}
	  }
	  else if(*parser.formula == 't'){
  		++parser.formula;
  		if(*parser.formula == 'a'){
  		  ++parser.formula;
  		  if(*parser.formula == 'n'){
    			++parser.formula;
    			if(*parser.formula == '('){
    			  ++parser.formula;
    			  double temp = parseSub();
    			  temp = atan(temp*M_PI/180);
    			  ++parser.formula;
    			  return temp;
    			}
  		  }
  		}
	  }
	  else if(*parser.formula == 'n'){
		  ++parser.formula;
  		if(*parser.formula == 's'){
  		  ++parser.formula;
  		  double temp = parser.prev_ans;
  		  return temp;
  		}
	  }
	}
	//exp
	else if(*parser.formula == 'e'){
	  ++parser.formula;
	  if(*parser.formula == 'x'){
  		++parser.formula;
  		if(*parser.formula == 'p'){
  		  ++parser.formula;
  		  if(*parser.formula == '('){
    			++parser.formula;
    			double temp = parseSub();
    			temp = exp(temp);
    			++parser.formula;
    			return temp;
  		  }
  		}
	  }
	}
	//ln log10
	else if(*parser.formula == 'l'){
	  ++parser.formula;
	  if(*parser.formula == 'n'){
  		++parser.formula;
  		if(*parser.formula == '('){
  		  ++parser.formula;
  		  double temp = parseSub();
  		  temp = log(temp);
  		  ++parser.formula;
  		  return temp;
  		}
	  }
	  else if(*parser.formula == 'o'){
  		++parser.formula;
  		if(*parser.formula == 'g'){
  		  ++parser.formula;
  		  if(*parser.formula == '('){
    			++parser.formula;
    			double temp = parseSub();
    			temp = log10(temp);
    			++parser.formula;
    			return temp;
  		  }
  		}
	  }
	}
	else{
	  printf("Syntax Error\n");
	  printf("Unknown symbol %c", *parser.formula);
	}
	return 0;
}

double parseNumber(){

	double number = 0;
	int neg_flag = 1;
	if(*parser.formula >= '0' && *parser.formula <= '9'){
	}
	else if(*parser.formula == '-'){
	  neg_flag = -1;
	  ++parser.formula;
	}
	else{
	  printf("Syntax Error\n");
	}


	while(*parser.formula >= '0' && *parser.formula <= '9'){
	  number = number * 10;
	  number = number + (int)(*parser.formula - '0');
	  ++parser.formula;
	}

	if(*parser.formula == '.'){
	  ++parser.formula;

	  //Check the next character is a number, else error

	  if(*parser.formula >= '0' && *parser.formula <= '9'){
		double weight = 1;
		while(*parser.formula >= '0' && *parser.formula <= '9'){
		  weight = weight / 10.0;
		  double scaled = (int)(*parser.formula - '0') * weight;
		  number = number + scaled;
		  ++parser.formula;
		}
	  }
	  else{
		printf("Syntax Error\n");
	  }
	}
	printf("Number Found: %lf\n", (number * neg_flag));
	return (number * neg_flag);
}

// STEPIEN: Added two touch panel functions to make the interface more
//          consistent with the LCD BSP.

#ifdef STM32F407xx
uint8_t BSP_TP_Init(void)
{
  // Initialise the interface and calibrate
  TP_Init(); // This is an empty function since done by STM32CubeMX
  TouchPanel_Calibrate();

  return 0;
}

uint8_t BSP_TP_GetDisplayPoint(Coordinate *pDisplay)
{
  Coordinate *pScreen;

  pScreen = Read_Ads7846();
  if (pScreen == NULL)
  {
    return 1; // Error reading the touch panel
  }
  if (getDisplayPoint(pDisplay, pScreen, &matrix ) == DISABLE)
  {
    return 1; // Error in LCD
  }
  return 0;
}
#endif
