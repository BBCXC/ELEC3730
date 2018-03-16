//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 2

#include "Ass-01.h"

//int byteswap(pcm_wavefile_header_t num, int size){
//
//	if(size == 8){
//
//	}
//	else if(size == 16){
//		pcm_wavefile_header_t num = (num>>8) | (num<<8);
//	}
//	else if(size == 32){
//		pcm_wavefile_header_t num = ((num>>24)&0xff) | // move byte 3 to byte 0
//	          	  	  	   	   	    ((num<<8)&0xff0000) | // move byte 1 to byte 2
//									((num>>8)&0xff00) | // move byte 2 to byte 1
//									((num<<24)&0xff000000); // byte 0 to byte 3
//	}
//	return 0;
//}

int read_pcm_wavefile(pcm_wavefile_header_t *header_p, char **data_p, char *filename)
{

	/*
	 * Read in file from filename rb
	 * Read header, store information in header_p
	 * allocate memory for data
	 * read in data
	 * return 0 success
	 * else return -1
	 * print header information
	 */
	FILE* file_p;
	pcm_wavefile_header_t head_data;

	printf("\n");
	file_p = fopen(filename, "rb");

	if(file_p == 0){	//If file pointer returns NULL
			printf("%3s ERROR: File unknown \n", " ");	//Log Error
			fclose(file_p);	//Close file
			printf("\n");
			return -1;	//Return Failed
	}

	fseek(file_p, 0, SEEK_END);	//Go to the end of the file
	long size_file = ftell(file_p);	//Count bytes in remaining file

	if(size_file <= 44){	//If file size too small
			printf("%3s ERROR: File size incorrect, file size: %ld \n"," ", size_file);
			fclose(file_p);	//Close file
			printf("\n");
			return -1;	//Return Failed
	}

	fseek(file_p, 0, SEEK_SET);	//Go back to position

	fread(&head_data.ChunkID,     	 4, 1, file_p);  //big
	fread(&head_data.ChunkSize,      4, 1, file_p);  //little
	fread(&head_data.Format,         4, 1, file_p);  //big

	fread(&head_data.Subchunk1ID,    4, 1, file_p);  //big
	fread(&head_data.Subchunk1Size,  4, 1, file_p);  //little
	fread(&head_data.AudioFormat,    2, 1, file_p);  //little
	fread(&head_data.NumChannels,    2, 1, file_p);  //little
	fread(&head_data.SampleRate,     4, 1, file_p);  //little
	fread(&head_data.ByteRate,       4, 1, file_p);  //little
	fread(&head_data.BlockAlign,     2, 1, file_p);  //little
	fread(&head_data.BitsPerSample,  2, 1, file_p);  //little

	fread(&head_data.Subchunk2ID,    4, 1, file_p);  //big
	fread(&head_data.Subchunk2Size,  4, 1, file_p);  //little

//	if(byteswap(&head_data.Subchunk1ID, 8) != 0){
//		printf("Error in byteswap\n");
//		return -1;
//	}


	printf("ChunkID : %s\n",       head_data.ChunkID);
	printf("ChunkSize : %u\n",     head_data.ChunkSize);
	printf("Format : %s\n\n",        head_data.Format);

	printf("Subchunk1ID : %s\n",   head_data.Subchunk1ID);
	printf("Subchunk1Size : %u\n", head_data.Subchunk1Size);
	printf("AudioFormat : %u\n",   head_data.AudioFormat);
	printf("NumChannels : %u\n",   head_data.NumChannels);
	printf("SampleRate : %u\n",    head_data.SampleRate);
	printf("ByteRate : %u\n",      head_data.ByteRate);
	printf("BlockAlign : %u\n",    head_data.BlockAlign);
	printf("BitsPerSample : %u\n\n", head_data.BitsPerSample);

	printf("Subchunk2ID : %s\n",   head_data.Subchunk2ID);
	printf("Subchunk2Size : %u\n", head_data.Subchunk2Size);


	if(head_data.AudioFormat != 1){
		printf("Wrong audio format, Expected : 1, Got : %u\n\n", head_data.AudioFormat);
		return -1;
	}

	data_p = (char**) calloc(head_data.Subchunk2Size, sizeof(char));

	for(int i=0; i<3; i++){
		fread(data_p[i],  4, 1, file_p);  //little
		printf("Data Reading : %s\n", data_p[i]);
	}

	//
	// WRITE CODE HERE
	//
	printf("CODE TO BE WRITTEN QUESTION 2...\n\n");
	printf("I got here 7");
	return 1;
}

int write_pcm_wavefile(pcm_wavefile_header_t *header_p, char *data, char *filename)
{
  //
  // WRITE CODE
  //
  printf("CODE TO BE WRITTEN QUESTION 2...\n\n");
  printf("I got here 6");
  return 1;
}
