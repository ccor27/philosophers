#include "philo.h"

/**
 * Wrapper function to start the threads correctly
 * and call the function that each one will use
 */
void	*ft_philo_starter(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	ft_philo_cycle(philo, data);
	return (NULL);
}

void	*ft_monitor_starter(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	ft_monitor_process(data);
	return (NULL);
}
/**
 * Function to get the current time
 * of the system
 * TODO: is this correct??
 */
long	ft_get_time(void)
{
	struct timeval	tv;
	long			time_in_ms;

	gettimeofday(&tv, NULL);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}
/**
 * Function to print the action that each philo does
 */
void    ft_print_action(t_data *data,t_philo *philo, t_code code)
{
	ft_handle_mutexes(data, &data->data_mtx, LOCK);
    if(data->end_simulation == 1)
    {
        ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
        return ;
    }
    ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
    ft_handle_mutexes(data, &data->print_mtx, LOCK);
	if(code == THINKING)
        printf("\033[32m%ld, philo %d is thinking\033[0m\n", ft_get_time(), philo->id);
    else if(code == EATING)
        printf("\033[33m%ld, philo %d is eating\033[0m\n", ft_get_time(), philo->id);
    else if(code == SLEEPING)
        printf("\033[34m%ld, philo %d is sleeping\033[0m\n", ft_get_time(), philo->id);
    else
        printf("\033[36m%ld, philo %d has taken a fork\033[0m\n", ft_get_time(), philo->id);
    ft_handle_mutexes(data, &data->print_mtx, UNLOCK);
}

/**
 * Function that will be used in the ft_philo_cycle
 * basically to validate in which moment each philo
 * need to stop their process.
 * If any of philos have died or a philo is full, 
 * the cycle should stop.
 */
int	ft_should_stop(t_data *data, t_philo *philo)
{
	int flag;

	flag = 0;
	ft_handle_mutexes(data,&data->data_mtx,LOCK);
	if(data->end_simulation == 1)
		flag = 1;
	ft_handle_mutexes(data,&data->data_mtx,UNLOCK);
	if(!flag)
	{
		ft_handle_mutexes(data,&philo->data_mtx,LOCK);
		if(philo->is_full)
			flag = 1;
		ft_handle_mutexes(data,&philo->data_mtx,UNLOCK);
	}
	return (flag);
}
