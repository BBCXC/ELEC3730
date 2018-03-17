//     $Date: 2018-03-11 05:18:25 +1100 (Sun, 11 Mar 2018) $
// $Revision: 1206 $
//   $Author: Peter $

// Question 2

#include "Ass-01.h"

//! Byte swap unsigned short
uint16_t swap_uint16( uint16_t val )
{
    return (val << 8) | (val >> 8 );
}

//! Byte swap short
int16_t swap_int16( int16_t val )
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

//! Byte swap unsigned int
uint32_t swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}

//! Byte swap int
int32_t swap_int32( int32_t val )
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | ((val >> 16) & 0xFFFF);
}

int read_pcm_wavefile(pcm_wavefile_header_t *header_p, char **data_p, char *filename)
{
  FILE* file_p; //Set file variable
  pcm_wavefile_header_t head_data;  //Set header variable

  printf("\n");
  file_p = fopen(filename, "rb"); //Open file to read

  if(file_p == 0){  //If file pointer returns NULL
      printf("%3s ERROR: File unknown \n, " "", " ");  //Log Error
      if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " "); //Close file
      printf("\n");
      return -1;  //Return Failed
  }

  if(fseek(file_p, 0, SEEK_END) != 0) printf("%3s ERROR : fseek\n", " "); //Go to the end of the file
  long size_head = ftell(file_p); //Count bytes in remaining file

  if(size_head <= 44){  //If file size too small
      printf("%3s ERROR: File size incorrect, " ", file size: %ld \n"," ", size_head);
      if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");  //Close file
      printf("\n");
      return -1;  //Return Failed
  }

  if(fseek(file_p, 0, SEEK_SET) != 0) printf("%3s ERROR : fseek\n", " "); //Go back to position

  if(fread(&head_data.ChunkID,      4, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //big
  if(fread(&head_data.ChunkSize,      4, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //little
  if(fread(&head_data.Format,         4, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //big

  if(fread(&head_data.Subchunk1ID,    4, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //big
  if(fread(&head_data.Subchunk1Size,  4, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //little
  if(fread(&head_data.AudioFormat,    2, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //little
  if(fread(&head_data.NumChannels,    2, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //little
  if(fread(&head_data.SampleRate,     4, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //little
  if(fread(&head_data.ByteRate,       4, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //little
  if(fread(&head_data.BlockAlign,     2, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //little
  if(fread(&head_data.BitsPerSample,  2, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //little

  if(fread(&head_data.Subchunk2ID,    4, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //big
  if(fread(&head_data.Subchunk2Size,  4, 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //little

  //TODO
  //Convert endian of
  //ChunkID
  //Format
  //Subchunk1ID
  //Subchunk2ID

  printf("ChunkID : %s\n",         head_data.ChunkID);
  printf("ChunkSize : %u\n",       head_data.ChunkSize);
  printf("Format : %s\n\n",        head_data.Format);

  printf("Subchunk1ID : %s\n",     head_data.Subchunk1ID);
  printf("Subchunk1Size : %u\n",   head_data.Subchunk1Size);
  printf("AudioFormat : %u\n",     head_data.AudioFormat);
  printf("NumChannels : %u\n",     head_data.NumChannels);
  printf("SampleRate : %u\n",      head_data.SampleRate);
  printf("ByteRate : %u\n",        head_data.ByteRate);
  printf("BlockAlign : %u\n",      head_data.BlockAlign);
  printf("BitsPerSample : %u\n\n", head_data.BitsPerSample);

  printf("Subchunk2ID : %s\n",     head_data.Subchunk2ID);
  printf("Subchunk2Size : %u\n",   head_data.Subchunk2Size);


  if(head_data.AudioFormat != 1){
    printf("%3s ERROR: Wrong audio format, Expected : 1, Got : %u\n\n", " ", head_data.AudioFormat); //If wave file not found wrong format
    return -1;
  }

  long curr_pos = ftell(file_p);
  if(fseek(file_p, 0, SEEK_END) != 0) printf("%3s ERROR : fseek\n", " ");  //Go to the end of the file
  long size_data = ftell(file_p); //Count bytes in remaining file

  int subchunk_bytes = head_data.SampleRate * head_data.NumChannels * head_data.BitsPerSample / 8;

  printf("Subchunk_bytes %i\n", subchunk_bytes);

  if(size_data < subchunk_bytes){ //If file size too small
      printf("%3s ERROR: File size incorrect, " ", file size: %ld \n"," ", size_data);
      if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " "); //Close file
      printf("\n");
      return -1;  //Return Failed
  }
  else{
    if(fseek(file_p, curr_pos, SEEK_SET) != 0) printf("%3s ERROR : fseek\n", " ");
  }

  *data_p = (char*) calloc(subchunk_bytes, sizeof(char));
  for(int i=0; i<subchunk_bytes; i++){
    if(fread(data_p,  1, 1, file_p) != 1){
      //printf("%3s ERROR : fread\n", " ");  //little
    }
    if(i % 1000 == 0) printf("Data %i, %c\n",i, *data_p);
  }


  if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");

  printf("CODE TO BE WRITTEN QUESTION 2...\n\n");
  return 0;
}

/**********************************************************************************************************
 **********************************************************************************************************
 **********************************************************************************************************
 */

int write_pcm_wavefile(pcm_wavefile_header_t *header_p, char *data, char *filename)
{
  //
  // WRITE CODE
  //
  printf("CODE TO BE WRITTEN QUESTION 2...\n\n");
  return 1;


  FILE* file_p; //Set file variable
  pcm_wavefile_header_t head_data;  //Set header variable


  printf("\n");
  printf("FILENAME %s", filename);
  file_p = fopen(filename, "rb"); //Open file to read

  if(file_p == 0){  //If file pointer returns NULL
      printf("%3s ERROR: File unknown \n", " ");  //Log Error
          if(freopen(filename, "wb", file_p) !=0){
            printf("%3s ERROR : Unable to create file", " ");
            return -1;
          }
      printf("\n");
  }

  if(fwrite(&head_data.ChunkID,        4, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little
  if(fwrite(&head_data.ChunkSize,      4, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little
  if(fwrite(&head_data.Format,         4, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little

  if(fwrite(&head_data.Subchunk1ID,    4, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little
  if(fwrite(&head_data.Subchunk1Size,  4, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little
  if(fwrite(&head_data.AudioFormat,    2, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little
  if(fwrite(&head_data.NumChannels,    2, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little
  if(fwrite(&head_data.SampleRate,     4, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little
  if(fwrite(&head_data.ByteRate,       4, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little
  if(fwrite(&head_data.BlockAlign,     2, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little
  if(fwrite(&head_data.BitsPerSample,  2, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little

  if(fwrite(&head_data.Subchunk2ID,    4, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little
  if(fwrite(&head_data.Subchunk2Size,  4, 1, file_p) != 1) printf("%3s ERROR : fwrite\n", " ");  //little

  printf("ChunkID : %s\n",         head_data.ChunkID);
  printf("ChunkSize : %u\n",       head_data.ChunkSize);
  printf("Format : %s\n\n",        head_data.Format);

  printf("Subchunk1ID : %s\n",     head_data.Subchunk1ID);
  printf("Subchunk1Size : %u\n",   head_data.Subchunk1Size);
  printf("AudioFormat : %u\n",     head_data.AudioFormat);
  printf("NumChannels : %u\n",     head_data.NumChannels);
  printf("SampleRate : %u\n",      head_data.SampleRate);
  printf("ByteRate : %u\n",        head_data.ByteRate);
  printf("BlockAlign : %u\n",      head_data.BlockAlign);
  printf("BitsPerSample : %u\n\n", head_data.BitsPerSample);

  printf("Subchunk2ID : %s\n",     head_data.Subchunk2ID);
  printf("Subchunk2Size : %u\n",   head_data.Subchunk2Size);

  //
  // WRITE CODE
  //
  printf("CODE TO BE WRITTEN QUESTION 2...\n\n");
  return 1;
}
