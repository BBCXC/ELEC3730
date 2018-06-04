#include "Ass-03-Input.h"
#include "Ass-03.h"

void inputInit() {

	osMutexWait(input_Handle, osWaitForever);
	record.record_time = 0;
	record.delay_time = 0;
	osMutexRelease(input_Handle);
}

int Get_Record_Time(){
	return record.record_time;
}
int Get_Record_Delay(){
	return record.delay_time;
}

void Set_Delay_Time(int Value){
	record.delay_time = Value;
}

void Set_Record_Time(int Value){
	record.record_time = Value;
}
