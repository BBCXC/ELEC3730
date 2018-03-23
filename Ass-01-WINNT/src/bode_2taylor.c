
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#define N 29
#define PI2 6.2832



#include "Ass-01.h"

/********************************************************************************************************************/
/*                                                 Filter Function                                                  */
/********************************************************************************************************************/

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
    double output[input_len/divisor];
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

    //For each sample in the file
    for(int i=0; i<input_len/divisor; i++){
      if ((i+1)<coeff_num) iteration = i+1; //Decide if the input exists
      else iteration = coeff_num; //Set input to coefficient

      output[i] = 0;

      for(int j=0; j<iteration; j++){ //For each coefficient filer the input file
        output[i] = output[i] + coeff_values[j] * input[i-j]; //Iterate through each buffer value
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


    // Output results to MATLAB / Octave M-file for plotting
    FILE *f = fopen("dftplots.m", "w");
    fprintf(f, "n = [0:%d];\n", N-1);
    fprintf(f, "x = [ ");
    for (n=0 ; n<N ; ++n) fprintf(f, "%f ", x[n]);
    fprintf(f, "];\n");
    fprintf(f, "Xre = [ ");
    for (k=0 ; k<N ; ++k) fprintf(f, "%f ", Xre[k]);
    fprintf(f, "];\n");
    fprintf(f, "Xim = [ ");
    for (k=0 ; k<N ; ++k) fprintf(f, "%f ", Xim[k]);
    fprintf(f, "];\n");
    fprintf(f, "P = [ ");
    for (k=0 ; k<N ; ++k) fprintf(f, "%f ", P[k]);
    fprintf(f, "];\n");
    fprintf(f, "subplot(3,1,1)\nplot(n,x)\n");
    fprintf(f, "xlim([0 %d])\n", N-1);
    fprintf(f, "subplot(3,1,2)\nplot(n,Xre,n,Xim)\n");
    fprintf(f, "xlim([0 %d])\n", N-1);
    fprintf(f, "subplot(3,1,3)\nstem(n,P)\n");
    fprintf(f, "xlim([0 %d])\n", N-1);
    fclose(f);


    printf("\n");
  }
  else{
    free(coeff_values);
    return -1;  //Not successful in reading coefficients
  }
  free(coeff_values);
  return 0; //Success
}




int main(void){

    char *filename_filter[] = {"filterBP.bin","filterHP.bin","filterLP.bin","filterSB.bin",
                             "filter-Bad-0.bin","filter-Bad-1.bin","filter-Bad-2.bin",NULL};
    char *filename_audio[] = {"8k16bitpcm.wav", "11k8bitpcm.wav","11k16bitpcm.wav",
                            "8kmp38.wav","8k16bitpcm-Bad-0.wav",
                            "8k16bitpcm-Bad-1.wav","8k16bitpcm-Bad-2.wav","8k16bitpcm-Bad-3.wav",NULL};
    // Loop variables
    int i;
    int j;


    char filename_output[100];
    printf("-> Question 3...\n");
    for (i=0;i<4;i++)
    {
      printf("--> File name %4s = %s: \n", " ", filename_filter[i]);
      snprintf(filename_output,100,"%s-%s",filename_filter[i],filename_audio[0]);
      if (filter(filename_filter[i], filename_audio[0], filename_output)==0)
      {
        printf("--> Written %s.\n", filename_output);
      }
    }


    // time and frequency domain data arrays
    int n, k;             // indices for time and frequency domains
    float Xre[N], Xim[N]; // DFT of x (real and imaginary parts)
    float P[N];           // power spectrum of x
     
    // Generate random discrete-time signal x in range (-1,+1)
    float x[29] = {-0.026728, -0.000000,  0.050455,
                             -0.000000,  0.000000, -0.000000,
                             -0.075683,  0.000000,  0.062366,
                              0.000000,  0.093549, -0.000000,
                             -0.302731,  0.000000,  0.400000,
                              0.000000, -0.302731, -0.000000,
                              0.093549,  0.000000,  0.062366,
                              0.000000, -0.075683, -0.000000,
                              0.000000, -0.000000,  0.050455,
                             -0.000000, -0.026728};
     
    // Calculate DFT of x using brute force
    for (k=0 ; k<N ; ++k)
    {
        // Real part of X[k]
        Xre[k] = 0;
        for (n=0 ; n<N ; ++n) Xre[k] += x[n] * cos(n * k * PI2 / N);
         
        // Imaginary part of X[k]
        Xim[k] = 0;
        for (n=0 ; n<N ; ++n) Xim[k] -= x[n] * sin(n * k * PI2 / N);
         
        // Power at kth frequency bin
        P[k] = Xre[k]*Xre[k] + Xim[k]*Xim[k];
    }
     
    
     
    // exit normally
    return 0;
}