#include "philo.h"


int	main(int argc, char **argv)
{
	t_data data;
	if(!(argc >=5 && argc <= 6))
		ft_exit("Error: not enough or too much arguments!",1);
	//1) validate the inputs and store them in the data struct
	memset(&data,0,sizeof(data));
	ft_parse_and_store(argc,&data,argv);
	//2) initialize data and start simulation
	ft_init_data(&data);
	//3) clean data and exist, no leaks when all philos are full or a philo dies
	ft_free_and_exit(NULL,&data,0);
	return (0);
}
