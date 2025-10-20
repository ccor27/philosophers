#include "philo.h"

void error_exit(const char *msg)
{
	if(msg)
		printf("Your input should be like: ./philo 5 800 200 200 [5]");
	exit(EXIT_FAILURE);

}
