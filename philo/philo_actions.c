#include "philo.h"

/**
 * Function to handle the action where a philo eat
 * What we do here is:
 * 	- update las time meal
 * 	- if there is number of meal, update the 
 * 		philo's counter and validate if it is full
 * 	- print the action
 * 	- put the philo to sleep
 */
void	ft_eat(t_data *data, t_philo *philo)
{
	ft_handle_mutexes(data, &philo->data_mtx, LOCK);
	philo->time_last_meal = ft_get_time();
	if (data->number_of_meals != -1)
	{
		philo->meals_counter++;
		if (philo->meals_counter >= data->number_of_meals)
			philo->is_full = 1;
	}
	ft_print_action(data, philo, EATING);
	ft_handle_mutexes(data, &philo->data_mtx, UNLOCK);
	usleep(data->time_to_eat * 1000);
}

/**
 * Function to handle the action of think
 * What we do here is:
 * 	- print the action
 * 	- put the philo to sleep, why?
 * 		because if we do not put the philo to
 * 		sleep, we can reach at deadlock, because,
 * 		in the ft_philo_cycle we just let start
 * 		the philos that are even, that in order to
 * 		avoid deadlock if all take the forks. So if
 * 		we do not put a time to sleep, after 2-3 cycles
 * 		the philos we'll synchronized and two neighbors 
 * 		we'll take	forks and we'll died waiting for the
 * 		fork that the other had, so adding a little
 * 		time to sleep, we can avoid that
 */
void	ft_think(t_data *data, t_philo *philo)
{
	ft_print_action(data, philo, THINKING);
	usleep(200);
}

/**
 * Function to handle the action of sleeping,
 * Basically here we print the action and
 * put the philo to sleep
 */
void	ft_sleep(t_data *data, t_philo *philo)
{
	ft_print_action(data, philo, SLEEPING);
	usleep(data->time_to_sleep * 1000);
}
