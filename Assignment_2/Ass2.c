char* input = ("1", "+", "2");
double output = 0;

char delim_paran_l = '(';
char delim_paran_r = ')';


int main(){
	output = equ_solver();
}

double equ_solver(input, input_len){

	for(index1=0; index1<input_len; index1++){
		if(input[index1] == delim_paran_l){
			paran_index_l = index1;
			for(index2=index1; index2<input_len; index2++){
				if(input[index2] == delim_paran_r){
					paran_index_r = index2
					*input_new = input[index1]; 
					output = equ_solver(input_new, index2 - index1);
				}
			}			
		}
	}

}



double equ_solver_2(operand, num_1, num_2){

}

double equ_solver(operator, num_1, num_2){

	switch(operator){
		case add: 
			result = (double) (num_1 + num_2);
			break;

		case sub: 
			result = (double) (num_1 - num_2);
			break;

		case multi: 
			result = (double) (num_1 * num_2);
			break;

		case div: 
			result = ((double)num_1) / ((double)num_2);
			break;
	}
}

recursive descent parser.