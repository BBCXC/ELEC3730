/* ELEC3730 Question 2
Author: Mitchell Marotta
Date: 12/03/2018
Description: */



#include "Ass-01.h"

int read_pcm_wavefile(pcm_wavefile_header_t *header_p, char **data_p, char *filename)
{
	FILE * filevar;		/* variable of type file */
int flag, i;
char ArrayCheck1[4] = "RIFF";
char ArrayCheck2[3] = "fmt";

	filevar = fopen(filename, "rb");										/* opening the file and storing it in file variable */

	fread(header_p->ChunkID, sizeof(uint8_t), 4, filevar);					/* Reading the Chunk ID */
	i =strncmp(header_p->ChunkID, ArrayCheck1, 4);							/* Checking to see if the ID is correct */
	if (i != 0) {
		printf("ERROR! incorrect ChunkID \n");
		flag = 1;
	}
	fread(&header_p->ChunkSize, sizeof(uint32_t), 1, filevar);

	fread(header_p->Format, sizeof(uint8_t), 4, filevar);

	fread(header_p->Subchunk1ID, sizeof(uint8_t), 4, filevar);
	i = strncmp(header_p ->Subchunk1ID, ArrayCheck2, 3);
			if (i != 0) {
				printf("Error! Incorrect Subchunk2ID \n");
				flag = 1;
			}




	fread(&header_p->Subchunk1Size, sizeof(uint32_t), 1, filevar);

		if (header_p-> Subchunk1Size != 16) {
			fread(&header_p->AudioFormat, sizeof(uint16_t), 1, filevar);
				if (header_p->AudioFormat != 1)
					printf("Error! not PCM encoded \n");
		flag = 1;
		}
		else  {

	fread(&header_p->AudioFormat, sizeof(uint16_t), 1, filevar);

		}

	fread(&header_p->NumChannels, sizeof(uint16_t), 1, filevar);
	fread(&header_p->SampleRate, sizeof(uint32_t), 1, filevar);
	fread(&header_p ->ByteRate, sizeof(uint32_t), 1, filevar);
	fread(&header_p->BlockAlign, sizeof(uint16_t), 1, filevar);
	fread(&header_p->BitsPerSample, sizeof(uint16_t), 1, filevar);
	fread(header_p->Subchunk2ID, sizeof(uint8_t), 4, filevar);
	fread(&header_p->Subchunk2Size, sizeof(uint32_t), 1, filevar);


	/* Print Statements */
	if (flag != 1) {
	printf("\n -> ChunkID = %c%c%c%c \n", header_p->ChunkID[0], header_p->ChunkID[1], header_p->ChunkID[2], header_p->ChunkID[3]);
	printf("-> ChunkSize = %d \n", header_p->ChunkSize);
	printf("-> Chunk Format = %c%c%c%c \n", header_p->Format[0], header_p->Format[1], header_p->Format[2], header_p->Format[3]);
	printf(" -> Subchunk1ID = %c%c%c \n", header_p->Subchunk1ID[0], header_p->Subchunk1ID[1], header_p->Subchunk1ID[2]);
	printf("-> Subchunk1size = %d \n", header_p->Subchunk1Size);
	printf("-> AudioFormat = %d \n", header_p->AudioFormat);
	printf(" -> Number of channels = %d \n", header_p ->NumChannels);
	printf(" -> Sample rate = %d \n", header_p ->SampleRate);
	printf(" -> ByteRate = %d \n", header_p ->ByteRate);
	printf(" -> BlockAlign = %d \n", header_p ->BlockAlign);
	printf(" -> BitsPerSample = %d \n", header_p ->BitsPerSample);
	printf(" -> SubChuck2ID = %c%c%c%c \n", header_p->Subchunk2ID[0], header_p->Subchunk2ID[1], header_p->Subchunk2ID[2], header_p->Subchunk2ID[3]);
	printf("-> SubChunk2Size = %d  \n\n", header_p->Subchunk2Size);

}
	 return 1;
}

int write_pcm_wavefile(pcm_wavefile_header_t *header_p, char *data, char *filename_output) /* I changed the signature from *filename to *filename_output */
{

	if(file_p == 0){  //If file pointer returns NULL
	      printf("%3s ERROR: File unknown \n", " ");  //Log Error
	          if(freopen(*filename, "wb", file_p) !=0) {
	            printf("%3s ERROR : Unable to create file", " ");
	            return -1;
	          }
	      printf("\n");
	  }

return 1;
 }
