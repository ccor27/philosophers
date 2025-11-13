#include "philo.h"

void	ft_error_exit(const char *msg)
{
	if (msg)
		printf("%s",msg);
	exit(EXIT_FAILURE);
}


/**
 * Functio to free memory allocated and exit the project
 */
void	ft_free_and_exit(const char *msg, t_data *data)
{
	if (!data)
		ft_error_exit("Error: the data struct is null");
	if (data->forks)
		ft_free_forks(data);
	if (data->philos)
		ft_free_philos(data);
	ft_error_exit(msg);
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
	if(&data->data_mtx)
		pthread_mutex_destroy(&data->data_mtx);
	if (&data->print_mtx)
		pthread_mutex_destroy(&data->print_mtx);

}
//TODO: finihs
void	ft_free_philos(t_data *data)
{
	int	i;
	t_philo *philo;

	i = 0;
	philo=NULL;
	while (i < data->philo_number)
	{
		philo=&data->philos[i];
		pthread_mutex_destroy(&philo->data_mtx);
        i++;
	}
	free(data->philos);
    data->philos = NULL;
	if(data->monitor)
		pthread_mutex_destroy(&data->monitor);
}
