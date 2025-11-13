#include "philo.h"

/**
 * Function to handle the philo routine
 * basically here we validate before each action
 * if the routine should stop, it should stop if any
 * philo died or all are full, in addition we block
 * the fork each time a philo takes it
 */
int	ft_routine(pthread_mutex_t *f1_mtx, pthread_mutex_t *f2_mtx, t_philo *philo,
		t_data *data)
{
	if (ft_should_stop(data, philo))
		return (1);
	ft_handle_mutexes(data, f1_mtx, LOCK);
	ft_print_action(data, philo, TAKING);
	if (ft_should_stop(data, philo))
	{
		ft_handle_mutexes(data, f1_mtx, UNLOCK);
		return (1);
	}
	ft_handle_mutexes(data, f2_mtx, LOCK);
	ft_print_action(data, philo, TAKING);
	ft_eat(data, philo);
	ft_handle_mutexes(data, f1_mtx, UNLOCK);
	ft_handle_mutexes(data, f2_mtx, UNLOCK);
	if (ft_should_stop(data, philo))
		return (1);
	ft_think(data, philo);
	if (ft_should_stop(data, philo))
		return (1);
	ft_sleep(data, philo);
	return (0);
}

/**
 * Function to handle the life cycle of a philo
 * Basically here we assign the forks to the philo,
 * depending if its positions, then if the position
 * of the philo is odd, we send it to sleep in order to
 * avoid all take forks and make a deadlock, after that
 * do the philo's routine
 */
void	ft_philo_cycle(t_philo *philo, t_data *data)
{
	pthread_mutex_t	*f1_mtx;
	pthread_mutex_t	*f2_mtx;

	if (philo->id % 2 == 0)
	{
		f1_mtx = &philo->left_fork->mutex;
		f2_mtx = &philo->right_fork->mutex;
	}
	else
	{
		f1_mtx = &philo->right_fork->mutex;
		f2_mtx = &philo->left_fork->mutex;
	}
	if (philo->id % 2 == 0)
		usleep((data->time_to_eat / 2) * 1000);
	while (1)
	{
		if (ft_routine(f1_mtx, f2_mtx, philo, data))
			break ;
	}
}

/**
 * Principal function to monitor if any philo has
 * died by hungry
 */
void	ft_monitor_process(t_data *data)
{
	while (1)
	{
		ft_handle_mutexes(data, &data->data_mtx, LOCK);
		if (data->end_simulation == 1)
		{
			ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
			printf("end_simuation has: %ld\n", data->end_simulation);
			break ;
		}
		ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
		if (data->number_of_meals != -1 && ft_check_all_full(data))
		{
			ft_handle_mutexes(data, &data->data_mtx, LOCK);
			data->end_simulation = 1;
			ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
			break ;
		}
		if (ft_check_philo_death(data, data->time_to_die))
			break ;
		usleep(1000);
	}
}

/**
 * Auxiliary function to check if indeed any philo
 * has died by hungry
 */
int	ft_check_philo_death(t_data *data, long time_to_die)
{
	int		i;
	long	time_since_last_meal;
	t_philo	*philo;

	i = 0;
	while (i < data->philo_number)
	{
		philo = &data->philos[i];
		ft_handle_mutexes(data, &philo->data_mtx, LOCK);
		time_since_last_meal = ft_get_time() - philo->time_last_meal;
		ft_handle_mutexes(data, &philo->data_mtx, UNLOCK);
		if (time_since_last_meal > time_to_die)
		{
			ft_handle_mutexes(data, &data->data_mtx, LOCK);
			data->end_simulation = 1;
			ft_handle_mutexes(data, &data->print_mtx, LOCK);
			printf("\033[41m%ld, philo %d died\033[0m\n", ft_get_time(),
				philo->id);
			ft_handle_mutexes(data, &data->print_mtx, UNLOCK);
			ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
			return (1);
		}
		i++;
	}
	return (0);
}
