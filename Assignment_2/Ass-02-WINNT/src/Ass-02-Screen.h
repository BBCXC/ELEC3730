#ifndef ASS_02_SCREEN_H_
#define ASS_02_SCREEN_H_

typedef struct {
  int Area[25][5];
  char **items;
} grid_struct;
grid_struct grid_space_p;

extern void Screen_Init(void);
extern int Get_Area(int pos1, int pos2);
extern char *Get_Item(int Item_Value);
extern void Set_Area(int pos1, int pos2, int value);
extern void Set_Item(int pos, int Item_number);

#endif /* ASS_02_SCREEN_H_ */
