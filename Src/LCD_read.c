#include "Ass-02.h"

//Given grid stuct and position touched, returns area touched
int get_touch_pos(grid_struct grid_space, int display.x, int display.y){
    for(int i=0; i<20; i++){
        if((display.x >= grid_space.Area_[i][0]) && (display.x <= grid_space.Area_[i][1]) && (display.y <= grid_space.Area_[i][2]) && (display.y >= grid_space.Area_[i][3])){
            return i;
        }
    }
}