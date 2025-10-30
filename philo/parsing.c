#include "philo.h"

/**
 * Function to parse the arguments and store them
 * into data struct
 * ./philo 5 800 200 200 [5]
 */
void	ft_parse_and_store(t_data *data, char **argv)
{
	data->philo_number = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]) * 1000;
	data->time_to_eat = ft_atol(argv[3]) * 1000;
	data->time_to_sleep = ft_atol(argv[4]) * 1000;
	if (data->time_to_die < 60000 || data->time_to_eat < 60000
		|| data->time_to_sleep < 60000)
		ft_error_exit("The timestamps should be grather than 60ms");
	if (argv[5])
		data->number_of_meals = ft_atol(argv[5]);
	else
		data->number_of_meals = -1;
	data->end_simulation = -1;
}

/**
 * Function to initialize the structs
 * forks and philos
 */
void	ft_init_data(t_data *data)
{
	int	i;

	if (!data)
		ft_error_exit("Error: the data struct is null");
	// init forks
	data->forks = malloc(sizeof(t_fork) * data->philo_number);
	if (!data->forks)
		ft_error_exit("Failed to allocate memory for forks");
	i = 0;
	if (ft_handle_mutexes(data, &data->data_mtx, INIT) != 0)
		ft_free_and_exit(NULL, data);
	if (ft_handle_mutexes(data, &data->print_mtx, INIT) != 0)
		ft_free_and_exit(NULL, data);
	while (i < data->philo_number)
	{
		data->forks[i].fork_id = (i + 1);
		if (ft_handle_mutexes(data, &data->forks[i].mutex, INIT) != 0)
			ft_free_and_exit(NULL, data);
		i++;
	}
	// init philos
	ft_init_philos(data);
}

/**
 * Function to initialize the philos
 */
void	ft_init_philos(t_data *data)
{
	int		i;
	t_philo	*philo;

	data->philos = malloc(sizeof(t_philo) * data->philo_number);
	if (!data->philos)
		ft_free_and_exit("Failed to allocate memory for forks", data);
	i = 0;
	data->start_simulation = ft_get_time_in_ms();
	philo = NULL;
	while (i < data->philo_number)
	{
		philo = &data->philos[i];
		philo->id = (i + 1);
		philo->meals_counter = 0;
		philo->is_full = 0;
		philo->time_last_meal = data->start_simulation; //?
		philo->data = data;
		if (ft_handle_mutexes(data, &philo->data_mtx, INIT) != 0)
			ft_free_and_exit(NULL, data);
		philo->left_fork = &data->forks[i];
		philo->right_fork = &data->forks[(i + 1) % data->philo_number];
		if (ft_handle_thread(philo->thread_id, &ft_philo_starter, philo, CREATE,
				data) != 0)
			ft_free_and_exit(NULL, data);
		i++;
	}
}

/**
 * Wrapper function to start the threads correctly
 * and call the function that each one will use
 */
void	*ft_philo_starter(void *arg)
{
	t_philo *philo;
	t_data *data;

	philo = (t_philo *)arg;
	data = philo->data;
	ft_philo_cycle(philo, data);
	return (NULL);
}