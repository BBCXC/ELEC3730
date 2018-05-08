#include "Ass-02-Debug.h"

void Debug_Init(void){

	// Initialise
	info.debug = 1;
	info.system = 1;
	info.formula_mode = 1;
	info.first_time = 1;
}
int Get_Debug(void){
	return(info.debug);
}

int Get_System(void){
	return(info.system);
}

int Get_Formula_Mode(void){
	return(info.formula_mode);
}

int Get_First_Time(void){
	return(info.first_time);
}

void Set_Debug(int Value){
	info.debug = Value;
}

void Set_System(int Value){
	info.system = Value;
}

void Set_Formula_Mode(int Value){
	info.formula_mode = Value;
}

void Set_First_Time(int Value){
	info.first_time = Value;
}
