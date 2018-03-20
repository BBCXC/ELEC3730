//     $Date: 2017-03-13 08:12:38 +1100 (Mon, 13 Mar 2017) $
//     $Revision: 821 $
//     $Author: Peter $

#include "Ass-01.h"

int filter(char *filter_filename, char *input_wavefilename, char *output_wavefilename){

  int coeff_num = 0;
  double *coeff_values; // Array of filter coefficient values

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

    printf("\n");

    int input_len = header.Subchunk2Size;
    char iteration;
    double output;
    char output_fixed[input_len];

    int input[input_len/2];

    for (int i=0; i<(input_len/2); i++){
    	input[i] = data[i*2]+(data[i*2+1]<<8);
    }

    for(int i=0; i<input_len/2; i++){ //For each sample in the file
      if ((i+1)<coeff_num) iteration = i+1;
      else iteration = coeff_num;

      output = 0;

      for(int j=0; j<iteration; j++){ //For each coefficient filer the input file
        output = output + coeff_values[j] * input[i-j]; //Iterate through each buffer value
      }
      output_fixed[i*2] = (char)(int)output;
      output_fixed[i*2+1] = (char)((int)output>> 8);
    }


//    int input_len = header.Subchunk2Size;
//    int j = 0;
//    int i = 0;
//    double output;
//    char output_fixed[input_len];
//    int input = 0;
//
//    for(i=0; i<input_len; i+=2){
//    	output = 0;
//
//    	for(j=0; j<coeff_num; j++){ //For each coefficient filer the input file
//           if((i-j)<0) input = 0;
//           else input = data[i-j] + (data[i-j+1] << 8);
//           output = output + coeff_values[j] * input; //Iterate through each buffer value
//    	}
//
//    	output_fixed[i] = (char)(int)output;
//    	output_fixed[i+1] = (char)((int)output>> 8);
//    }




    write_pcm_wavefile(&header, output_fixed, output_wavefilename); //Write the new data to the new file TODO

    printf("\n");

  }
  else{
    free(coeff_values);
    return -1;  //Not successful in reading coefficients
  }
  free(coeff_values);
  return 0; //Success
}

