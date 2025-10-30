#include "philo.h"

// function hadle the cycle of each philo
void	ft_philo_cycle(t_philo *philo, t_data *data)
{
	while (1)
	{
		data->start_simulation = ft_get_time_in_ms();
		// take  forka
		if (ft_should_stop(data,philo))
			break ;
		ft_lock_forks(data, philo, LOCK);
		if (ft_should_stop(data,philo))
			break ;
		// eat
		ft_eat(data, philo);
		if (ft_should_stop(data,philo))
			break ;
		// release forks
		ft_lock_forks(data, philo, UNLOCK);
		if (ft_should_stop(data,philo))
			break ;
		// think
		ft_think(data, philo);
		if (ft_should_stop(data,philo))
			break ;
		// sleep
		ft_sleep(data, philo);
	}
}

/**
 * Principal function to monitor if any philo has
 * died by hungry
 */
void	ft_monitor_process(t_data *data)
{
	int		i;
	long	time_since_last_meal;

	while (1)
	{
		i = 0;
		ft_handle_mutexes(data, &data->data_mtx, LOCK);
		if (data->end_simulation)
		{
			ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
			break ;
		}
		ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
		if (ft_check_philo_death(data, data->time_to_die))
			return ;
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

	philo = NULL;
	i = 0;
	while (i < data->philo_number)
	{
		philo = &data->philos[i];
		ft_handle_mutexes(data, &philo->data_mtx, LOCK);
		time_since_last_meal = ft_get_time_in_ms() - philo->time_last_meal;
		ft_handle_mutexes(data, &philo->data_mtx, UNLOCK);
		if (time_since_last_meal > time_to_die)
		{
			ft_handle_mutexes(data, &data->data_mtx, LOCK);
			data->end_simulation = 1;
			ft_handle_mutexes(data, &data->print_mtx, LOCK);
			printf("%d, philo %d died", ft_get_time_in_ms(), philo->id);
			ft_handle_mutexes(data, &data->print_mtx, UNLOCK);
			ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
			return (1);
		}
		i++;
	}
	return (0);
}
