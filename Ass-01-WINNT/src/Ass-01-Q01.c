//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 1

#include "Ass-01.h"

int read_coefficients(int *coeff_num_p, double **coeff_values_p, char *filename)
{
	FILE * file_p;
	int coeff_num;	//little endian?

	file_p = fopen(filename, "rb");	//open file for reading

	if(file_p == 0){
		printf("ERROR: File unknown \n");
		fclose(file_p);
		return -1;
	}

	fread(&coeff_num, sizeof(int), 1, file_p);

	fseek(file_p, 0, SEEK_END);
	long size_file = ftell(file_p);

	if((size_file - sizeof(int)) % coeff_num != 0){
		printf("ERROR: File size incorrect for declaration, declaration: %i, file size: %ld \n",coeff_num, size_file);
		fclose(file_p);
		return -1;
	}

	else{
		printf("\n");
		printf("coeff_num : %i \n", coeff_num);
		printf("File size: %ld \n", size_file);
	}

	fseek(file_p, sizeof(int), SEEK_SET);

	double* coeff_values = (double*) malloc(8 * coeff_num);
	if(coeff_values == 0){
		printf("ERROR: Memory allocation failed, File : %s \n", filename);
		fclose(file_p);
		free(coeff_values);
		return -1;
	}

	//store values here
	for(int i = 0; i < coeff_num; i++){
		fread(&coeff_values[i], 8, 1, file_p);

		printf("Value of : %i : %lf \n", i, coeff_values[i]);
	}

	*coeff_num_p = coeff_num;
	*coeff_values_p = &coeff_values[0];
	fclose(file_p);
	return 0;
}
