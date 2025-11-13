#include "philo.h"

/**
 * Function to parse the arguments and store them
 * into data struct
 * ./philo 5 800 200 200 [5]
 */
//TODO: validate is the value of the number of meals is < 0
void	ft_parse_and_store(int argc, t_data *data, char **argv)
{
	data->philo_number = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]) ;
	data->time_to_eat = ft_atol(argv[3]) ;
	data->time_to_sleep = ft_atol(argv[4]) ;
	data->number_of_meals = -1;
	if(argc == 6)
		data->number_of_meals = ft_atol(argv[5]);
	if(data->philo_number == 0)
		ft_error_exit("The number of philos must be > 1\n");
	if (data->time_to_die < 60 || data->time_to_eat < 60
		|| data->time_to_sleep < 60)
		ft_error_exit("The timestamps should be grather than 60ms\n");
	if (data->number_of_meals < -1)
			ft_error_exit("The number of meals must be >=0 \n");
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
	//ft_show_data(data);
}

/**
 * Function to initialize the philos
 * //TODO: I've changed the i = 0 to i = -1
 */
void	ft_init_philos(t_data *data)
{
	int		i;
	t_philo	*philo;

	data->philos = malloc(sizeof(t_philo) * data->philo_number);
	if (!data->philos)
		ft_free_and_exit("Failed to allocate memory for forks", data);
	i = 0;
	data->start_simulation = ft_get_time();
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
		philo->right_fork = &data->forks[i];
		philo->left_fork = &data->forks[(i + 1) % data->philo_number];
		if (ft_handle_thread(&philo->thread_id, &ft_philo_starter, philo, CREATE,
				data) != 0)
		if (ft_handle_thread(&philo->thread_id, NULL, philo, CREATE,
				data) != 0)
			ft_free_and_exit(NULL, data);
		i++;
	}
	if (ft_handle_thread(&data->monitor, &ft_monitor_starter, data, CREATE, data) != 0)
        ft_free_and_exit("Failed to create monitor thread", data);
	ft_join_threads(data);
}

void	ft_join_threads(t_data *data)
{
	int  i;
	t_philo *philo;

	i = 0;
	ft_handle_thread(&data->monitor, NULL, NULL, JOIN,
				data);
	while (i < data->philo_number)
	{
		philo = &data->philos[i];
		ft_handle_thread(&philo->thread_id, NULL, NULL, JOIN,
				data);
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

void	*ft_monitor_starter(void *arg)
{
	t_data *data;
	data = (t_data *)arg;
	ft_monitor_process(data);
	return(NULL);
}

// static void ft_show_data(t_data *data)
// {
// 	 if (!data)
//         return;

//     printf("philo_number: %ld\n", data->philo_number);
//     printf("time_to_die: %ld\n", data->time_to_die);
//     printf("time_to_eat: %ld\n", data->time_to_eat);
//     printf("time_to_sleep: %ld\n", data->time_to_sleep);
//     printf("number_of_meals: %ld\n", data->number_of_meals);
//     printf("start_simulation: %ld\n", data->start_simulation);
//     printf("end_simulation: %ld\n", data->end_simulation);

// 	int i;

// 	i = 0;
// 	while (i < data->philo_number)
// 	{
// 		printf("the philo : %d\n",data->philos[i].id);
// 		printf("the right fork : %d\n",data->philos[i].right_fork->fork_id);
// 		printf("the left fork : %d\n\n",data->philos[i].left_fork->fork_id);
// 		i++;
// 	}
	
// }