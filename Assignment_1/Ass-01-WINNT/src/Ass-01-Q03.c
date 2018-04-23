/*
 * Author 	   : Mitchell Marotta C3258958
 * 				 Taylor Young C3206230
 * Date	  	   : 23 March 2018
 * Description : Applys given filter to given wavefile
 * 				 Outputs new filtered wavefile
*/

#include "Ass-01.h"

/********************************************************************************************************************/
/*                                                 Filter Function                                                  */
/********************************************************************************************************************/

/*
 * Input  : Pointer to filter name
 * 			Pointer to input wavefile
 * 			Pointer to output file
 * Output : New filtered wavefile
 * 			Success returns 0; Failure returns -1
 */
int filter(char *filter_filename, char *input_wavefilename, char *output_wavefilename){

  int coeff_num = 0;
  double *coeff_values; //Array of filter coefficient values
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
      else if(header.Subchunk1Size == 8){
        //input[i] = data[i];
        printf("Only 16 bit files supported\n");
        printf("ERROR: 8 bit functionality Currently in development\n");
        return -1;
      }
      else{
        printf("ERROR: Incorrect Subchunk1Size\n");
        return -1;
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
        printf("Only 16 bit files supported\n");
        printf("ERROR: 8 bit functionality Currently in development\n");
        return -1;
      }
      else{
        printf("ERROR: Incorrect Subchunk1Size\n");
        return -1;
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
