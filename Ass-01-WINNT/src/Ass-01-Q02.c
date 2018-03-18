/* ELEC3730 Question 2
Author: Mitchell Marotta
Date: 12/03/2018
Description: */



#include "Ass-01.h"

int read_pcm_wavefile(pcm_wavefile_header_t *header_p, char **data_p, char *filename) /* use the size of function everywhere that there is a 4 */
{
	FILE * filevar;		/* variable of type file */


	filevar = fopen(filename, "rb");										/* opening the file and storing it in file variable */
	fread(header_p->ChunkID, sizeof(uint8_t), 4, filevar);					/* Reading the first contents */
	printf("\n -> ChuckID = %c%c%c%c \n", header_p->ChunkID[0], header_p->ChunkID[1], header_p->ChunkID[2], header_p->ChunkID[3]); 			/* printing ChunkID */

	fread(&header_p->ChunkSize, sizeof(uint32_t), 1, filevar);
	printf("-> ChunkSize = %d \n", header_p->ChunkSize);

	fread(header_p->Format, sizeof(uint8_t), 4, filevar);
	printf("-> Chunk Format = %c%c%c%c \n", header_p->Format[0], header_p->Format[1], header_p->Format[2], header_p->Format[3]);

	fread(header_p->Subchunk1ID, sizeof(uint8_t), 4, filevar);
	printf(" -> Subchunk1ID = %c%c%c \n", header_p->Subchunk1ID[0], header_p->Subchunk1ID[1], header_p->Subchunk1ID[2]);

	fread(&header_p->Subchunk1Size, sizeof(uint32_t), 1, filevar);
	printf("-> Subchunk1size = %d \n", header_p->Subchunk1Size);

	fread(&header_p->AudioFormat, sizeof(uint16_t), 1, filevar);
	printf("-> AudioFormat = %d \n", header_p->AudioFormat);

	fread(&header_p->NumChannels, sizeof(uint16_t), 1, filevar);
	printf(" -> Number of channels = %d \n", header_p ->NumChannels);

	fread(&header_p->SampleRate, sizeof(uint32_t), 1, filevar);
	printf(" -> Sample rate = %d \n", header_p ->SampleRate);

	fread(&header_p ->ByteRate, sizeof(uint32_t), 1, filevar);
	printf(" -> ByteRate = %d \n", header_p ->ByteRate);

	fread(&header_p->BlockAlign, sizeof(uint16_t), 1, filevar);
	printf(" -> Blockalign = %d \n", header_p ->BlockAlign);

	fread(&header_p->BitsPerSample, sizeof(uint16_t), 1, filevar);
	printf(" -> BitsPerSample = %d \n", header_p ->BitsPerSample);

	// fread(header_p->Subchunk2ID, sizeof(uint8_t), 4, filevar);
	// printf(" -> SubChuck2ID = %c%c%c%c \n", header_p->Subchunk2ID[0], header_p->Subchunk2ID[1], header_p->Subchunk2ID[2], header_p->Subchunk2Size[3]);

	fread(&header_p->Subchunk2Size, sizeof(uint32_t), 1, filevar);
	printf("-> SubChunk2Size = %d  \n", header_p->Subchunk2Size);







		// fread(*header_p, counter+4, counter +7, filevar)








		/* header section */



	  //
	  // WRITE CODE HERE
	  //
	  printf("CODE TO BE WRITTEN...\n");
	  return 1;
}

int write_pcm_wavefile(pcm_wavefile_header_t *header_p, char *data, char *filename_output) /* I changed the signature from *filename to *filename_output */
{

  // WRITE CODE

  printf("CODE TO BE WRITTEN...\n");
 return 1;
 }
