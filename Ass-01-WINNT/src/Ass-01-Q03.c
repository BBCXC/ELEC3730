//     $Date: 2017-03-13 08:12:38 +1100 (Mon, 13 Mar 2017) $
//     $Revision: 821 $
//     $Author: Peter $

#include "Ass-01.h"

/*
 *  Handle different lengths of coefficient files
 *  Negative inputs will be 0,
 *  yk must be converted back to a fixed point number before writing	()yk
 *  only read in number of uk values needed at a time
 *  current circular buffer can be improved
 *  whole data structure can be improved
 */

int filter(char *filter_filename, char *input_wavefilename, char *output_wavefilename){

  int coeff_num = 0;
  double *coeff_values; // Array of coefficient values

  if (read_coefficients(&coeff_num, &coeff_values, filter_filename)==0){  //If success in reading coefficients
    if (coeff_num <= 0) return -1;  //No coefficients in filter

    pcm_wavefile_header_t header;
    char *data;
    if (read_pcm_wavefile(&header, &data, input_wavefilename)==0){
      if (strcmp(input_wavefilename,"8k16bitpcm.wav")!=0) return -1;  //Error?? TODO figure this out
    }
    else{
      printf("%3s ERROR: Unable to read '.wav' file\n\n", " "); //Unable to read wavefile
      free(data);
      free(coeff_values);
      return -1;
    }

    //If you got here then things seem good so far
    double* circular_buffer = (double*) calloc(coeff_num, 8); //Allocate enough memory for the circular buffer to 0.0

    if(circular_buffer == 0){ //If malloc fails returns NULL ptr
      printf("%3s ERROR: Memory allocation failed circular_buffer \n", " ");  //Log Error
      printf("\n");
      free(circular_buffer);  //TODO Shouldn't be needed
      return -1;  //Return Failed
    }

    circular_buffer = coeff_values;
    for(int var=0; var<coeff_num; var++){
    	printf("circular_buffer %lf\n", circular_buffer[var]);
    }
    printf("\n");

    int input_len = header.SampleRate * header.NumChannels * header.BitsPerSample / 8;  //TODO this should contain a value

    int j = 0;
    int i = 0;
    double* output = (double*) calloc(input_len, sizeof(double));  //Current output value to be inserted into data of output wavefile

    if(output == 0){ //If malloc fails returns NULL ptr
          printf("%3s ERROR: Memory allocation failed output \n", " ");  //Log Error
          printf("\n");
          free(output);  //TODO Shouldn't be needed
          return -1;  //Return Failed
        }

    double input = 0;

    for(i=0; i<input_len; i++){ //For each sample in the file
      output[i] = 0;
      for(j=0; j<coeff_num; j++){ //For each coefficient filer the input file
        if((i-j)<0) input = 0.0;
        else input = 0.0;//data[(i - j)];
        output[i] = output[i] + circular_buffer[j] * input; //Iterate through each buffer value
      }
    }

    char* output_fixed = (char*) output;	//Convert back to fixed point TODO

    write_pcm_wavefile(&header, output_fixed, output_wavefilename); //Write the new data to the new file TODO

    free(circular_buffer);
    free(output);
    printf("\n");

  }
  else{
    free(coeff_values);
    return -1;  //Not successful in reading coefficients
  }
  free(coeff_values);
  return 0; //Success
}


//Effiecient Read

/*int filter(char *filter_filename, char *input_wavefilename, char *output_wavefilename){

  int coeff_num = 0;
  double *coeff_values; // Array of coefficient values

  if (read_coefficients(&coeff_num, &coeff_values, filter_filename)==0){  //If success in reading coefficients
    if (coeff_num <= 0) return -1;  //No coefficients in filter

    pcm_wavefile_header_t header;
    char *data;
    if (read_pcm_wavefile(&header, &data, input_wavefilename)==0){
      if (strcmp(input_wavefilename,"8k16bitpcm.wav")!=0) return -1;  //Error?? TODO figure this out
    }
    else{
      printf("%3s ERROR: Unable to read '.wav' file\n\n", " "); //Unable to read wavefile
      free(data);
      free(coeff_values);
      return -1;
    }

    //If you got here then things seem good so far
    double* circular_buffer = (double*) calloc(coeff_num, 8); //Allocate enough memory for the circular buffer to 0.0

    if(circular_buffer == 0){ //If malloc fails returns NULL ptr
      printf("%3s ERROR: Memory allocation failed circular_buffer \n", " ");  //Log Error
      printf("\n");
      free(circular_buffer);  //TODO Shouldn't be needed
      return -1;  //Return Failed
    }

    circular_buffer = coeff_values;

    int j = 0;
    int i = 0;
    double output_current = 0;  //Current output value to be inserted into data of output wavefile

    int input_len = 0;  //TODO this should contain a value

    for(i=0; i<input_len; i++){ //For each sample in the file
      output_current = 0;
      input_buffer[(i-1)-(coeff_num-1)] = read_pcm_wavefile[i]// Not sure on this TODO
      for(j=0; j<coeff_num; j++){ //For each coefficient filer the input file
        output_current = output_current + circular_buffer[j] * input_buffer[(i-1)-(coeff_num-1)-j]; //Itterate through each buffer value
      }
      write_pcm_wavefile(&header, data, output_wavefilename); //Write the new data to the new file TODO

    }
    free(circular_buffer);
    printf("\n");

  }
  else{
    free(coeff_values);
    return -1;  //Not successful in reading coefficients
  }
  free(coeff_values);
  return 0; //Success
}
*/




//Original

/*
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
			free(data);
			free(coeff_values);
			return -1;
		}

		//If you got here then things seem good so far
		double* circular_buffer = (double*) calloc(coeff_num, 8);	//Allocate enough memory for the circular buffer to 0.0

		if(circular_buffer == 0){	//If malloc fails returns NULL ptr
			printf("%3s ERROR: Memory allocation failed circular_buffer \n", " ");	//Log Error
			printf("\n");
			free(circular_buffer);	//TODO Shouldn't be needed
			return -1;	//Return Failed
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
					free(data);
					free(coeff_values);
					free(circular_buffer);
					return -1;
				}
			}
		}
		free(circular_buffer);
		printf("\n");

	}
	else{
		free(coeff_values);
		return -1;	//Not successful in reading coefficients
	}
	free(coeff_values);
	return 0;	//Success
}*/
