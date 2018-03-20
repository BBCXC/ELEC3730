/* ELEC3730 Question 2
Author: Mitchell Marotta
Date: 12/03/2018
Description: */



#include "Ass-01.h"
#include <string.h>

int read_pcm_wavefile(pcm_wavefile_header_t *header_p, char **data_p, char *filename){

  FILE* file_p;    /* variable of type file */
  pcm_wavefile_header_t head_data;
  const char* RIFF_string = "RIFF";
  const char* sub1_string = "fmt";
  const char* fmt_string = "";
  int ui8_t_size = sizeof(uint8_t);
  int ui16_t_size = sizeof(uint16_t);
  int ui32_t_size = sizeof(uint32_t);

  file_p = fopen(filename, "rb");                    /* opening the file and storing it in file variable */
  if(file_p == 0){  //If file pointer returns NULL
    printf("%3s ERROR: File unknown \n", " ");  //Log Error
    return -1;
  }

  if(fread(head_data.ChunkID, ui8_t_size, 4, file_p) != 4) printf("%3s ERROR : fread : %s\n", " ", "head_data.ChunkID");          /* Reading the Chunk ID */

  if (strncmp(RIFF_string, (char*)head_data.ChunkID, 4) != 0){ /* Checking to see if the ID is correct */
    printf("%3s ERROR: Incorrect ChunkID expected: %s got: %c%c%c%c\n", " ", RIFF_string, head_data.ChunkID[0], head_data.ChunkID[1], head_data.ChunkID[2], head_data.ChunkID[3]);
    return -1;
  }

  if(fread(&head_data.ChunkSize, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.ChunkSize");

  if(fread(head_data.Format, ui8_t_size, 4, file_p) != 4) printf("%3s ERROR : fread : %s\n", " ", "head_data.Format");
  if (strncmp(fmt_string, (char*)head_data.Fotmat, 3) != 0) {
    printf("%3s ERROR: Incorrect Format expected: %s got: %c%c%c%c\n", " ", fmt_string, head_data.Subchunk1ID[0], head_data.Subchunk1ID[1], head_data.Subchunk1ID[2], head_data.Subchunk1ID[3]);
    return -1;
  }

  if(fread(head_data.Subchunk1ID, ui8_t_size, 4, file_p) != 4) printf("%3s ERROR : fread : %s\n", " ", "head_data.Subchunk1ID");

  if (strncmp(sub1_string, (char*)head_data.Subchunk1ID, 3) != 0) {
    printf("%3s ERROR: Incorrect Subchunk1ID expected: %s got: %c%c%c%c\n", " ", sub1_string, head_data.Subchunk1ID[0], head_data.Subchunk1ID[1], head_data.Subchunk1ID[2], head_data.Subchunk1ID[3]);
    return -1;
  }

  if(fread(&head_data.Subchunk1Size, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.Subchunk1Size");

  if (head_data. Subchunk1Size != 16) {
    if(fread(&head_data.AudioFormat, ui16_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.AudioFormat");
    if (head_data.AudioFormat != 1){
      printf("%3s ERROR: Not PCM encoded \n", " ");
    }
    return -1;
  }

  else{
    if(fread(&head_data.AudioFormat, ui16_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.AudioFormat");
  }

  if(fread(&head_data.NumChannels, ui16_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.NumChannels");
  if(&head_data.NumChannels != 1){
    printf("%3s ERROR : NumChannels > 1\n", " ");
  }

  if(fread(&head_data.SampleRate, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.SampleRate");
  if(fread(&head_data.ByteRate, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.ByteRate");
  if(fread(&head_data.BlockAlign, ui16_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.BlockAlign");
  if(fread(&head_data.BitsPerSample, ui16_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.BitsPerSample");
  if(fread(head_data.Subchunk2ID, ui8_t_size, 4, file_p) != 4) printf("%3s ERROR : fread : %s\n", " ", "head_data.Subchunk2ID");
  if(fread(&head_data.Subchunk2Size, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.Subchunk2Size");


  int subchunk_bytes = head_data.SampleRate * head_data.NumChannels * head_data.BitsPerSample / 8;

  if(head_data.ByteRate != subchunk_bytes){
	  printf("%3s ERROR : Incorrect ByteRate", " ");
  }
  /* Print Statements */
  printf("\n");
  printf("-> ChunkID = %c%c%c%c \n", head_data.ChunkID[0], head_data.ChunkID[1], head_data.ChunkID[2], head_data.ChunkID[3]);
  printf("-> ChunkSize = %d \n", head_data.ChunkSize);
  printf("-> Chunk Format = %c%c%c%c \n", head_data.Format[0], head_data.Format[1], head_data.Format[2], head_data.Format[3]);
  printf("-> Subchunk1ID = %c%c%c \n", head_data.Subchunk1ID[0], head_data.Subchunk1ID[1], head_data.Subchunk1ID[2]);
  printf("-> Subchunk1size = %d \n", head_data.Subchunk1Size);
  printf("-> AudioFormat = %d \n", head_data.AudioFormat);
  printf("-> Number of channels = %d \n", head_data.NumChannels);
  printf("-> Sample rate = %d \n", head_data.SampleRate);
  printf("-> ByteRate = %d \n", head_data.ByteRate);
  printf("-> BlockAlign = %d \n", head_data.BlockAlign);
  printf("-> BitsPerSample = %d \n", head_data.BitsPerSample);
  printf("-> SubChuck2ID = %c%c%c%c \n", head_data.Subchunk2ID[0], head_data.Subchunk2ID[1], head_data.Subchunk2ID[2], head_data.Subchunk2ID[3]);
  printf("-> SubChunk2Size = %d  \n\n", head_data.Subchunk2Size);

  *header_p = head_data;

  long curr_pos = ftell(file_p);
  if(fseek(file_p, 0, SEEK_END) != 0) printf("%3s ERROR : fseek\n", " ");  //Go to the end of the file
  long size_data = ftell(file_p); //Count bytes in remaining file

  printf("size data %i\n\n", size_data - 44);

  if(size_data-8 < head_data.Subchunk2Size){ //If file size too small
    printf("%3s ERROR: File size incorrect, " ", file size: %ld \n"," ", size_data);
    if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " "); //Close file
    printf("\n");
    return -1;  //Return Failed
  }

  else{
    if(fseek(file_p, curr_pos, SEEK_SET) != 0) printf("%3s ERROR : fseek\n", " ");
  }
  //printf("Current Position %i\n", ftell(file_p));

  //printf("Cursor %ld\n", curr_pos);



  *data_p = (char*) calloc(head_data.Subchunk2Size, sizeof(char));
  if(fread(*data_p,  sizeof(char), head_data.Subchunk2Size, file_p) != head_data.Subchunk2Size) printf("ERROR: Reading data\n");
  else printf("Success reading data\n");

  printf("\n");

  if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");

  return 0;
}

int write_pcm_wavefile(pcm_wavefile_header_t *header_p, char *data, char *filename_output){ /* I changed the signature from *filename to *filename_output */

  FILE* file_p;   /* variable of type file */

  printf("\n");
  file_p = fopen(filename_output, "wb");                   /* opening the file and storing it in file variable */

  if(file_p == 0){  //If file pointer returns NULL
      printf("%3s ERROR: File unknown \n", " ");  //Log Error
      if(freopen(filename_output, "wb", file_p) !=0) {
          printf("%3s ERROR : Unable to create file", " ");
          return -1;
      }
      printf("Created New File : %s\n", filename_output);
      printf("\n");
  }

  pcm_wavefile_header_t head_data = *header_p;

  if(fwrite(head_data.ChunkID,         4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "head_data.ChunkID");  //little
  if(fwrite(&head_data.ChunkSize,      4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.ChunkSize");  //little
  if(fwrite(&head_data.Format,         4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.Format");  //little

  if(fwrite(&head_data.Subchunk1ID,    4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.Subchunk1ID");  //little
  if(fwrite(&head_data.Subchunk1Size,  4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.Subchunk1Size");  //little
  if(fwrite(&head_data.AudioFormat,    2, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.AudioFormat");  //little
  if(fwrite(&head_data.NumChannels,    2, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.NumChannels");  //little
  if(fwrite(&head_data.SampleRate,     4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.SampleRate");  //little
  if(fwrite(&head_data.ByteRate,       4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.ByteRate");  //little
  if(fwrite(&head_data.BlockAlign,     2, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.BlockAlign");  //little
  if(fwrite(&head_data.BitsPerSample,  2, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.BitsPerSample");  //little

  if(fwrite(&head_data.Subchunk2ID,    4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.Subchunk2ID");  //little
  if(fwrite(&head_data.Subchunk2Size,  4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.Subchunk2Size");  //little

  printf("ChunkID = %c%c%c%c \n", head_data.ChunkID[0], head_data.ChunkID[1], head_data.ChunkID[2], head_data.ChunkID[3]);
  printf("ChunkSize : %u\n",       head_data.ChunkSize);
  printf("Chunk Format = %c%c%c%c \n\n", head_data.Format[0], head_data.Format[1], head_data.Format[2], head_data.Format[3]);

  printf("Subchunk1ID = %c%c%c \n", head_data.Subchunk1ID[0], head_data.Subchunk1ID[1], head_data.Subchunk1ID[2]);
  printf("Subchunk1Size : %u\n",   head_data.Subchunk1Size);
  printf("AudioFormat : %u\n",     head_data.AudioFormat);
  printf("NumChannels : %u\n",     head_data.NumChannels);
  printf("SampleRate : %u\n",      head_data.SampleRate);
  printf("ByteRate : %u\n",        head_data.ByteRate);
  printf("BlockAlign : %u\n",      head_data.BlockAlign);
  printf("BitsPerSample : %u\n\n", head_data.BitsPerSample);

  printf("SubChuck2ID = %c%c%c%c \n", head_data.Subchunk2ID[0], head_data.Subchunk2ID[1], head_data.Subchunk2ID[2], head_data.Subchunk2ID[3]);
  printf("Subchunk2Size : %u\n",   head_data.Subchunk2Size);

  if(fwrite(data, head_data.Subchunk2Size, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "head_data.Subchunk2Size");
  else printf("\nSuccess writing data\n");

  printf("\n");

  fclose(file_p);
  return 0;
}
