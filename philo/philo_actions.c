#include "philo.h"

/**
 * Function to lock and unlock the forks each
 * time a philo will eat ot finish of eat.
 * This was designed in order to avoid deadlock.
 */
void	ft_lock_unlock_forks(t_data *data, t_philo *philo, t_code code)
{
	if (philo->id % 2 == 0)
	{
		ft_handle_mutexes(data, philo->left_fork, code);
		ft_handle_mutexes(data, philo->right_fork, code);
	}
	else
	{
		ft_handle_mutexes(data, philo->right_fork, code);
		ft_handle_mutexes(data, philo->left_fork, code);
	}
}
/**
 * Function to handle the action of eating for each
 * philo
 */
void	ft_eat(t_data *data, t_philo *philo)
{
    ft_handle_mutexes(data,&philo->data_mtx,LOCK);
    philo->time_last_meal = ft_get_time_in_ms();
	philo->meals_counter++;
	ft_print_action(data, philo, EATING);

	if (data->number_of_meals != -1)
	{
		if ( philo->meals_counter == data->number_of_meals)
			philo->is_full = 1;
	}
    ft_handle_mutexes(data,&philo->data_mtx,UNLOCK);
    usleep(data->time_to_eat);
}

/**
 * Function to handle the case when a philo is
 * thinking, that basically is just 
 * set the thread to sleep
 */
void    ft_think(t_data *data, t_philo *philo)
{
    ft_print_action(data,philo,THINKING);
}

/**
 * Function to handle the case when a philo is
 * sleeping, that basically is just 
 * set the thread to sleep
 */
void    ft_sleep(t_data *data, t_philo *philo)
{
    ft_print_action(data,philo,THINKING);
    usleep(data->time_to_sleep);
}