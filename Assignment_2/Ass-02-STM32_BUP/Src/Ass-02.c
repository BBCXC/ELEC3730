//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE 
//

void Ass_02_Main(void)
{
#ifdef STM32F407xx
  uint16_t i=0;
#endif

  // Initialise

  CommandLineParserInit();
#ifdef STM32F407xx
  grid_struct grid_space;
  char **input;

  CalculatorInit(&grid_space, &input);
  printf("Left init\n");
#endif

  // Loop indefinitely
  while (1)
  {
    CommandLineParserProcess();
#ifdef STM32F407xx
    //printf("Starting process\n");
    CalculatorProcess(&grid_space, &input);
#endif

#ifdef STM32F407xx
    if (i++ > 10000)
    {
      HAL_GPIO_TogglePin(GPIOD, LD3_Pin); // Toggle LED3
      i=0;
    }
#endif
  }
}
