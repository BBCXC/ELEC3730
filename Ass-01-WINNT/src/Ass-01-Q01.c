#include "Ass-01.h"

int read_coefficients(int *coeff_num_p, double **coeff_values_p, char *filename)
{
	FILE * file_p;	//File
	int coeff_num;	//Number of ceofficients
	int pretty_print = 0;	//Toggles whether array prints

	file_p = fopen(filename, "rb");	//Open file for reading

	if(file_p == 0){	//If file pointer returns NULL
		printf("%3s ERROR: Unable to open file for reading\n", " ");	//Log Error
		printf("\n");
		return -1;	//Return Failed
	}

	if(fseek(file_p, 0, SEEK_END) != 0) printf("%3s ERROR : fseek", " ");	//Go to the end of the file
	long size_file = ftell(file_p);	//Count bytes in remaining file
	if(fseek(file_p, 0, SEEK_SET) != 0) printf("%3s ERROR : fseek", " ");	//Go back to position

	if(size_file <= 4){	//If file size too small
		printf("%3s ERROR: File too small, file size: %ld \n"," ", size_file);
		if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");	//Close file
		printf("\n");
		return -1;	//Return Failed
	}

	if(fread(&coeff_num, sizeof(int), 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");	//Read in first 4 bytes

	if((size_file - sizeof(int)) % coeff_num != 0){	//If remaining bytes doesn't add up TODO this should be (((size_file - 4)/coeff_num) == sizeof(double))
		printf("%3s ERROR: File size incorrect for declaration, declaration: %i, file size: %ld \n"," ", coeff_num, size_file);
		if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");	//Close file
		printf("\n");
		return -1;	//Return Failed
	}
	else printf("--> Array size %3s = %d\n", " ", coeff_num);	//All seems good print size of array

	double* coeff_values = (double*) calloc(coeff_num, 8);	//Allocate memory
	if(coeff_values == 0){	//If malloc fails returns NULL ptr
		printf("%3s ERROR: Memory allocation failed, File : %s \n"," ", filename);	//Log Error
		if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");	//Close file
		printf("\n");
		free(coeff_values);	//TODO Shouldn't be needed
		return -1;	//Return Failed
	}

	//Store values
	printf("--> Array contents =\n");

	if(fread(coeff_values, 8, coeff_num, file_p) != coeff_num) printf("%3s ERROR : fread\n", " ");	//Read the value into the array

	if(pretty_print == 1){
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
	}
	else printf("\n%4sNot pretty print\n", " ");

	*coeff_num_p = coeff_num;	//Return values to ptr
	*coeff_values_p = &coeff_values[0];	//Return values to ptr
	if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");	//Close file
	return 0;	//Return Success!
}
