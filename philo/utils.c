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
		ft_handle_mutex_error(action, result);
        data->end_simulation = 1;
        data->reason_simulation_end = 1;
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
		ft_handle_thread_error(action, result);
        data->end_simulation = 1;
        data->reason_simulation_end = 2;
	}
	return (result);
}

/**
 * Function to get the current time
 * of the system
 */
long	ft_get_time_in_ms(void)
{
	struct timeval	tv;
	long			time_in_ms;

	gettimeofday(&tv, NULL);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}
