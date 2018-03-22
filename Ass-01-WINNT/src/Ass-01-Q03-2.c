#include "Ass-01.h"

/********************************************************************************************************************/
/*                                                  Read Function                                                   */
/********************************************************************************************************************/
int read_pcm_wavefile_header(pcm_wavefile_header_t *header_p, char **data_p, char *filename){

  FILE* file_p;
  pcm_wavefile_header_t head_data;

  const char* RIFF_string = "RIFF"; //String espected
  const char* sub1_string = "fmt";  //String espected
  const char* fmt_string = "WAVE";  //String espected
  const char* schunk2id_string = "data";  //String espected

  int ui8_t_size = sizeof(uint8_t); //Pre calculate values
  int ui16_t_size = sizeof(uint16_t); //Pre calculate values
  int ui32_t_size = sizeof(uint32_t); //Pre calculate values

  file_p = fopen(filename, "rb"); //Open the file for reading
  if(file_p == 0){  //If file pointer returns NULL
    printf("%3s ERROR: File unknown \n", " ");  //Log Error
    return -1;
  }

  /********************************************************************************************************************/
  /*                                                 Read Header Data                                                 */
  /********************************************************************************************************************/

  // Read Chunk ID
  // Compare to expected value
  if(fread(head_data.ChunkID, ui8_t_size, 4, file_p) != 4) printf("%3s ERROR : fread : %s\n", " ", "head_data.ChunkID");
  if (strncmp(RIFF_string, (char*)head_data.ChunkID, 4) != 0){ /* Checking to see if the ID is correct */
    printf("%3s ERROR: Incorrect ChunkID expected: %s got: %c%c%c%c\n", " ", RIFF_string, head_data.ChunkID[0], head_data.ChunkID[1], head_data.ChunkID[2], head_data.ChunkID[3]);
    return -1;
  }

  // Read Chunk Size
  // Compare to expected value
  if(fread(&head_data.ChunkSize, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.ChunkSize");

  // Read Format
  // Compare to expected value
  if(fread(head_data.Format, ui8_t_size, 4, file_p) != 4) printf("%3s ERROR : fread : %s\n", " ", "head_data.Format");
  if (strncmp(fmt_string, (char*)head_data.Format, 3) != 0) {
    printf("%3s ERROR: Incorrect Format expected: %s got: %c%c%c%c\n", " ", fmt_string, head_data.Subchunk1ID[0], head_data.Subchunk1ID[1], head_data.Subchunk1ID[2], head_data.Subchunk1ID[3]);
    return -1;
  }

  // Read Sub Chunk ID 1
  // Compare to expected value
  if(fread(head_data.Subchunk1ID, ui8_t_size, 4, file_p) != 4) printf("%3s ERROR : fread : %s\n", " ", "head_data.Subchunk1ID");
  if (strncmp(sub1_string, (char*)head_data.Subchunk1ID, 3) != 0) {
    printf("%3s ERROR: Incorrect Subchunk1ID expected: %s got: %c%c%c%c\n", " ", sub1_string, head_data.Subchunk1ID[0], head_data.Subchunk1ID[1], head_data.Subchunk1ID[2], head_data.Subchunk1ID[3]);
    return -1;
  }

  // Read Sub Chunk 1 Size
  // Compare to expected value
  if(fread(&head_data.Subchunk1Size, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.Subchunk1Size");
  //TODO Why am i doing this?
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

  // Read Number of Channels
  // Compare to expected value
  if(fread(&head_data.NumChannels, ui16_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.NumChannels");
  if(head_data.NumChannels != 1){
    printf("%3s ERROR : NumChannels > 1\n", " ");
  }

  // Read SampleRate
  if(fread(&head_data.SampleRate, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.SampleRate");

  // Read ByteRate
  if(fread(&head_data.ByteRate, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.ByteRate");

  // Read Block Align
  if(fread(&head_data.BlockAlign, ui16_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.BlockAlign");

  // Read Bits Per Sample
  if(fread(&head_data.BitsPerSample, ui16_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.BitsPerSample");

  // Read Sub Chunk ID 2
  // Compare to expected value
  if(fread(head_data.Subchunk2ID, ui8_t_size, 4, file_p) != 4) printf("%3s ERROR : fread : %s\n", " ", "head_data.Subchunk2ID");
  if (strncmp(schunk2id_string, (char*)head_data.Subchunk2ID, 3) != 0) {
    printf("%3s ERROR: Incorrect Format expected: %s got: %c%c%c%c\n", " ", schunk2id_string, head_data.Subchunk2ID[0], head_data.Subchunk2ID[1], head_data.Subchunk2ID[2], head_data.Subchunk2ID[3]);
    return -1;
  }

  // Read Sub Chunk 2 Size 
  // Compare to expected value
  if(fread(&head_data.Subchunk2Size, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.Subchunk2Size");

  int subchunk_bytes = head_data.SampleRate * head_data.NumChannels * head_data.BitsPerSample / 8;
  if(head_data.ByteRate != subchunk_bytes){
    printf("%3s ERROR : Incorrect ByteRate", " ");
  }

  long curr_pos = ftell(file_p);
  if(fseek(file_p, 0, SEEK_END) != 0) printf("%3s ERROR : fseek\n", " ");  //Go to the end of the file
  long size_data = ftell(file_p); //Count bytes in remaining file

  if(size_data-8 < head_data.Subchunk2Size){ //If file size too small
    printf("%3s ERROR: File size incorrect, " ", file size: %ld \n"," ", size_data);
    if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " "); //Close file
    printf("\n");
    return -1;  //Return Failed
  }
  else{
    if(fseek(file_p, curr_pos, SEEK_SET) != 0) printf("%3s ERROR : fseek\n", " ");
  }

  /********************************************************************************************************************/
  /*                                                 Print Header Data                                                */
  /********************************************************************************************************************/

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
}
int read_pcm_wavefile_data(pcm_wavefile_header_t *header_p, char *data_p, char *filename){

  if(fread(*data_p,  sizeof(char), head_data.Subchunk2Size, file_p) != head_data.Subchunk2Size) printf("ERROR: Reading data\n");
  else printf("Success reading data\n");

  printf("\n");

  return 0;
}

/********************************************************************************************************************/
/*                                                 Filter Function                                                  */
/********************************************************************************************************************/

int filter(char *filter_filename, char *input_wavefilename, char *output_wavefilename){

  int coeff_num = 0;
  double *coeff_values; // Array of filter coefficient values
  pcm_wavefile_header_t header;
  char *data;

  if (read_coefficients(&coeff_num, &coeff_values, filter_filename)==0){  //If success in reading coefficients
    if (coeff_num <= 0) return -1;  //No coefficients in filter

    if (read_pcm_wavefile_header(&header, &data, input_wavefilename) != 0){
      printf("%3s ERROR: Unable to read '.wav' file\n\n", " "); //Unable to read wavefile
      free(data);
      free(coeff_values);
      return -1;
    }

    if(header.Subchunk1Size != 16){
      printf("Only 16 bit files supported\n");
    }

    printf("\n");

    int input_len = header.Subchunk2Size;
    int divisor = header.Subchunk1Size / 8;
    char iteration;
    double output;
    char output_fixed[input_len];
    int input;

    //For each sample in the file
    for(int i=0; i<input_len/divisor; i++){
      if ((i+1)<coeff_num) iteration = i+1; //Decide if the input exists
      else iteration = coeff_num; //Set input to coefficient

      output = 0;

      for(int j=0; j<iteration; j++){ //For each coefficient filer the input file
        if(header.Subchunk1Size == 16){
          value = read_pcm_wavefile_data(&header, &data, input_wavefilename);
          input = value;
          value = read_pcm_wavefile_data(&header, &data, input_wavefilename);
          input = value<<8;
        }
        if(header.Subchunk1Size == 8){
          //input[i] = data[i];
          printf("Currently in development\n");
        }
        output = output + coeff_values[j] * input[i-j]; //Iterate through each buffer value
      }
      
      if(header.Subchunk1Size == 16){
        output_fixed[i*2] = (char)(int)output; //Split output into fixed point 8 bit numbers
        output_fixed[i*2+1] = (char)((int)output>> 8); //Split output into fixed point 8 bit numbers
      }
      else if(header.Subchunk1Size == 8){
        //output_fixed[i] = (char)(int)output;
        printf("Currently in development\n");
      }
    }
    if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");

    write_pcm_wavefile(&header, output_fixed, output_wavefilename); //Write the new data to the new file TODO
    printf("\n");
  }
  else{
    free(coeff_values);
    if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");
    return -1;  //Not successful in reading coefficients
  }
  free(coeff_values);
  return 0; //Success
}
