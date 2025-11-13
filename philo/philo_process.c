#include "philo.h"

/*
The Deadlock Analogy: The issue isn't that the monitor's exit causes the hang;
 the hang happens because a philosopher thread gets stuck in a blocking
 state (like waiting for a mutex) and never reaches the
 exit check (ft_should_stop). The main thread then waits forever for that
 stuck philosopher to join.
 */
void	ft_philo_cycle(t_philo *philo, t_data *data)
{
	if (philo->id % 2 != 0)
    	usleep((data->time_to_eat / 2) * 1000);//the others wait half time to grab forks
	while (1)
	{
		//TODO: make the get fork process independently
		if (ft_should_stop(data,philo))
		{
			printf("in the if of ft_should_stop before take forks\n");
			break ;
		}
		//grab first fork
		if(ft_handle_mutexes(data, &philo->right_fork->mutex, LOCK))
			break;
		ft_print_action(data,philo,TAKING);
		//usleep(100);
		if (ft_should_stop(data,philo))
		{
			ft_handle_mutexes(data, &philo->right_fork->mutex, UNLOCK);
			printf("in the if of ft_should_stop after take forks\n");
			break ;
		}
		//grab second fork
		if(ft_handle_mutexes(data, &philo->left_fork->mutex, LOCK))
		{
			ft_handle_mutexes(data, &philo->right_fork->mutex, UNLOCK);
			//usleep(100);
			ft_handle_mutexes(data, &philo->left_fork->mutex, UNLOCK);
			break;
		}
		ft_print_action(data,philo,TAKING);
		if (ft_should_stop(data,philo))
		{
			ft_handle_mutexes(data, &philo->left_fork->mutex, UNLOCK);
			printf("in the if of ft_should_stop after take forks\n");
			break ;
		}
		//eat
		ft_eat(data, philo);
		//release fork
		if(ft_should_stop(data,philo))
			break;
		ft_handle_mutexes(data, &philo->right_fork->mutex, UNLOCK);
		if(ft_should_stop(data,philo))
			break;
		//usleep(100);
		ft_handle_mutexes(data, &philo->left_fork->mutex, UNLOCK);
		if(ft_should_stop(data,philo))
			break;
		// think
		ft_think(data, philo);
		if (ft_should_stop(data,philo))
				{
			printf("in the if of ft_should_stop after think\n");
			break ;
		}
		// sleep
		ft_sleep(data, philo);
	}
	printf("out of the while in ft_philo_cycle");
}
// function hadle the cycle of each philo
// void	ft_philo_cycle(t_philo *philo, t_data *data)
// {
// 	if (philo->id % 2 != 0)
//     	usleep((data->time_to_eat / 2) * 1000);//the others wait half time to grab forks
// 	while (1)
// 	{
// 		//TODO: make the get fork process independently
// 		if (ft_should_stop(data,philo))
// 		{
// 			printf("in the if of ft_should_stop before take forks\n");
// 			break ;
// 		}
// 		ft_lock_unlock_forks(data, philo, LOCK);
// 		if (ft_should_stop(data,philo))
// 		{
// 			ft_lock_unlock_forks(data, philo, UNLOCK);
// 			printf("in the if of ft_should_stop after take forks\n");
// 			break ;
// 		}
// 		// eat
// 		ft_eat(data, philo);
// 		if (ft_should_stop(data,philo))
// 		{
// 			printf("in the if of ft_should_stop after eat\n");
// 			break ;
// 		}
// 		// release forks
// 		ft_lock_unlock_forks(data, philo, UNLOCK);
// 		if (ft_should_stop(data,philo))
// 			{
// 			printf("in the if of ft_should_stop before think\n");
// 			break ;
// 		}
// 		// think
// 		ft_think(data, philo);
// 		if (ft_should_stop(data,philo))
// 				{
// 			printf("in the if of ft_should_stop after think\n");
// 			break ;
// 		}
// 		// sleep
// 		ft_sleep(data, philo);
// 	}
// 	printf("out of the while in ft_philo_cycle");
// }

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
		time_since_last_meal = ft_get_time() - philo->time_last_meal;
		ft_handle_mutexes(data, &philo->data_mtx, UNLOCK);
		if (time_since_last_meal > time_to_die)
		{
			ft_handle_mutexes(data, &data->data_mtx, LOCK);
			data->end_simulation = 1;
			ft_handle_mutexes(data, &data->print_mtx, LOCK);
			printf("\033[41m%ld, philo %d died\033[0m\n", ft_get_time(), philo->id);
			ft_handle_mutexes(data, &data->print_mtx, UNLOCK);
			ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
			return (1);
		}
		i++;
	}
	return (0);
}
