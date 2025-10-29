#include "philo.h"

// function hadle the cycle of each philo
void	ft_philo_cycle(t_philo *philo, t_data *data)
{
	long	current;
	long	elapsed;

	current = ft_get_time_in_ms();
	elapsed = current - data->start_simulation;
	// to have the time when a philo do the action
	// validate if any philo has died, if it so exit
	// 1)start thinking
	// validate if any philo has died, if it so exit
	// 2)pickup the forks if they are avaiable
	// validate if any philo has died, if it so exit
	// 3)start eating
	// 4)releasae forks and start to sleep
	// 5)repeate all the process
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
/**
 * Function to print the action that each philo does
 */
void    ft_print_action(t_data *data,t_philo *philo, t_code code)
{
        ft_handle_mutexes(data, &data->print_mtx, LOCK);
		if(code == THINKING)
            printf("%d, philo %d is thinking", ft_get_time_in_ms(), philo->id);
        else if(code == EATING)
            printf("%d, philo %d is eating", ft_get_time_in_ms(), philo->id);
        else if(code == SLEEPING)
            printf("%d, philo %d is sleeping", ft_get_time_in_ms(), philo->id);
        else
            printf("%d, philo %d has taken a fork", ft_get_time_in_ms(), philo->id);
        ft_handle_mutexes(data, &data->print_mtx, UNLOCK);
}