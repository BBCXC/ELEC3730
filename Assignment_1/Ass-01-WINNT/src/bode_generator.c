#include "Ass-01.h"

#include <complex.h>
const float complex _Complex_I

int main(char *filter_filename, char *input_wavefilename, char *output_wavefilename){

  int coeff_num = 0;
  double *coeff_values; // Array of filter coefficient values
  pcm_wavefile_header_t header;
  char *data;

  if (read_coefficients(&coeff_num, &coeff_values, filter_filename)==0){  //If success in reading coefficients
    if (coeff_num <= 0) return -1;  //No coefficients in filter

    printf("\n");

    int input_len = header.Subchunk2Size;
    int divisor = header.Subchunk1Size / 8;
    char iteration;
    double output;
    char output_fixed[input_len];
    int input[input_len/divisor];
    double X[coeff_num];

    for(int K=0; K<coeff_num; K++){
      for(int n=0; n<coeff_num; n++){
        X[k] = X[k] + x[n] * exp(((-2 * pi() * _Complex_I) / coeff_values) * k * n);
      }
      printf("X[k], %lf", X[k]);
    }

  }
  free(coeff_values);
  return 0; //Success
}
