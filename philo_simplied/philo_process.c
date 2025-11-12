#include "philo.h"

int ft_routine(pthread_mutex_t *f1_mtx,pthread_mutex_t *f2_mtx,t_philo *philo, t_data *data)
{
	if (ft_should_stop(data, philo)) 
		return(1);
    ft_handle_mutexes(data, f1_mtx, LOCK);
    ft_print_action(data, philo, TAKING); // Log first fork
    if (ft_should_stop(data, philo)) 
	{
         ft_handle_mutexes(data, f1_mtx, UNLOCK); // MUST Release F1!
        return(1);
    }
    ft_handle_mutexes(data, f2_mtx, LOCK);
    ft_print_action(data, philo, TAKING); // Log second fork
    ft_eat(data, philo); 
    ft_handle_mutexes(data, f1_mtx, UNLOCK); // Release F1
    ft_handle_mutexes(data, f2_mtx, UNLOCK); // Release F2
    if (ft_should_stop(data, philo))
		return(1);
    ft_think(data, philo);     
    if (ft_should_stop(data, philo))
		return(1);
    ft_sleep(data, philo); 
	return (0);
}

/*
The Deadlock Analogy: The issue isn't that the monitor's exit causes the hang;
 the hang happens because a philosopher thread gets stuck in a blocking 
 state (like waiting for a mutex) and never reaches the 
 exit check (ft_should_stop). The main thread then waits forever for that 
 stuck philosopher to join.
 */
// function hadle the cycle of each philo
void	ft_philo_cycle(t_philo *philo, t_data *data)
{
	pthread_mutex_t *f1_mtx;
    pthread_mutex_t *f2_mtx;
    
    if (philo->id % 2 == 0) // Even: Left then Right
    {
        f1_mtx = &philo->left_fork->mutex;
        f2_mtx = &philo->right_fork->mutex;
    }
    else // Odd: Right then Left
    {
        f1_mtx = &philo->right_fork->mutex;
        f2_mtx = &philo->left_fork->mutex;
    }
	// if (data->philo_number == 1)
	// 	ft_handle_one_philo()
    if (philo->id % 2 == 0)
        usleep((data->time_to_eat / 2) * 1000);
    while (1)
    {
        if(ft_routine(f1_mtx,f2_mtx,philo,data))
			break;
    }
	
}

/**
 * Principal function to monitor if any philo has
 * died by hungry
 */
void	ft_monitor_process(t_data *data)
{
	printf("out of the while (before start) in ft_monitor_process\n");
	while (1)
	{
		//printf("in the while of ft_monitor_process\n");	
		ft_handle_mutexes(data, &data->data_mtx, LOCK);
		if (data->end_simulation == 1)
		{
			ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
			printf("end_simuation has: %ld\n",data->end_simulation);	
			break;
		}
		ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
		if(data->number_of_meals!=-1 && ft_check_all_full(data))
		{
			ft_handle_mutexes(data, &data->data_mtx, LOCK);
			data->end_simulation = 1;
			ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
			break;
		}
		if (ft_check_philo_death(data, data->time_to_die))
		{
			//TODO: here is a deadlock!!
			printf("the ft_check_philo_death return 1\n");
			break;
		}			
		usleep(500);
	}
	printf("out of the while in ft_monitor_process\n");
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
			printf("\033[41m%ld, philo %d died\033[0m\n", ft_get_time_in_ms(), philo->id);
			ft_handle_mutexes(data, &data->print_mtx, UNLOCK);
			ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
			return (1);
		}
		i++;
	}
	return (0);
}
