#include "Ass-01.h"

int filter(char *filter_filename, char *input_wavefilename, char *output_wavefilename){

  int coeff_num = 0;
  double *coeff_values; // Array of filter coefficient values
  pcm_wavefile_header_t header;
  char *data;

  if (read_coefficients(&coeff_num, &coeff_values, filter_filename)==0){  //If success in reading coefficients
    if (coeff_num <= 0) return -1;  //No coefficients in filter

    if (read_pcm_wavefile(&header, &data, input_wavefilename) != 0){
      printf("%3s ERROR: Unable to read '.wav' file\n\n", " "); //Unable to read wavefile
      free(data);
      free(coeff_values);
      return -1;
    }

    if(header.Subchunk1Size != 16){
      printf("Only 16 bit files supported\n");
    }

    printf("\n");

    int input_len = header.Subchunk2Size;
    int divisor = header.Subchunk1Size / 8;
    char iteration;
    double output;
    char output_fixed[input_len];
    int input[input_len/divisor];

    //Create new array with combined values as per header
    for (int i=0; i<(input_len/divisor); i++){
      if(header.Subchunk1Size == 16){
        input[i] = data[i*2]+(data[i*2+1]<<8);
      }
      if(header.Subchunk1Size == 8){
        //input[i] = data[i];
        printf("Currently in development\n");
      }
    }

    //For each sample in the file
    for(int i=0; i<input_len/divisor; i++){
      if ((i+1)<coeff_num) iteration = i+1; //Decide if the input exists
      else iteration = coeff_num; //Set input to coefficient

      output = 0;

      for(int j=0; j<iteration; j++){ //For each coefficient filer the input file
        output = output + coeff_values[j] * input[i-j]; //Iterate through each buffer value
      }
      
      if(header.Subchunk1Size == 16){
        output_fixed[i*2] = (char)(int)output; //Split output into fixed point 8 bit numbers
        output_fixed[i*2+1] = (char)((int)output>> 8); //Split output into fixed point 8 bit numbers
      }
      else if(header.Subchunk1Size == 8){
        //output_fixed[i] = (char)(int)output;
        printf("Currently in development\n");
      }
    }

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
