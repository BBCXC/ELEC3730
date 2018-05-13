struct window {
  int width = 250;
  int height = 142;
  int position[4] = {x_min, x_max, y_min, y_max};

  int bg_colour;
  int line_colour;
  int grid_colour;

  int zoom_coeff = 1;         // Number between 1 and MAX_ZOOM
  int window_buflen;  // Number of input values that are mapped to the
                          // window buffer

  int next = 0;	// Holds the position in the window buffer array
  				// that is the latest filled

}

void windowInit(){
	int Window_buffer[window.width][2] = ()calloc();

	window.buflen = Max_Samples / zoom_coeff / window.width;
}

// Only called when input buffer has been populated with next 40 samples
int populateWindow_avg(){

	// Increment Window buffer position
	window.next++;
	if(window.next == window.width){window.next = 0;}

	// Increment Input buffer position
	input.next++;
	if(input.next == Max_Samples){input.next = 0;}

	Window_buffer[window.next][0] = Input_buffer[input.next];

	for(int i=1; i<window.buflen; i++){
		input.next++;
		Window_buffer[window.next][0] += Input_buffer[input.next];
	}

	// Window buffer now contains average of last 40 samples
	return 0;
}

// If zoom_coeff is changed repopulate the Window_buffer with nex mapping
int re_populateWindow_avg(){

	// Figure out how im zooming
	// Zoom on middle
	// Zoom on start
	// Zoom on end

	// Increment from 0 to window.width
	// 

	// // Increment Window buffer position
	// window.next++;
	// if(window.next == window.width){window.next = 0;}

	// // Increment Input buffer position
	// input.next++;
	// if(input.next == Max_Samples){input.next = 0;}

	// Window_buffer[window.next][0] = Input_buffer[input.next];

	// for(int i=1; i<window.buflen; i++){
	// 	input.next++;
	// 	Window_buffer[window.next][0] += Input_buffer[input.next];
	// }

	// // Window buffer now contains average of last 40 samples
	// return 0;
}



#define num_buttons 12
struct button {
  int position[num_buttons][4] = {(x_min, x_max, y_min, y_max),   // Button 1
                                  (x_min, x_max, y_min, y_max)};  // Button 2
  int item[num_buttons];
  int
}

/*
        Buttons
        - Play
        - Stop
        - Save
        - Load
        - New
        - Reset (Reset zoom)
        - Zoom In
        - Zoom out
        - Move Left
        - Move Right
        - Up arrow
        - Down arrow

*/

| = 2px
| 64px|     250px      |
********************************** - 2px
* play*                          *
*     *                          *
*     *                          *  ~142px
* stop*                          *
*     *0                       10*
********************************** - 2px
* save* timestamp  <  >  +  - r  *
* load* |FILENAME        |    ^  *  ~92px
* new *           1 of 10    !^  *
********************************** - 2px

void LCDPopulate(){
	// TODO below has been commented out, hard coding for now, should be done on the fly
	// int display_height
	// int display_width

	window.position[1] = display_width - display_line; // 318
	window.position[0] = window.position[1] - window.width; // 68
	window.position[2] = display_line; // 2
	window.position[3] = window_height; // 144

	// int button_width = window.position[0] - display_line;

	// // Populate left and side buttons x_min x_max
	// for(int i=0; i<5; i++){
	// 	button.position[i][0] = display_line;
	// 	button.position[i][1] = button_width;
	// }

	// // Populate Play and Stop y_min y_max
	// int temp1 = 2
	// for(int i=0; i<temp1; i++){
	// 	button.position[i][2] =	display_line + (i * (window_height - display_line) / temp1);
	// 	button.position[i][3] = display_line + ((i+1) * (window_height - display_line) / temp1);
	// }

	// // Populate Save Load New y_min y_max
	// int temp2 = 3;
	// int panel_height = 3 * display_line - window.width;
	// for(int i=temp1; i<temp2; i++){
	// 	button.position[i][2] =	display_line + (i * (panel_height - display_line) / temp2);
	// 	button.position[i][3] = display_line + ((i+1) * (panel_height - display_line) / temp2);
	// }


	// Tempted to hardcode these values, maybe there's a better way
	// TODO Remove hard coded values

	button.item[0] = "Play";
	button.position[0][0] = 2;
	button.position[0][1] = 66;
	button.position[0][2] = 2;
	button.position[0][3] = 70;

	button.item[1] = "Stop";
	button.position[1][0] = 2;
	button.position[1][1] = 66;
	button.position[1][2] = 68;
	button.position[1][3] = 144;

	button.item[2] = "Save";
	button.position[2][0] = 2;
	button.position[2][1] = 66;
	button.position[2][2] = 146;
	button.position[2][3] = 176;

	button.item[3] = "Load";
	button.position[3][0] = 2;
	button.position[3][1] = 66;
	button.position[3][2] = 178;
	button.position[3][3] = 208;

	button.item[4] = "New";
	button.position[4][0] = 2;
	button.position[4][1] = 66;
	button.position[4][2] = 210;
	button.position[4][3] = 238;

	button.item[5] = "<";
	button.position[5][0] = 170;
	button.position[5][1] = 198;
	button.position[5][2] = 146;
	button.position[5][3] = 176;

	button.item[6] = ">";
	button.position[6][0] = 200;
	button.position[6][1] = 228;
	button.position[6][2] = 146;
	button.position[6][3] = 176;

	button.item[7] = "+";
	button.position[7][0] = 230;
	button.position[7][1] = 258;
	button.position[7][2] = 146;
	button.position[7][3] = 176;

	button.item[8] = "-";
	button.position[8][0] = 260;
	button.position[8][1] = 288;
	button.position[8][2] = 146;
	button.position[8][3] = 176;

	button.item[9] = "r";
	button.position[9][0] = 290;
	button.position[9][1] = 318;
	button.position[9][2] = 146;
	button.position[9][3] = 176;

	button.item[10] = "^";
	button.position[10][0] = 290;
	button.position[10][1] = 318;
	button.position[10][2] = 178;
	button.position[10][3] = 208;

	button.item[11] = "!^";
	button.position[11][0] = 290;
	button.position[11][1] = 318;
	button.position[11][2] = 210;
	button.position[11][3] = 238;

	button.item[12] = "";	//Timestamp
	button.position[12][0] = 68;
	button.position[12][1] = 168;
	button.position[12][2] = 146;
	button.position[12][3] = 176;

	button.item[13] = "";	//FileName
	button.position[13][0] = 68;
	button.position[13][1] = 288;
	button.position[13][2] = 178;
	button.position[13][3] = 208;

	button.item[14] = "";	//FileNumber
	button.position[14][0] = 68;
	button.position[14][1] = 288;
	button.position[14][2] = 210;
	button.position[14][3] = 238;
}