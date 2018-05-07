#include "Ass-02-Equation.h"

void Equation_Init(void){
	output.prev_ans = 120.04;
	output.result = 24507.064;
	output.pos = 0;
}

int Get_Size(void){
	return(equation.size);
}

char **Get_Input(void){
	return(equation.input);
}

int Get_Pos(void){
	return(equation.pos);
}

void Set_Size(int Value){
	equation.size = Value;
}

void Set_Input(char **Value){
	equation.input = Value;
}

void Set_Pos(int Value){
	equation.pos = Value;
}



char Get_Formula_c(void){
	return(output.formula[output.pos]);
}

char* Get_Formula(void){
	return(output.formula);
}

double Get_Result(void){
	return(output.result);
}

double Get_Prev_ans(void){
	return(output.prev_ans);
}

void Set_Formula(char *Value){
	output.formula = Value;
	printf("Formula Expected %s\n", output.formula);
}

void Set_Result(double Value){
	output.result = Value;
}

void Set_Prev_ans(double Value){
	output.prev_ans = Value;
}

int Increment_Formula(){
	return(output.pos+=1);
}

void Reset_Formula_pos(void){
	output.pos = 0;
}

