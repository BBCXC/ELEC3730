#include <math.h>
#include <stdio.h>
#include <stdint.h>

#define M_PI 3.14159265358979323846

int main(){

	double coeff_values[] = {-0.026728, -0.000000,  0.050455,
							 -0.000000,  0.000000, -0.000000,
							 -0.075683,  0.000000,  0.062366,
							  0.000000,  0.093549, -0.000000,
							 -0.302731,  0.000000,  0.400000,
							  0.000000, -0.302731, -0.000000,
							  0.093549,  0.000000,  0.062366,
							  0.000000, -0.075683, -0.000000,
							  0.000000, -0.000000,  0.050455,
							 -0.000000, -0.026728};
	int coeff_num = 29;
	double val_cos;
	double val_sin;
	double magnitude;

	int f_min = 1;
	int f_max = 1000000;
	uint32_t f_val_new;

	for(uint32_t f_val=f_min; f_val<f_max; f_val *= 10){
		for(int i=1; i<10; i++){
			f_val_new = i * f_val;
			val_cos = 0;
			for(int n_c=0; n_c<coeff_num; n_c++){
				val_cos += cos((2*n_c*f_val_new	*M_PI)/coeff_num) * coeff_values[n_c];
			}


			// Real part of X[k]
	        Xre[k] = 0;
	        for (n=0 ; n<N ; ++n) Xre[k] += x[n] * cos(n * k * PI2 / N);


			val_sin = 0;
			for(int n_s=0; n_s<coeff_num; n_s++){
				val_sin += sin((2*n_s*f_val_new	*M_PI)/coeff_num) * coeff_values[n_s];
			}

			// Imaginary part of X[k]
	        Xim[k] = 0;
	        for (n=0 ; n<N ; ++n) Xim[k] -= x[n] * sin(n * k * PI2 / N);

			magnitude = sqrt((val_cos * val_cos) + (val_sin * val_sin));

			P[k] = Xre[k]*Xre[k] + Xim[k]*Xim[k];


			printf("Magnitude of frequency %i : %lf\n", f_val_new, magnitude);

			
		}
	}
	return 0;
}

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