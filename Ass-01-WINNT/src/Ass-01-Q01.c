//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 1

#include "Ass-01.h"

int read_coefficients(int *coeff_num_p, double **coeff_values_p, char *filename)
{

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

	FILE *fopen( const char * filename, "rb");	//open file for reading
	int coeff_num = 0;	//? little endian
	coeff_num_p = coeff_num;

	float coeff_values = *malloc(coeff_num * 8);
	if(coeff_values == NULL){
		printf("Memory allocation failed, File : %s", filename);
		return -1;
	}
	//store values here
	if(){//!eof
		return -1;
	}
	else if(){//if last float is not full
		return -1;
	}
	else if(){//something else

	}
	return 0;
	//
	// WRITE CODE HERE
	//
	printf("CODE TO BE WRITTEN...\n");
	return 1;
}
