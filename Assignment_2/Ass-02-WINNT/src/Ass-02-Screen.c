#include "Ass-02-Screen.h"

void Screen_Init(void){
	static char *item[42] = {"=", "+", "ANS", ".", "0", ">", "-", "3", "2", "1",
	                           "<", "/", "6", "5", "4", "AC", "x", "9", "8", "7", "DEL",
	                           "=", "(", "ANS", "sqrt(", "^", ">", ")", "atan(", "acos(", "asin(",
	                           "<", "pi", "tan(", "cos(", "sin(", "AC", "", "log(", "ln(", "exp(", "DEL"};
	//Store that array of strings in the struct
	grid_space_p.items = item;
}

int Get_Area(int pos1, int pos2){
	return(grid_space_p.Area[pos1][pos2]);
}

char **Get_Item(int Item_Value){
	return(grid_space_p.items[Item_Value]);
}

void Set_Area(int pos1, int pos2, int value){
	grid_space_p.Area[pos1][pos2] = value;
}

void Set_Item(int pos, int Item_number){
	grid_space_p.items[pos] = Item_number;
}