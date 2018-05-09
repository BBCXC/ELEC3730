/*
 * Author      : Mitchell Marotta C3258958
 *               Taylor Young C3206230
 * Date        : 4 May 2018
 * Description : Initilise some variables
 *               Initilise some arrays
 */

#include "Ass-02.h"

void Ass_02_Main(void) {
#ifdef STM32F407xx
  uint16_t i = 0;
#endif
  // Initilise structs for specific functions
  Debug_Init();
  Equation_Init();
  Screen_Init();

  CommandLineParserInit();

#ifdef STM32F407xx
  // Initilise display
  DisplayInit();

  // Initilise LCD for specific mode
  if (Get_Graph_Mode() == 0) {
    CalculatorInit();
  } else if (Get_Graph_Mode() == 1) {
    GraphInit();
  }
#endif

  // Loop indefinitely
  while (1) {
    CommandLineParserProcess();
#ifdef STM32F407xx
    if (Get_Graph_Mode() == 0) {
      CalculatorProcess();
    }
#endif

#ifdef STM32F407xx
    if (i++ > 10000) {
      HAL_GPIO_TogglePin(GPIOD, LD3_Pin);  // Toggle LED3
      i = 0;
    }
#endif
  }
}
