#include "philo.h"

/**
 * Function to parse the arguments and store them
 * into data struct
 * ./philo 5 800 200 200 [5]
 */
void	ft_parse_and_store(int argc, t_data *data, char **argv)
{
	data->philo_number = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	data->number_of_meals = -1;
	if (argc == 6)
	{
		data->number_of_meals = ft_atol(argv[5]);
		if (data->number_of_meals == 0)
			ft_exit("Number of meals should be grater then 0!", 1);
	}
	else
		data->number_of_meals = -1;
	if (data->philo_number == 0)
		ft_exit("The number of philos must be > 1\n", 1);
	if (data->time_to_die < 60 || data->time_to_eat < 60
		|| data->time_to_sleep < 60)
		ft_exit("The timestamps should be grather than 60ms\n", 1);
	data->end_simulation = -1;
}

/**
 * Function to initialize the structs
 * forks and philos
 */
void	ft_init_data(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(t_fork) * data->philo_number);
	if (!data->forks)
		ft_exit("Failed to allocate memory for forks", 1);
	data->philos = malloc(sizeof(t_philo) * data->philo_number);
	if (!data->philos)
		ft_free_and_exit("Failed to allocate memory for philos", data, 1);
	if (ft_handle_mutexes(data, &data->data_mtx, INIT) != 0)
		ft_free_and_exit(NULL, data, 1);
	if (ft_handle_mutexes(data, &data->print_mtx, INIT) != 0)
		ft_free_and_exit(NULL, data, 1);
	while (i < data->philo_number)
	{
		data->forks[i].fork_id = (i + 1);
		if (ft_handle_mutexes(data, &data->forks[i].mutex, INIT) != 0)
			ft_free_and_exit(NULL, data, 1);
		i++;
	}
	i = 0;
	while (i < data->philo_number)
		ft_init_philo(data, i++);
	ft_init_monitor(data);
	ft_join_threads(data);
}

/**
 * Function to initialize a philo
 * and handlig the case of any error
 * in  both of mutex and thread
 */
void	ft_init_philo(t_data *data, int i)
{
	t_philo	*philo;
	int		result;

	philo = &data->philos[i];
	philo->id = (i + 1);
	philo->meals_counter = 0;
	philo->is_full = 0;
	philo->time_last_meal = ft_get_time();
	philo->data = data;
	if (ft_handle_mutexes(data, &philo->data_mtx, INIT) != 0)
		ft_free_and_exit(NULL, data, 1);
	philo->right_fork = &data->forks[i];
	philo->left_fork = &data->forks[(i + 1) % data->philo_number];
	result = ft_handle_thread(&philo->thread_id, &ft_philo_starter, philo,
			CREATE);
	if (result != 0)
	{
		pthread_mutex_lock(&data->data_mtx);
		data->end_simulation = 1;
		ft_handle_thread_error(CREATE, result);
		pthread_mutex_unlock(&data->data_mtx);
		ft_free_and_exit(NULL, data, 1);
	}
}

/**
 * Function to create and join the monitor thread
 * handling the possible cases of error
 */
void	ft_init_monitor(t_data *data)
{
	int	creation_result;
	int	joining_result;

	creation_result = ft_handle_thread(&data->monitor, &ft_monitor_starter,
			data, CREATE);
	if (creation_result != 0)
	{
		pthread_mutex_lock(&data->data_mtx);
		data->end_simulation = 1;
		ft_handle_thread_error(CREATE, creation_result);
		pthread_mutex_unlock(&data->data_mtx);
		ft_free_and_exit(NULL, data, 1);
	}
	joining_result = ft_handle_thread(&data->monitor, NULL, NULL, JOIN);
	if (joining_result != 0)
	{
		pthread_mutex_lock(&data->data_mtx);
		data->end_simulation = 1;
		ft_handle_thread_error(JOIN, creation_result);
		pthread_mutex_unlock(&data->data_mtx);
		ft_free_and_exit(NULL, data, 1);
	}
}

/**
 * Function to join all the threads
 * and handling the case of any error
 */
void	ft_join_threads(t_data *data)
{
	int		i;
	int		result;
	t_philo	*philo;

	i = 0;
	while (i < data->philo_number)
	{
		philo = &data->philos[i];
		result = ft_handle_thread(&philo->thread_id, NULL, NULL, JOIN);
		if (result != 0)
		{
			pthread_mutex_lock(&data->data_mtx);
			data->end_simulation = 1;
			ft_handle_thread_error(CREATE, result);
			pthread_mutex_unlock(&data->data_mtx);
			ft_free_and_exit(NULL, data, 1);
		}
		i++;
	}
}
