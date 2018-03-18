/* ELEC3730 Question 2
Author: Mitchell Marotta
Date: 12/03/2018
Description: */



#include "Ass-01.h"
#include <string.h>

int read_pcm_wavefile(pcm_wavefile_header_t *header_p, char **data_p, char *filename){

	FILE* filevar;		/* variable of type file */
	pcm_wavefile_header_t header;
	const char* RIFF_string = "RIFF";
	const char* fmt_string = "fmt";
	int ui8_t_size = sizeof(uint8_t);
	int ui16_t_size = sizeof(uint16_t);
	int ui32_t_size = sizeof(uint32_t);

	filevar = fopen(filename, "rb");										/* opening the file and storing it in file variable */
	if(filevar == 0){  //If file pointer returns NULL
		printf("%3s ERROR: File unknown \n", " ");  //Log Error
		return -1;
	}

	fread(header.ChunkID, ui8_t_size, 4, filevar);					/* Reading the Chunk ID */
	if (strncmp(RIFF_string, (char*)header.ChunkID, 4) != 0){	/* Checking to see if the ID is correct */
		printf("%3s ERROR: Incorrect ChunkID expected: %s got: %c%c%c%c\n", " ", RIFF_string, header.ChunkID[0], header.ChunkID[1], header.ChunkID[2], header.ChunkID[3]);
		return -1;
	}
	fread(&header.ChunkSize, ui32_t_size, 1, filevar);

	fread(header.Format, ui8_t_size, 4, filevar);

	fread(header.Subchunk1ID, ui8_t_size, 4, filevar);
	if (strncmp(fmt_string, (char*)header.Subchunk1ID, 3) != 0) {
		printf("%3s ERROR: Incorrect Subchunk1ID expected: %s got: %c%c%c%c\n", " ", fmt_string, header.Subchunk1ID[0], header.Subchunk1ID[1], header.Subchunk1ID[2], header.Subchunk1ID[3]);
		return -1;
	}

	fread(&header.Subchunk1Size, ui32_t_size, 1, filevar);
	if (header. Subchunk1Size != 16) {
		fread(&header.AudioFormat, ui16_t_size, 1, filevar);
		if (header.AudioFormat != 1){
			printf("%3s ERROR: Not PCM encoded \n", " ");
		}
		return -1;
	}
	else{
		fread(&header.AudioFormat, ui16_t_size, 1, filevar);
	}

	fread(&header.NumChannels, ui16_t_size, 1, filevar);
	fread(&header.SampleRate, ui32_t_size, 1, filevar);
	fread(&header_p ->ByteRate, ui32_t_size, 1, filevar);
	fread(&header.BlockAlign, ui16_t_size, 1, filevar);
	fread(&header.BitsPerSample, ui16_t_size, 1, filevar);
	fread(header.Subchunk2ID, ui8_t_size, 4, filevar);
	fread(&header.Subchunk2Size, ui32_t_size, 1, filevar);


	/* Print Statements */
	printf("\n");
	printf("-> ChunkID = %c%c%c%c \n", header.ChunkID[0], header.ChunkID[1], header.ChunkID[2], header.ChunkID[3]);
	printf("-> ChunkSize = %d \n", header.ChunkSize);
	printf("-> Chunk Format = %c%c%c%c \n", header.Format[0], header.Format[1], header.Format[2], header.Format[3]);
	printf("-> Subchunk1ID = %c%c%c \n", header.Subchunk1ID[0], header.Subchunk1ID[1], header.Subchunk1ID[2]);
	printf("-> Subchunk1size = %d \n", header.Subchunk1Size);
	printf("-> AudioFormat = %d \n", header.AudioFormat);
	printf("-> Number of channels = %d \n", header_p ->NumChannels);
	printf("-> Sample rate = %d \n", header_p ->SampleRate);
	printf("-> ByteRate = %d \n", header_p ->ByteRate);
	printf("-> BlockAlign = %d \n", header_p ->BlockAlign);
	printf("-> BitsPerSample = %d \n", header_p ->BitsPerSample);
	printf("-> SubChuck2ID = %c%c%c%c \n", header.Subchunk2ID[0], header.Subchunk2ID[1], header.Subchunk2ID[2], header.Subchunk2ID[3]);
	printf("-> SubChunk2Size = %d  \n\n", header.Subchunk2Size);

	return 0;
}

int write_pcm_wavefile(pcm_wavefile_header_t *header_p, char *data, char *filename_output){ /* I changed the signature from *filename to *filename_output */

	FILE* filevar;		/* variable of type file */

	filevar = fopen(filename_output, "rb");										/* opening the file and storing it in file variable */

	if(filevar == 0){  //If file pointer returns NULL
	    printf("%3s ERROR: File unknown \n", " ");  //Log Error
	    if(freopen(filename_output, "wb", filevar) !=0) {
	        printf("%3s ERROR : Unable to create file", " ");
	        return -1;
	    }
	    printf("\n");
	}

return 0;
}
