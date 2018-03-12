//     $Date: 2017-03-13 06:50:40 +1100 (Mon, 13 Mar 2017) $
// $Revision: 818 $
//   $Author: Peter $

#include "Ass-01.h"

void serial_string_parser(void)
{
	char c;
	int i,j;
    char command_line[101];
	char **array_of_words;
	int count;

	// Get one line of input
	printf("--> Enter text:\n");
	i=0;
	c=getchar();
	while (c != 13 && i < 100)
	{
		printf("%c",c);
		command_line[i]=c;
		i++;
	    c=getchar();
	}
	command_line[i]=0;

	// Parse the input and print result
  	count = string_parser(command_line, &array_of_words);
   	printf("\n--> count = %d\n", count);
   	for (j=0;j<count;j++)
   	{
   	  printf("---> %2d: '%s'\n", j+1, (array_of_words)[j]);
   	}
   	if (count != 0)
   	{
   	  free(array_of_words[0]);
   	  free(array_of_words);
   	}
}
