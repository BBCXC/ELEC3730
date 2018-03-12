//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 1

#include "Ass-01.h"

int read_coefficients(int *coeff_num_p, double **coeff_values_p, char *filename)
{
	FILE * file_p;
	int coeff_num;	//? little endian

	//	The first four bytes contain an integer n which is in stored in little endian format which represents the number
	//	of filter coefficients.
	//	The next 8n bytes contain an array of n floating point numbers stored as 8 byte double precision which
	//	represent the filter coefficients.


	//read first 4 bytes little endian number of coefficients // pass back to coeff_num_p
				//malloc for 8n stored in coeff_values_p array of n 8 byte numbers
				//if successful reading return 0 // Below is definition of success (from me)
					//eof
					//whole number of coefficients in 8 byte lots
					//
				//else return -1

	file_p = fopen(filename, "rb");	//open file for reading

	if(file_p == 0){
		printf("ERROR: File unknown \n");
		return -1;
	}

	//coeff_num_p = coeff_num;
	fread(&coeff_num, 4, 1, file_p);

	fseek(file_p, 0, SEEK_END);
	long size_file = ftell(file_p);

	if((size_file - 4) % coeff_num != 0){
		printf("ERROR: File size incorrect for declaration, declaration: %i, file size: %ld \n",coeff_num, size_file);
		return -1;
	}
	else{
		printf("coeff_num : %i \n", coeff_num);
		printf("File size: %ld \n", size_file);
	}
	fseek(file_p, 4, SEEK_SET);

	double * coeff_values = (double *) malloc(8 * coeff_num);
	if(coeff_values == 0){
		printf("ERROR: Memory allocation failed, File : %s \n", filename);
		return -1;
	}

	//store values here
	for(int i = 0; i < coeff_num; i++){
		fread(&coeff_values[i], 8, 1, file_p);

		printf("Value of : %i : %lf \n", i, coeff_values[i]);
	}

	return 0;
	//
	// WRITE CODE HERE
	//
	printf("CODE TO BE WRITTEN...\n");
	return 1;
}
