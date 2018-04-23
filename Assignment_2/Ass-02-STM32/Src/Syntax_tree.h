#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct{
	char* input;
	double result;

}
	grid_struct;

	grid_struct grid_space_p;

extern double parseFormula();
extern double parseSub();
extern double parseSum();
extern double parsePro();
extern double parseDiv();
extern double parsePow();
extern double parseFactor();
extern double parseNumber();