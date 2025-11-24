#include "philo.h"

/**
 * Function to handle the philos rotuine
 * Basically here we do:
 * 	- validata before each action if the program should stop
 * 		(in case any philo has died or all are full)
 * 	- do the cycle eat->sleep->think
 * 	- lock/unlock the forks
 * This function will be called into a while from other 
 * funtion.
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
	ft_sleep(data, philo);
	if (ft_should_stop(data, philo))
		return (1);
	ft_think(data, philo);
	return (0);
}

/**
 * Function to hanlde the case we get just one philo
 * Here we just grab the first fork, print and sleep
 * the time of died and realese the fork
 */
void	ft_routine_for_one(pthread_mutex_t *f1_mtx, t_philo *philo,
		t_data *data)
{
	ft_handle_mutexes(data,f1_mtx,LOCK);
	ft_print_action(data,philo,TAKING);
	usleep(data->time_to_die * 1000);
	ft_handle_mutexes(data,f1_mtx,UNLOCK);
}
/**
 * Function that will handle the cycle of a philo
 * Here we:
 * 		- decided the philo's fork depending in its
 * 			id
 * 		- validate if we have just 1 or more philos
 * 		- no let all philos start in order to avoid
 * 			a deadlock by the forks
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
	if(data->philo_number == 1)
	{
		ft_routine_for_one(f1_mtx, philo, data);
		return ;
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
 * Function that handle the behaviour of the moinitor
 * Basically here:
 * 		- we constantly validate if any philo has died
 * 		calling the auxiliar function and validating the 
 * 		variable from data (end_simulation)
 * 		- sleep the process to give time the philos
 * 			to do their actions (a little time)
 */
void	ft_monitor_process(t_data *data)
{

	while (1)
	{
		ft_handle_mutexes(data, &data->data_mtx, LOCK);
		if (data->end_simulation == 1)
		{
			ft_handle_mutexes(data, &data->data_mtx, UNLOCK);
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
		usleep(200);
	}
}

/**
 * Auxiliary function to validate if a philo has died by starvation
 * Here basically:
 * 		- loop the philo array and for any of them
 * 			calculate the time since last meal in order to know
 * 			if it is grather than the time to die
 * 		- change the end_simulation variable to 1
 * 		- lock/unlock mutexes
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
