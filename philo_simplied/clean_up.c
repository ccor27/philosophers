#include "philo.h"

/**
 * Function to printa message and exit the program
 * hanlding both cases, error and sucess
 */
void	ft_exit(const char *msg, int is_error)
{
	if (msg)
		printf("%s", msg);
	if (is_error)
		exit(EXIT_FAILURE);
	else
		exit(EXIT_SUCCESS);
}

/**
 * Functio to free memory allocated and exit the project
 * showing a message (optional)
 */
void	ft_free_and_exit(const char *msg, t_data *data, int is_error)
{
	if (!data)
		ft_exit("Error: the data struct is null", 1);
	if (data->forks)
		ft_free_forks(data);
	if (data->philos)
		ft_free_philos(data);
	pthread_mutex_destroy(&data->data_mtx);
	pthread_mutex_destroy(&data->print_mtx);
	ft_exit(msg, is_error);
}

/**
 * Function to free the memory allocated
 * to the array of forks and destroy
 * the forks's mutex
 */
void	ft_free_forks(t_data *data)
{
	int		i;
	t_fork	*fork;

	i = 0;
	while (i < data->philo_number)
	{
		fork = &data->forks[i];
		pthread_mutex_destroy(&fork->mutex);
		i++;
	}
	fork = NULL;
	free(data->forks);
	data->forks = NULL;
}

/**
 * Function to free the memory allocated
 * to the array of philos and destroy
 * the philo's mutex
 */
void	ft_free_philos(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = NULL;
	while (i < data->philo_number)
	{
		philo = &data->philos[i];
		pthread_mutex_destroy(&philo->data_mtx);
		i++;
	}
	philo = NULL;
	free(data->philos);
	data->philos = NULL;
}
