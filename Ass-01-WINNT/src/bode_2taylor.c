
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#define PI2 6.2832



#include "Ass-01.h"

int read_coefficients(int *coeff_num_p, double **coeff_values_p, char *filename)
{
    FILE * file_p;  //File
    int coeff_num;  //Number of ceofficients
    int pretty_print = 0;   //Toggles whether array prints

    file_p = fopen(filename, "rb"); //Open file for reading

    if(file_p == 0){    //If file pointer returns NULL
        printf("%3s ERROR: Unable to open file for reading\n", " ");    //Log Error
        printf("\n");
        return -1;  //Return Failed
    }

    if(fseek(file_p, 0, SEEK_END) != 0) printf("%3s ERROR : fseek", " ");   //Go to the end of the file
    long size_file = ftell(file_p); //Count bytes in remaining file
    if(fseek(file_p, 0, SEEK_SET) != 0) printf("%3s ERROR : fseek", " ");   //Go back to position

    if(size_file <= 4){ //If file size too small
        printf("%3s ERROR: File too small, file size: %ld \n"," ", size_file);
        if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");    //Close file
        printf("\n");
        return -1;  //Return Failed
    }

    if(fread(&coeff_num, sizeof(int), 1, file_p) != 1) printf("%3s ERROR : fread\n", " ");  //Read in first 4 bytes

    if((size_file - sizeof(int)) % coeff_num != 0){ //If remaining bytes doesn't add up TODO this should be (((size_file - 4)/coeff_num) == sizeof(double))
        printf("%3s ERROR: File size incorrect for declaration, declaration: %i, file size: %ld \n"," ", coeff_num, size_file);
        if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");    //Close file
        printf("\n");
        return -1;  //Return Failed
    }
    else printf("--> Array size %3s = %d\n", " ", coeff_num);   //All seems good print size of array

    double* coeff_values = (double*) calloc(coeff_num, 8);  //Allocate memory
    if(coeff_values == 0){  //If malloc fails returns NULL ptr
        printf("%3s ERROR: Memory allocation failed, File : %s \n"," ", filename);  //Log Error
        if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");    //Close file
        printf("\n");
        free(coeff_values); //TODO Shouldn't be needed
        return -1;  //Return Failed
    }

    //Store values
    printf("--> Array contents =\n");

    if(fread(coeff_values, 8, coeff_num, file_p) != coeff_num) printf("%3s ERROR : fread\n", " ");  //Read the value into the array

    if(pretty_print == 1){
        //Pretty printing not really needed
        for(int i=0; i<coeff_num; i=i+3){   //Loop in groups of 3
            for(int k=i; k<i+3; k++){   //For each group of 3
                if(k<coeff_num){    //Check we haven't accidentally passed further than the array
                    if(coeff_values[k]<0)printf("%3s %lf", " ", coeff_values[k]);   //If negative use less spaces
                    else printf("%4s %lf", " ", coeff_values[k]);   //If positive use more spaces
                }
            }
            printf("\n");   //Print the line end for the group
        }
        printf("\n");
    }
    else printf("\n%4sNot pretty print\n", " ");

    *coeff_num_p = coeff_num;   //Return values to ptr
    *coeff_values_p = &coeff_values[0]; //Return values to ptr
    if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");    //Close file
    return 0;   //Return Success!
}


int read_pcm_wavefile(pcm_wavefile_header_t *header_p, char **data_p, char *filename){

  FILE* file_p;
  pcm_wavefile_header_t head_data;

  const char* RIFF_string = "RIFF"; //String espected
  const char* sub1_string = "fmt";  //String espected
  const char* fmt_string = "WAVE";  //String espected
  const char* schunk2id_string = "data";  //String espected

  int ui8_t_size = sizeof(uint8_t); //Pre calculate values
  int ui16_t_size = sizeof(uint16_t); //Pre calculate values
  int ui32_t_size = sizeof(uint32_t); //Pre calculate values

  printf("\n");

  file_p = fopen(filename, "rb"); //Open the file for reading
  if(file_p == 0){  //If file pointer returns NULL
    printf("%3s ERROR: File unknown \n", " ");  //Log Error
    return -1;
  }

  printf("--> Reading %s data <--\n", filename);

  /********************************************************************************************************************/
  /*                                                 Read Header Data                                                 */
  /********************************************************************************************************************/

  //Read Chunk ID
  //Compare to expected value
  if(fread(head_data.ChunkID, ui8_t_size, 4, file_p) != 4) printf("%3s ERROR : fread : %s\n", " ", "head_data.ChunkID");
  if (strncmp(RIFF_string, (char*)head_data.ChunkID, 4) != 0){ /* Checking to see if the ID is correct */
    printf("%3s ERROR: Incorrect ChunkID expected: %s got: %c%c%c%c\n", " ", RIFF_string, head_data.ChunkID[0], head_data.ChunkID[1], head_data.ChunkID[2], head_data.ChunkID[3]);
    return -1;
  }

  //Read Chunk Size
  //Compare to expected value
  if(fread(&head_data.ChunkSize, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.ChunkSize");

  //Read Format
  //Compare to expected value
  if(fread(head_data.Format, ui8_t_size, 4, file_p) != 4) printf("%3s ERROR : fread : %s\n", " ", "head_data.Format");
  if (strncmp(fmt_string, (char*)head_data.Format, 3) != 0) {
    printf("%3s ERROR: Incorrect Format expected: %s got: %c%c%c%c\n", " ", fmt_string, head_data.Subchunk1ID[0], head_data.Subchunk1ID[1], head_data.Subchunk1ID[2], head_data.Subchunk1ID[3]);
    return -1;
  }

  //Read Sub Chunk ID 1
  //Compare to expected value
  if(fread(head_data.Subchunk1ID, ui8_t_size, 4, file_p) != 4) printf("%3s ERROR : fread : %s\n", " ", "head_data.Subchunk1ID");
  if (strncmp(sub1_string, (char*)head_data.Subchunk1ID, 3) != 0) {
    printf("%3s ERROR: Incorrect Subchunk1ID expected: %s got: %c%c%c%c\n", " ", sub1_string, head_data.Subchunk1ID[0], head_data.Subchunk1ID[1], head_data.Subchunk1ID[2], head_data.Subchunk1ID[3]);
    return -1;
  }

  //Read Sub Chunk 1 Size
  //Compare to expected value
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

  //Read Number of Channels
  //Compare to expected value
  if(fread(&head_data.NumChannels, ui16_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.NumChannels");
  if(head_data.NumChannels != 1){
    printf("%3s ERROR : NumChannels > 1\n", " ");
  }

  //Read SampleRate
  if(fread(&head_data.SampleRate, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.SampleRate");

  //Read ByteRate
  if(fread(&head_data.ByteRate, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.ByteRate");

  //Read Block Align
  if(fread(&head_data.BlockAlign, ui16_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.BlockAlign");

  //Read Bits Per Sample
  if(fread(&head_data.BitsPerSample, ui16_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.BitsPerSample");

  //Read Sub Chunk ID 2
  //Compare to expected value
  if(fread(head_data.Subchunk2ID, ui8_t_size, 4, file_p) != 4) printf("%3s ERROR : fread : %s\n", " ", "head_data.Subchunk2ID");
  if (strncmp(schunk2id_string, (char*)head_data.Subchunk2ID, 3) != 0) {
    printf("%3s ERROR: Incorrect Format expected: %s got: %c%c%c%c\n", " ", schunk2id_string, head_data.Subchunk2ID[0], head_data.Subchunk2ID[1], head_data.Subchunk2ID[2], head_data.Subchunk2ID[3]);
    return -1;
  }

  //Read Sub Chunk 2 Size 
  //Compare to expected value
  if(fread(&head_data.Subchunk2Size, ui32_t_size, 1, file_p) != 1) printf("%3s ERROR : fread : %s\n", " ", "&head_data.Subchunk2Size");
  if(head_data.Subchunk2Size % 2 != 0) printf("WARN: Buffer byte detected in : %s\n", filename);

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

  *data_p = (char*) calloc(head_data.Subchunk2Size, sizeof(char));
  if(fread(*data_p,  sizeof(char), head_data.Subchunk2Size, file_p) != head_data.Subchunk2Size) printf("ERROR: Reading data\n");
  else printf("Success reading data\n");


  if(fclose(file_p) != 0) printf("%3s ERROR : fclose\n", " ");

  return 0;
}

/********************************************************************************************************************/
/*                                                 Write Function                                                   */
/********************************************************************************************************************/

/*
* Input  :  Pointer to struct *header_p
*           Pointer to **data
*           Pointer to output file
* Output :  Wavefile with output file name
*           Success returns 0; Failure returns -1
*/
int write_pcm_wavefile(pcm_wavefile_header_t *header_p, char *data, char *filename_output){ /* I changed the signature from *filename to *filename_output */

  FILE* file_p; //variable of type file
  pcm_wavefile_header_t head_data = *header_p;

  printf("\n");
  file_p = fopen(filename_output, "wb");  //opening the file and storing it in file variable

  if(file_p == 0){  //If file pointer returns NULL
      printf("%3s ERROR: File unknown \n", " ");  //Log Error
      if(freopen(filename_output, "wb", file_p) !=0) {
          printf("%3s ERROR : Unable to create file", " ");
          return -1;
      }
      printf("Created New File : %s\n", filename_output);
      printf("\n");
  }

  /********************************************************************************************************************/
  /*                                                 Write Function                                                   */
  /********************************************************************************************************************/

  if(fwrite(head_data.ChunkID,         4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "head_data.ChunkID"); //Write header to file
  if(fwrite(&head_data.ChunkSize,      4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.ChunkSize");  //Write header to file
  if(fwrite(&head_data.Format,         4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.Format"); //Write header to file

  if(fwrite(&head_data.Subchunk1ID,    4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.Subchunk1ID");  //Write header to file
  if(fwrite(&head_data.Subchunk1Size,  4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.Subchunk1Size");  //Write header to file
  if(fwrite(&head_data.AudioFormat,    2, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.AudioFormat");  //Write header to file
  if(fwrite(&head_data.NumChannels,    2, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.NumChannels");  //Write header to file
  if(fwrite(&head_data.SampleRate,     4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.SampleRate"); //Write header to file
  if(fwrite(&head_data.ByteRate,       4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.ByteRate"); //Write header to file
  if(fwrite(&head_data.BlockAlign,     2, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.BlockAlign"); //Write header to file
  if(fwrite(&head_data.BitsPerSample,  2, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.BitsPerSample");  //Write header to file

  if(fwrite(&head_data.Subchunk2ID,    4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.Subchunk2ID");  //Write header to file
  if(fwrite(&head_data.Subchunk2Size,  4, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "&head_data.Subchunk2Size");  //Write header to file

  /********************************************************************************************************************/
  /*                                                Print Header Data                                                 */
  /********************************************************************************************************************/

  printf("-> ChunkID = %c%c%c%c \n", head_data.ChunkID[0], head_data.ChunkID[1], head_data.ChunkID[2], head_data.ChunkID[3]);
  printf("-> ChunkSize : %u\n", head_data.ChunkSize);
  printf("-> Chunk Format = %c%c%c%c \n\n", head_data.Format[0], head_data.Format[1], head_data.Format[2], head_data.Format[3]);

  printf("-> Subchunk1ID = %c%c%c \n", head_data.Subchunk1ID[0], head_data.Subchunk1ID[1], head_data.Subchunk1ID[2]);
  printf("-> Subchunk1Size : %u\n", head_data.Subchunk1Size);
  printf("-> AudioFormat : %u\n", head_data.AudioFormat);
  printf("-> NumChannels : %u\n", head_data.NumChannels);
  printf("-> SampleRate : %u\n", head_data.SampleRate);
  printf("-> ByteRate : %u\n", head_data.ByteRate);
  printf("-> BlockAlign : %u\n", head_data.BlockAlign);
  printf("-> BitsPerSample : %u\n\n", head_data.BitsPerSample);

  printf("-> SubChuck2ID = %c%c%c%c \n", head_data.Subchunk2ID[0], head_data.Subchunk2ID[1], head_data.Subchunk2ID[2], head_data.Subchunk2ID[3]);
  printf("-> Subchunk2Size : %u\n", head_data.Subchunk2Size);
  
  if(fwrite(data, head_data.Subchunk2Size, 1, file_p) != 1) printf("%3s ERROR : fwrite : %s\n", " ", "head_data.Subchunk2Size");
  else printf("\nSuccess writing data\n");

  fclose(file_p);
  return 0;
}


/********************************************************************************************************************/
/*                                                 Filter Function                                                  */
/********************************************************************************************************************/

int filter(char *filter_filename, char *input_wavefilename, char *output_wavefilename){

  int coeff_num = 0;
  double *coeff_values; //Array of filter coefficient values
  pcm_wavefile_header_t header;
  char *data;

  if (read_coefficients(&coeff_num, &coeff_values, filter_filename)==0){  //If success in reading coefficients
    if (coeff_num <= 0) return -1;  //No coefficients in filter

    if (read_pcm_wavefile(&header, &data, input_wavefilename) != 0){
      printf("%3s ERROR: Unable to read '.wav' file\n\n", " "); //Unable to read wavefile
      free(data);
      free(coeff_values);
      return -1;
    }

    printf("\n");

    int input_len = header.Subchunk2Size;
    int divisor = header.Subchunk1Size / 8;
    char iteration;
    double output[input_len/divisor];
    char output_fixed[input_len];
    int input[input_len/divisor];

    //Create new array with combined values as per header
    for (int i=0; i<(input_len/divisor); i++){
      if(header.Subchunk1Size == 16){
        input[i] = data[i*2]+(data[i*2+1]<<8);
      }
      else if(header.Subchunk1Size == 8){
        //input[i] = data[i];
        printf("Only 16 bit files supported\n");
        printf("ERROR: 8 bit functionality Currently in development\n");
        return -1;
      }
      else{
        printf("ERROR: Incorrect Subchunk1Size\n");
        return -1;
      }

    //For each sample in the file
    for(int i=0; i<input_len/divisor; i++){
      if ((i+1)<coeff_num) iteration = i+1; //Decide if the input exists
      else iteration = coeff_num; //Set input to coefficient

      output[i] = 0;

      for(int j=0; j<iteration; j++){ //For each coefficient filer the input file
        output[i] = output[i] + coeff_values[j] * input[i-j]; //Iterate through each buffer value
      }
      
      if(header.Subchunk1Size == 16){
        output_fixed[i*2] = (char)(int)output; //Split output into fixed point 8 bit numbers
        output_fixed[i*2+1] = (char)((int)output>> 8); //Split output into fixed point 8 bit numbers
      }
      else if(header.Subchunk1Size == 8){
        //output_fixed[i] = (char)(int)output;
        printf("Only 16 bit files supported\n");
        printf("ERROR: 8 bit functionality Currently in development\n");
        return -1;
      }
      else{
        printf("ERROR: Incorrect Subchunk1Size\n");
        return -1;
      }
    }

    write_pcm_wavefile(&header, output_fixed, output_wavefilename); //Write the new data to the new file TODO
    int N = input_len/divisor;
     // time and frequency domain data arrays
    int n, k;             // indices for time and frequency domains
    float Xre[N], Xim[N]; // DFT of x (real and imaginary parts)
    float P[N];           // power spectrum of x
     
    // Generate random discrete-time signal x in range (-1,+1)
     
    // Calculate DFT of x using brute force
    for (k=0 ; k<N ; ++k)
    {
        // Real part of X[k]
        Xre[k] = 0;
        for (n=0 ; n<N ; ++n) Xre[k] += output[n] * cos(n * k * PI2 / N);
         
        // Imaginary part of X[k]
        Xim[k] = 0;
        for (n=0 ; n<N ; ++n) Xim[k] -= output[n] * sin(n * k * PI2 / N);
         
        // Power at kth frequency bin
        P[k] = Xre[k]*Xre[k] + Xim[k]*Xim[k];
    }

    // Output results to MATLAB / Octave M-file for plotting
    FILE *f = fopen("dftplots.m", "w");
    fprintf(f, "n = [0:%d];\n", N-1);
    fprintf(f, "x = [ ");
    for (n=0 ; n<N ; ++n) fprintf(f, "%f ", output[n]);
    fprintf(f, "];\n");
    fprintf(f, "Xre = [ ");
    for (k=0 ; k<N ; ++k) fprintf(f, "%f ", Xre[k]);
    fprintf(f, "];\n");
    fprintf(f, "Xim = [ ");
    for (k=0 ; k<N ; ++k) fprintf(f, "%f ", Xim[k]);
    fprintf(f, "];\n");
    fprintf(f, "P = [ ");
    for (k=0 ; k<N ; ++k) fprintf(f, "%f ", P[k]);
    fprintf(f, "];\n");
    fprintf(f, "subplot(3,1,1)\nplot(n,x)\n");
    fprintf(f, "xlim([0 %d])\n", N-1);
    fprintf(f, "subplot(3,1,2)\nplot(n,Xre,n,Xim)\n");
    fprintf(f, "xlim([0 %d])\n", N-1);
    fprintf(f, "subplot(3,1,3)\nstem(n,P)\n");
    fprintf(f, "xlim([0 %d])\n", N-1);
    fclose(f);


    printf("\n");
    }
    free(coeff_values);
    return 0; //Success
}
}




int main(void){

    char *filename_filter[] = {"filterBP.bin","filterHP.bin","filterLP.bin","filterSB.bin",
                             "filter-Bad-0.bin","filter-Bad-1.bin","filter-Bad-2.bin",NULL};
    char *filename_audio[] = {"8k16bitpcm.wav", "11k8bitpcm.wav","11k16bitpcm.wav",
                            "8kmp38.wav","8k16bitpcm-Bad-0.wav",
                            "8k16bitpcm-Bad-1.wav","8k16bitpcm-Bad-2.wav","8k16bitpcm-Bad-3.wav",NULL};
    // Loop variables
    int i;
    int j;


    char filename_output[100];
    printf("-> Question 3...\n");
    for (i=0;i<4;i++){
        printf("--> File name %4s = %s: \n", " ", filename_filter[i]);
        snprintf(filename_output,100,"%s-%s",filename_filter[i],filename_audio[0]);
        if (filter(filename_filter[i], filename_audio[0], filename_output)==0)
        {
        printf("--> Written %s.\n", filename_output);
        }
    }


   
     
    
     
    // exit normally
    return 0;
}