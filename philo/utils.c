#include "philo.c"

/**
 * Function to handle safately the different actions
 * we can do to a mutex
 */
int	ft_handle_mutexes(t_data *data,pthread_mutex_t *mutex, t_code action)
{
	int	result;

	result = 0;
	if (action == INIT)
		result = pthread_mutex_init(mutex, NULL);
	else if (action == DESTROY)
		result = pthread_attr_destroy(mutex);
	else if (action == LOCK)
		result = pthread_mutex_lock(mutex);
	else if (action == UNLOCK)
		result = pthread_mutex_unlock(mutex);
	if (result != 0)
	{
		pthread_mutex_lock(&data->data_mtx);
        data->end_simulation = 1;
        ft_handle_mutex_error(action, result);
		pthread_mutex_unlock(&data->data_mtx);

	}
	return (result);
}

/**
 * Function to handle safately the different actions
 * we can do to a thread
 */
int	ft_handle_thread(pthread_t *thread, void *(*routine)(void *), void *arg,
		t_code action, t_data *data)
{
	int	result;

	result = 0;
	if (action == CREATE)
		result = pthread_create(thread, NULL, routine, arg);
	else if (action == JOIN)
		result = pthread_join(*thread, NULL);
	else if (action == DETEACH)
		result = pthread_detach(*thread);
	if (result != 0)
	{
		pthread_mutex_lock(&data->data_mtx);
        data->end_simulation = 1;
		ft_handle_thread_error(action, result);
		pthread_mutex_unlock(&data->data_mtx);
	}
	return (result);
}

/**
 * Function to get the current time
 * of the system
 * TODO: is this correct??
 */
long	ft_get_time_in_ms(void)
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
	if(data->end_simulation = 1)
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
