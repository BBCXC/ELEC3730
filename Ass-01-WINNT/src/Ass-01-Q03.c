//     $Date: 2017-03-13 08:12:38 +1100 (Mon, 13 Mar 2017) $
// $Revision: 821 $
//   $Author: Peter $

#include "Ass-01.h"

int filter(char *filter_filename, char *input_wavefilename, char *output_wavefilename)
{


	int coeff_num = 0;
	double *coeff_values; // Array of coefficient values

	printf("--> %19s: ", filter_filename);
	if (read_coefficients(&coeff_num, &coeff_values, filter_filename)==0){
		printf("coeff_num = %d \n", coeff_num);
		if (coeff_num > 0){
			printf("coeff_values[0] = %lf \n", coeff_values[0]);
			printf("coeff_values[1] = %lf \n", coeff_values[1]);
			printf("coeff_values[2] = %lf \n", coeff_values[2]);
			printf("coeff_values[3] = %lf \n", coeff_values[3]);
		}
		else{
			return -1;
		}
		double* circular_buffer = (double*) malloc(8 * coeff_num);

		for(int idiot=0; idiot<coeff_num; idiot++){
			circular_buffer[idiot] = 0.0;	//TODO change this
		}

		/*---------------------------------------Question 2---------------------------------------*/

		pcm_wavefile_header_t header;
		char filename_output[100];
		char *data;
		printf("-> Question 2...\n");
		printf("--> %20s: ",input_wavefilename);
		if (read_pcm_wavefile(&header, &data, input_wavefilename)==0){
			if (strcmp(input_wavefilename,"8k16bitpcm.wav")==0){
				snprintf(filename_output,100,"%s-%s",input_wavefilename, input_wavefilename);
				printf("--> Write %s", filename_output);
				write_pcm_wavefile(&header, data, filename_output);
			}
			free(data);
		}

		/*---------------------------------------Question 2---------------------------------------*/

		int j = 0;
		int i = 0;
		double y[1];
		for(i=0; i<1; i++){
			if(i < coeff_num){
				circular_buffer[0] = coeff_values[i];
			}
			for(j=0; j<coeff_num; j++){
				output_file[i] = output_file[i] + circular_buffer[j] * data[(i - j) % coeff_num];
			}
			if(i < coeff_num){
				void move_buffer(circular_buffer, coeff_num);
			}
		}
		printf("\n");
		//
		// WRITE CODE HERE
		//
		printf("CODE TO BE WRITTEN...\n");
	}

	  return 1;
}

void move_buffer(double *circular_buffer, int *coeff_num){
	for(int k=coeff_num; k>0; k--){
		circular_buffer[k] = circular_buffer[k-1];
	}
}
