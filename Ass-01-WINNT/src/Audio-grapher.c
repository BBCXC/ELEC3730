#include "Ass-01.h"

int audio_print(max_input, min_input, input_len, scaled_height, scaled_width){
	//iterate from maximum value to minimum value
	for(i=max_input; i>min_input;){
		//Print axis
		printf("%3s |", " ");

		//Iterate through data values looking for value that lies within range
		for(int k=0; k=input_len; k++){
			if(input[k] >= i && input[k] < i + scaled_height){
				for(int j=0; j<space_between; j++){
					printf(" ");
				}
				printf("x");
				space_between = 0
			}
			space_between++;
		}
		i = i - scaled_height;
	}
}


int audio_grapher(pcm_wavefile_header_t *header, char* data, int width, int height){

	int scaled_width;
	int scaled_height;
	int min_input = ;
	int max_input = ;

	
    int divisor = header.Subchunk1Size / 8;
    int input_len = header.Subchunk2Size / divisor;
    int input[input_len];

    if(header.Subchunk1Size == 16){
        min_input = max_input = data[0*2]+(data[0*2+1]<<8);
	}
	else if(header.Subchunk1Size == 8){
		//input[i] = data[i];
		printf("Currently in development\n");
	}
	else {
		printf("ERROR\n");
		return -1;
	}

    //Create new array with combined values as per header
    for (int i=1; i<(input_len); i++){
      if(header.Subchunk1Size == 16){
        input[i] = data[i*2]+(data[i*2+1]<<8);
      }
      else if(header.Subchunk1Size == 8){
        //input[i] = data[i];
        printf("Currently in development\n");
      }

      //Find min and max values
      if(intput[i] > max_input){
      	max_input = input[i];
      }
      else if(intput[i] < min_input){
      	min_input = input[i];
      }

    }

    //Currently i know the minimum and maximum values for my data
    //I know the length of the width
    //

    //Figure out which side of the x axis my data lies on

    if(min_input > 0){ //Data must all lie above the x axis
    	//Print data
    	if(audio_print(max_input, min_input, input_len, scaled_height, scaled_width) != 0) printf("ERROR");

    	//Print x axis
    }
    else if(max_input < 0){ //Data must lie below the x axis
    	//Print the x axis first

    	//Print data
    	if(audio_print(max_input, min_input, input_len, scaled_height, scaled_width) != 0) printf("ERROR");
    }
    else{ //Data must be either side of the axis
    	//Print data above axis
    	if(audio_print(max_input, 0, input_len, scaled_height, scaled_width) != 0) printf("ERROR");

    	//Print x axis and data that lies here

    	//Print data below axis
    	if(audio_print(0, min_input, input_len, scaled_height, scaled_width) != 0) printf("ERROR");
    }

    return 0;
}