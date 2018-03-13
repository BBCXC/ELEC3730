//     $Date: 2017-03-13 08:12:38 +1100 (Mon, 13 Mar 2017) $
//     $Revision: 821 $
//     $Author: Peter $

#include "Ass-01.h"

int move_buffer(double* circular_buffer, int *coeff_num){
	for(int k=*coeff_num; k>0; k--){
		circular_buffer[k] = circular_buffer[k-1];	//Rotate buffer to right
	}
	return 0;
}

int filter(char *filter_filename, char *input_wavefilename, char *output_wavefilename){

	int coeff_num = 0;
	double *coeff_values; // Array of coefficient values

	if (read_coefficients(&coeff_num, &coeff_values, filter_filename)==0){	//If success in reading coefficients
		if (coeff_num <= 0) return -1;	//No coefficients in filter

		pcm_wavefile_header_t header;
		char *data;
		if (read_pcm_wavefile(&header, &data, input_wavefilename)==0){
			if (strcmp(input_wavefilename,"8k16bitpcm.wav")!=0) return -1;	//Error?? TODO figure this out
		}
		else{
			printf("%3s ERROR: Unable to read '.wav' file\n\n", " ");	//Unable to read wavefile
			return -1;
		}

		//If you got here then things seem good so far
		double* circular_buffer = (double*) malloc(8 * coeff_num);	//Allocate enough memory for the circular buffer

		for(int idiot=0; idiot<coeff_num; idiot++){	//Initialise array to 0.0 for circular buffing
			circular_buffer[idiot] = 0.0;	//TODO change this
		}

		int j = 0;
		int i = 0;
		double output_current;	//Current output value to be inserted into data of output wavefile

		int Wave_length = 0;	//TODO this should contain a value

		for(i=0; i<Wave_length; i++){	//For each sample in the file
			if(i < coeff_num){	//Load the buffer with the appropriate coefficient
				circular_buffer[0] = coeff_values[i];	//Set first buffer value to next coefficient
			}
			for(j=0; j<coeff_num; j++){	//For each coefficient filer the input file
				output_current = output_current + circular_buffer[j] * data[(i - j) % coeff_num];	//Itterate through each buffer value
			}
			write_pcm_wavefile(&header, data, output_wavefilename);	//Write the new data to the new file TODO
			if(i < coeff_num){	//If buffer isn't full
				if(move_buffer(circular_buffer, &coeff_num)!=0){	//Rotate the buffer
					printf("%3s ERROR: Move buffer failed\n", " ");	//Log error if buffer failed to rotate
					return -1;
				}
			}
		}
		printf("\n");

	}
	else return -1;	//Not successful in reading coefficients

	return 0;	//Success
}
