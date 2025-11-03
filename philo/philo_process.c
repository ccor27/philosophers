#include "philo.h"

// function hadle the cycle of each philo
void	ft_philo_cycle(t_philo *philo, t_data *data)
{
	while (1)
	{
		if (philo->id % 2 != 0)
    		usleep(100);
		//printf("first line in the while in ft_philo_cycle\n");
	//	ft_handle_mutexes(data,&philo->data_mtx,LOCK);
	//	data->start_simulation = ft_get_time_in_ms();
	//	ft_handle_mutexes(data,&philo->data_mtx,UNLOCK);
		// take  forka
		if (ft_should_stop(data,philo))
		{
			printf("in the if of ft_should_stop before take forks\n");
			break ;
		}
		ft_lock_unlock_forks(data, philo, LOCK);
		if (ft_should_stop(data,philo))
		{
			printf("in the if of ft_should_stop after take forks\n");
			break ;
		}
		// eat
		ft_eat(data, philo);
		if (ft_should_stop(data,philo))
		{
			printf("in the if of ft_should_stop after eat\n");
			break ;
		}
		// release forks
		ft_lock_unlock_forks(data, philo, UNLOCK);
		if (ft_should_stop(data,philo))
			{
			printf("in the if of ft_should_stop before think\n");
			break ;
		}
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
}

/**
 * Principal function to monitor if any philo has
 * died by hungry
 */
void	*ft_monitor_process(void *arg)
{
	t_data *data;

	data = (t_data *)arg;
	while (1)
	{
		//printf("in the while of ft_monitor_process\n");	
		ft_handle_mutexes(data, &data->data_mtx, LOCK);
		if (data->end_simulation != -1)
		{
			ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
			printf("end_simuation has: %ld\n",data->end_simulation);	
			break;
		}
		ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
		if (ft_check_philo_death(data, data->time_to_die))
			return(NULL);
		usleep(1000);
	}
	return(NULL);
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
	//printf("in the ft_check_philo_death\n");
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
			printf("\033[41m%ld, philo %d died[0m\n", ft_get_time_in_ms(), philo->id);
			ft_handle_mutexes(data, &data->print_mtx, UNLOCK);
			ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
			return (1);
		}
		i++;
	}
	return (0);
}
