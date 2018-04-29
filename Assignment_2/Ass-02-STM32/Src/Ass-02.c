/*
 * Author      : Mitchell Marotta C3258958
 *               Taylor Young C3206230
 * Date        : 4 May 2018
 * Description : Initilise some variables
 *               Initilise some arrays
*/

#include "Ass-02.h"

void Ass_02_Main(void)
{
#ifdef STM32F407xx
  uint16_t i=0;
#endif

  // Initialise
  info.debug = 1;
  info.system = 1;
  info.formula_mode = 0;

  CommandLineParserInit();
#ifdef STM32F407xx
  static char *item[42] = {"=", "+", "ANS", ".", "0", ">", "-", "3", "2", "1",
   		                     "<", "/", "6", "5", "4", "AC", "x", "9", "8", "7", "DEL",
                           "=", "(", "ANS", "sqrt(", "^", ">", ")", "atan(", "acos(", "asin(",
                           "<", "pi", "tan(", "cos(", "sin(", "AC", "", "log(", "ln(", "exp(", "DEL"};

  grid_space_p.items = item;
  output.prev_ans = 0;
  output.result = 0;
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
