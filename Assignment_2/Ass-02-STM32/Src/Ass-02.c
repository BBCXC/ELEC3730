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
  static char *item[42] = {"=", "+", "ANS", ".", "0", ">", "-", "3", "2", "1",
   		            "<", "/", "6", "5", "4", "AC", "x", "9", "8", "7", "DEL",
                      "=", "(", "ANS", "sqrt", "^", ">", ")", "atan", "acos", "asin",
                      "<", "pi", "tan", "cos", "sin", "AC", "", "log", "ln", "exp", "DEL"};

  grid_space_p.items = item;
  grid_space_p.num_char = 0;
  grid_space_p.prev_ans = 0;
  grid_space_p.formula = NULL;
  grid_space_p.equation = NULL;
  CalculatorInit();
#endif

  // Loop indefinitely
  while (1)
  {
    CommandLineParserProcess();
#ifdef STM32F407xx
    CalculatorProcess();
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
