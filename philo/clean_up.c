#include "philo.h"

void	ft_error_exit(const char *msg)
{
	if (msg)
		printf("Your input should be like: ./philo 5 800 200 200 [5]");
	exit(EXIT_FAILURE);
}

void	ft_free_and_exit(const char *msg, t_data *data)
{
	if (!data)
		ft_error_exit("Error: the data struct is null");
	if (data->forks)
		ft_free_forks(data);
	if (data->philos)
		ft_free_philos(data);
}
/**
 * Function to destroy fork's mutexes
 * and free the data's forks memory
 */
void	ft_free_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_number)
	{
		pthread_mutex_destroy(&data->forks[i].mutex);
		i++;
	}
	free(data->forks);
	data->forks = NULL;
}

void	ft_free_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_number)
	{
	}
}
