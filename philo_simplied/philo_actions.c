#include "philo.h"

/**
 * Function to handle the action of eating for each
 * philo
 */
void	ft_eat(t_data *data, t_philo *philo)
{
    ft_handle_mutexes(data,&philo->data_mtx,LOCK);
    philo->time_last_meal = ft_get_time_in_ms();
	philo->meals_counter++;
	if (data->number_of_meals != -1)
	{
		if ( philo->meals_counter >= data->number_of_meals)
			philo->is_full = 1;
	}
	ft_print_action(data, philo, EATING);
    ft_handle_mutexes(data,&philo->data_mtx,UNLOCK);
    usleep(data->time_to_eat * 1000);
}

/**
 * Function to handle the case when a philo is
 * thinking, that basically is just 
 * set the thread to sleep
 */
void    ft_think(t_data *data, t_philo *philo)
{
    ft_print_action(data,philo,THINKING);
	usleep(200);
}

/**
 * Function to handle the case when a philo is
 * sleeping, that basically is just 
 * set the thread to sleep
 */
void    ft_sleep(t_data *data, t_philo *philo)
{
    ft_print_action(data,philo,SLEEPING);
    usleep(data->time_to_sleep * 1000);
}

int ft_check_all_full(t_data *data)
{
    int i;
    int all_full;
    t_philo *philo;

    if (data->number_of_meals == -1)
        return (0); // Meal limit disabled
    
    i = 0;
    all_full = 1;
    while (i < data->philo_number)
    {
        philo = &data->philos[i];
        
        // Safely read philo->is_full
        ft_handle_mutexes(data, &philo->data_mtx, LOCK);
        if (philo->is_full == 0)
            all_full = 0; // Found one not full, stop checking
        ft_handle_mutexes(data, &philo->data_mtx, UNLOCK);
        
        if (all_full == 0)
            return (0);
        i++;
    }
    return (1); // All philos are full!
}

// void ft_  (t_data *data, t_philo *philo, )
// {
// 	ft_handle_mutexes(data, f1_mtx, LOCK);
//     ft_print_action(data, philo, TAKING);
//     usleep(data->time_to_die * 1000); 
//     ft_handle_mutexes(data, f1_mtx, UNLOCK);
// }