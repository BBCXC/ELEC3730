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

	if(file_p == 0){
		printf("ERROR: File unknown \n");	//If file pointer returns NULL
		fclose(file_p);
		return -1;
	}

	fread(&coeff_num, sizeof(int), 1, file_p);	//Read in first 4 bytes

	fseek(file_p, 0, SEEK_END);	//Go to the end of the file
	long size_file = ftell(file_p);	//Count bytes in remaining file

	if((size_file - sizeof(int)) % coeff_num != 0){	//If remaining bytes doesn't add up TODO this should be (((size_file - 4)/coeff_num) == sizeof(double))
		printf("ERROR: File size incorrect for declaration, declaration: %i, file size: %ld \n",coeff_num, size_file);
		fclose(file_p);
		return -1;
	}

	else{
		printf("\n");
		printf("coeff_num : %i \n", coeff_num);
		printf("File size: %ld \n", size_file);
	}

	fseek(file_p, sizeof(int), SEEK_SET);	//Go back to position after int

	double* coeff_values = (double*) malloc(8 * coeff_num);	//Allocate memory
	if(coeff_values == 0){	//If malloc failes returns NULL ptr
		printf("ERROR: Memory allocation failed, File : %s \n", filename);
		fclose(file_p);
		free(coeff_values);	//TODO Shouldn't be needed
		return -1;
	}

	//Store values here
	for(int i = 0; i < coeff_num; i++){
		fread(&coeff_values[i], 8, 1, file_p);	//For the number of coefficients, read the value
		printf("Value of : %i : %lf \n", i, coeff_values[i]);
	}

	*coeff_num_p = coeff_num;	//Return ptr to values
	*coeff_values_p = &coeff_values[0];	//Return ptr to values
	fclose(file_p);
	return 0;
}
