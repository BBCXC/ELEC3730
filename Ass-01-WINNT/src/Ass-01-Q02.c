/* ELEC3730 Question 2
Author: Mitchell Marotta
Date: 12/03/2018
Description: */



#include "Ass-01.h"
#include <string.h>


int string_parser_2(char *inp, char **array_of_words_p[]){
    char curr_char;
    char prev_char = '.';
    int num_words = 0;
    int len_word = 0;

    for(int i=0; i<strlen(inp); i++){
        curr_char = inp[i];

        if(curr_char == '\0') return 0;  //No words in string
        else if(curr_char == ' ' && prev_char == ' '){
            //Multiple spaces in a row
        }
        else if(curr_char != ' ' && prev_char == ' '){
            //Found new word
            num_words++;
        }
        else{
            //Currently in word, don't do anything
        }
        prev_char = curr_char;
    }

    *array_of_words_p = (char**) calloc(num_words, sizeof(char*));

    prev_char = '.';
    num_words     = 0;
    for (int i = 0; i < strlen(inp);) {
        curr_char = inp[i];

        if (curr_char == '\0' && len_word == 0){
            return num_words;
        }
        else if (curr_char == '.' && prev_char == '.') {
            // Multiple spaces in a row
        }
        else if (curr_char != '.' && prev_char == '.') {
            // Found new word
            num_words++;
            len_word = 1;
            // printf("Found new word\n");
        }
        else if (curr_char == '.' && prev_char != '.') {
            // Found end of word
            (*array_of_words_p)[num_words - 1] = (char*) calloc(len_word + 1, sizeof(char));  // Allocate enough memory for each word
            strncpy((*array_of_words_p)[num_words - 1], (inp + (i - len_word)), len_word);  // Copy inp into memory allocation
            len_word = 0;
            i+=3;
        }
        else {
            // Currently in word
            len_word++;
        }
        prev_char = curr_char;
        // No words in string
        i++;
    }
    if(len_word != 0){
         (*array_of_words_p)[num_words - 1] = (char*) calloc(len_word + 1, sizeof(char)); // Allocate enough memory for each word
         strncpy((*array_of_words_p)[num_words - 1], (inp + (strlen(inp) - len_word)), len_word); // Copy inp into memory allocation
         len_word = 0;
    }
    return num_words;
}





int read_pcm_wavefile(pcm_wavefile_header_t *header_p, char **data_p, char *filename){

  FILE* file_p;    /* variable of type file */
  pcm_wavefile_header_t head_data;
  const char* RIFF_string = "RIFF";
  const char* fmt_string = "fmt";
  int ui8_t_size = sizeof(uint8_t);
  int ui16_t_size = sizeof(uint16_t);
  int ui32_t_size = sizeof(uint32_t);

  file_p = fopen(filename, "rb");                    /* opening the file and storing it in file variable */
  if(file_p == 0){  //If file pointer returns NULL
    printf("%3s ERROR: File unknown \n", " ");  //Log Error
    return -1;
  }

  if(fread(head_data.ChunkID, ui8_t_size, 4, file_p)) printf("%3s ERROR : fread\n", " ");          /* Reading the Chunk ID */

  if (strncmp(RIFF_string, (char*)head_data.ChunkID, 4) != 0){ /* Checking to see if the ID is correct */
    printf("%3s ERROR: Incorrect ChunkID expected: %s got: %c%c%c%c\n", " ", RIFF_string, head_data.ChunkID[0], head_data.ChunkID[1], head_data.ChunkID[2], head_data.ChunkID[3]);
    return -1;
  }

  if(fread(&head_data.ChunkSize, ui32_t_size, 1, file_p)) printf("%3s ERROR : fread\n", " ");

  if(fread(head_data.Format, ui8_t_size, 4, file_p)) printf("%3s ERROR : fread\n", " ");

  if(fread(head_data.Subchunk1ID, ui8_t_size, 4, file_p)) printf("%3s ERROR : fread\n", " ");

  if (strncmp(fmt_string, (char*)head_data.Subchunk1ID, 3) != 0) {
    printf("%3s ERROR: Incorrect Subchunk1ID expected: %s got: %c%c%c%c\n", " ", fmt_string, head_data.Subchunk1ID[0], head_data.Subchunk1ID[1], head_data.Subchunk1ID[2], head_data.Subchunk1ID[3]);
    return -1;
  }

  if(fread(&head_data.Subchunk1Size, ui32_t_size, 1, file_p)) printf("%3s ERROR : fread\n", " ");

  if (head_data. Subchunk1Size != 16) {
    if(fread(&head_data.AudioFormat, ui16_t_size, 1, file_p)) printf("%3s ERROR : fread\n", " ");
    if (head_data.AudioFormat != 1){
      printf("%3s ERROR: Not PCM encoded \n", " ");
    }
    return -1;
  }

  else{
    if(fread(&head_data.AudioFormat, ui16_t_size, 1, file_p)) printf("%3s ERROR : fread\n", " ");
  }

  if(fread(&head_data.NumChannels, ui16_t_size, 1, file_p)) printf("%3s ERROR : fread\n", " ");
  if(fread(&head_data.SampleRate, ui32_t_size, 1, file_p)) printf("%3s ERROR : fread\n", " ");
  if(fread(&head_data.ByteRate, ui32_t_size, 1, file_p)) printf("%3s ERROR : fread\n", " ");
  if(fread(&head_data.BlockAlign, ui16_t_size, 1, file_p)) printf("%3s ERROR : fread\n", " ");
  if(fread(&head_data.BitsPerSample, ui16_t_size, 1, file_p)) printf("%3s ERROR : fread\n", " ");
  if(fread(head_data.Subchunk2ID, ui8_t_size, 4, file_p)) printf("%3s ERROR : fread\n", " ");
  if(fread(&head_data.Subchunk2Size, ui32_t_size, 1, file_p)) printf("%3s ERROR : fread\n", " ");

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

  int subchunk_bytes = head_data.SampleRate * head_data.NumChannels * head_data.BitsPerSample / 8;

  printf("Subchunk_bytes %i\n\n", subchunk_bytes);

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
    if(i % 1000 == 0) printf("Data %i, %s\n",i, *data_p);
  }
  printf("\n");

  if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");

  return 0;
}

int write_pcm_wavefile(pcm_wavefile_header_t *header_p, char *data, char *filename_output){ /* I changed the signature from *filename to *filename_output */

  FILE* file_p;   /* variable of type file */

  char **array_of_words;
  int count;
  int j=0;

  count = string_parser_2(filename_output, &array_of_words);

  char file_output_name[100];
  strcpy(file_output_name, array_of_words[0]);

  for(j=1; j<count; j++){
    strcat(file_output_name, array_of_words[j]);
  }
  strcat(file_output_name, ".wav");

  printf("New File Name %s\n", file_output_name);

  free(array_of_words[0]);
  free(array_of_words);


  file_p = fopen(file_output_name, "rb");                   /* opening the file and storing it in file variable */

  if(file_p == 0){  //If file pointer returns NULL
      printf("%3s ERROR: File unknown \n", " ");  //Log Error
      if(freopen(file_output_name, "wb", file_p) !=0) {
          printf("%3s ERROR : Unable to create file", " ");
          return -1;
      }
      printf("Created New File : %s\n", file_output_name);
      printf("\n");
  }

  pcm_wavefile_header_t head_data = *header_p;

  if(fwrite(head_data.ChunkID,         4, 1, file_p) != 1) printf("%3s ERROR : fwrite1\n", " ");  //little
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

  printf("ChunkID = %c%c%c%c \n", head_data.ChunkID[0], head_data.ChunkID[1], head_data.ChunkID[2], head_data.ChunkID[3]);
  printf("ChunkSize : %u\n",       head_data.ChunkSize);
  printf("-> Chunk Format = %c%c%c%c \n\n", head_data.Format[0], head_data.Format[1], head_data.Format[2], head_data.Format[3]);

  printf("-> Subchunk1ID = %c%c%c \n", head_data.Subchunk1ID[0], head_data.Subchunk1ID[1], head_data.Subchunk1ID[2]);
  printf("Subchunk1Size : %u\n",   head_data.Subchunk1Size);
  printf("AudioFormat : %u\n",     head_data.AudioFormat);
  printf("NumChannels : %u\n",     head_data.NumChannels);
  printf("SampleRate : %u\n",      head_data.SampleRate);
  printf("ByteRate : %u\n",        head_data.ByteRate);
  printf("BlockAlign : %u\n",      head_data.BlockAlign);
  printf("BitsPerSample : %u\n\n", head_data.BitsPerSample);

  printf("-> SubChuck2ID = %c%c%c%c \n", head_data.Subchunk2ID[0], head_data.Subchunk2ID[1], head_data.Subchunk2ID[2], head_data.Subchunk2ID[3]);
  printf("Subchunk2Size : %u\n",   head_data.Subchunk2Size);


  fclose(file_p);
  return 0;
}
