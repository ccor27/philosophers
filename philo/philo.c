#include "philo.h"


int	main(int argc, char **argv)
{
	t_data data;
	if(!(argc >=5 && argc <= 6))
		ft_error_exit("Error: not enough or too much arguments!");
	//if the input is correct

	//1) validate the inputs and store them in the data struct
	ft_parse_and_store(&data,argv);
	//2) initialize the philos and forks in addition decided how distribut the forks
	ft_init_data(&data);
	//3) make the simulation
	//dinner(&data);
	//4) clean data and exist, no leaks when all philos are full or a philo dies
	//clean_up(&data)
	return (0);
}
