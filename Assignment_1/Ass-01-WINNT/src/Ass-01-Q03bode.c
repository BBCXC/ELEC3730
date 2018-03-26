/*
 * Author 	   : Mitchell Marotta C3258958
 * 				 Taylor Young C3206230
 * Date	  	   : 23 March 2018
 * Description : Applys given filter to given wavefile
 * 				 Outputs new filtered wavefile
*/

#include "Ass-01.h"

#define PI2 6.2832

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
int filter2(char *filter_filename, char *input_wavefilename, char *output_wavefilename){

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

    //write_pcm_wavefile(&header, output_fixed, output_wavefilename); //Write the new data to the new file TODO
    printf("\n");

    int N = input_len/(100* divisor);
     // time and frequency domain data arrays
    int n, k;             // indices for time and frequency domains
    double Xre[N], Xim[N]; // DFT of x (real and imaginary parts)
    double P[N];           // power spectrum of x
     
    // Generate random discrete-time signal x in range (-1,+1)
     
    // Calculate DFT of x using brute force
    for (k=0 ; k<N ; ++k)
    {
        // Real part of X[k]
        Xre[k] = 0;
        for (n=0 ; n<N ; ++n) Xre[k] += output[n] * cos(n * k * PI2 / N);
         
        // Imaginary part of X[k]
        Xim[k] = 0;
        for (n=0 ; n<N ; ++n) Xim[k] -= output[n] * sin(n * k * PI2 / N);
         
        // Power at kth frequency bin
        P[k] = Xre[k]*Xre[k] + Xim[k]*Xim[k];
    }

    // Output results to MATLAB / Octave M-file for plotting
    FILE *f;
    f = fopen("dftplots.m", "w");

    if(f == 0){  //If file pointer returns NULL
        printf("%3s ERROR: MATLAB \n", " ");  //Log Error
        if(freopen("dftplots.m", "w", f) !=0) {
            printf("%3s ERROR : Unable to create file", " ");
            return -1;
        }
        printf("Created New File : %s\n", "dftplots.m");
        printf("\n");
    }
    else{
    	printf("Successfully opened file\n");
    }

    fprintf(f, "n = [0:%d];\n", N-1);
    fprintf(f, "x = [ ");
    for (n=0 ; n<N ; ++n) fprintf(f, "%f ", output[n]);
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


    printf("Here1");

    if(fclose(f) != 0) printf("%3s ERROR : fclose\n", " ");

    printf("Here2");
  }
  else{
    free(coeff_values);
    return -1;  //Not successful in reading coefficients
  }
  free(coeff_values);
  return 0; //Success
}
