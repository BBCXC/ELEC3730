//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 1

#include "Ass-01.h"

int read_coefficients(int *coeff_num_p, double **coeff_values_p, char *filename)
{
	FILE * file_p;	//File
	int coeff_num;	//Little endian? TODO

	file_p = fopen(filename, "rb");	//Open file for reading

	if(file_p == 0){	//If file pointer returns NULL
		printf("%3s ERROR: File unknown \n", " ");	//Log Error
		fclose(file_p);	//Close file
		printf("\n");
		return -1;	//Return Failed
	}

	fread(&coeff_num, sizeof(int), 1, file_p);	//Read in first 4 bytes

	fseek(file_p, 0, SEEK_END);	//Go to the end of the file
	long size_file = ftell(file_p);	//Count bytes in remaining file

	if((size_file - sizeof(int)) % coeff_num != 0){	//If remaining bytes doesn't add up TODO this should be (((size_file - 4)/coeff_num) == sizeof(double))
		printf("%3s ERROR: File size incorrect for declaration, declaration: %i, file size: %ld \n"," ", coeff_num, size_file);
		fclose(file_p);	//Close file
		printf("\n");
		return -1;	//Return Failed
	}

	else printf("--> Array size %3s = %d\n", " ", coeff_num);	//All seems good print size of array

	fseek(file_p, sizeof(int), SEEK_SET);	//Go back to position after int

	double* coeff_values = (double*) calloc(coeff_num, 8);	//Allocate memory
	if(coeff_values == 0){	//If malloc fails returns NULL ptr
		printf("%3s ERROR: Memory allocation failed, File : %s \n"," ", filename);	//Log Error
		fclose(file_p);	//Close file
		printf("\n");
		free(coeff_values);	//TODO Shouldn't be needed
		return -1;	//Return Failed
	}

	//Store values here
	printf("--> Array contents =\n");
	for(int i = 0; i < coeff_num; i++){	//For the number of expected values
		fread(&coeff_values[i], 8, 1, file_p);	//Read the value into the array
	}


	//Pretty printing not really needed
	for(int i=0; i<coeff_num; i=i+3){	//Loop in groups of 3
		for(int k=i; k<i+3; k++){	//For each group of 3
			if(k<coeff_num){	//Check we haven't accidentally passed further than the array
				if(coeff_values[k]<0)printf("%3s %lf", " ", coeff_values[k]);	//If negative use less spaces
				else printf("%4s %lf", " ", coeff_values[k]);	//If positive use more spaces
			}
		}
		printf("\n");	//Print the line end for the group
	}
	printf("\n");



	*coeff_num_p = coeff_num;	//Return values to ptr
	*coeff_values_p = &coeff_values[0];	//Return values to ptr
	fclose(file_p);	//Close file
	return 0;	//Return Success!
}
