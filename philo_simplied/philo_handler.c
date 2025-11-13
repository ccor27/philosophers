#include "philo.h"

/**
 * Function to handle the errors of mutexes
 */
void	ft_handle_mutex_error(t_code action, int result_code)
{
	const char	*action_str;
	const char	*reason_str;

	reason_str = "Unknown error";
	if (action == INIT)
		action_str = "init";
	else if (action == DESTROY)
		action_str = "destroy";
	else if (action == LOCK)
		action_str = "lock";
	else if (action == UNLOCK)
		action_str = "unlock";
	else
		action_str = "unknown";
	if (result_code == 22)
		reason_str = "invalid mutex or attributes";
	else if (result_code == 16)
		reason_str = "mutex is currently locked (cannot destroy)";
	else if (result_code == 1)
		reason_str = "mutex not owned by this thread (cannot unlock)";
	else if (result_code == 35)
		reason_str = "deadlock detected (mutex already locked by this thread)";
	else if (result_code == 12)
		reason_str = "not enough memory to initialize mutex";
	printf("Mutex error [%s]: %s", action_str, reason_str);
}

/**
 * Function to handle the errors of threads (pthreads)
 */
void	ft_handle_thread_error(t_code action, int result_code)
{
	const char	*action_str;
	const char	*reason_str;

	reason_str = "Unknown error";
	if (action == CREATE)
		action_str = "creating";
	else if (action == JOIN)
		action_str = "joining";
	else if (action == DETEACH)
		action_str = "detaching";
	else
		action_str = "unknown";
	if (result_code == 11)
		reason_str = "Not enough system resources to create thread";
	else if (result_code == 22)
		reason_str = "Invalid attributes or operation not allowed";
	else if (result_code == 1)
		reason_str = "No permission to set scheduling policy";
	else if (result_code == 35)
		reason_str = "Deadlock detected (joining self)";
	else if (result_code == 3)
		reason_str = "Thread does not exist";
	printf("Thread error [%s]: %s\n", action_str, reason_str);
}

/**
 * Function to handle safately the different actions
 * we can do to a mutex
 */
int	ft_handle_mutexes(t_data *data, pthread_mutex_t *mutex, t_code action)
{
	int	result;

	result = 0;
	if (action == INIT)
		result = pthread_mutex_init(mutex, NULL);
	else if (action == DESTROY)
		result = pthread_mutex_destroy(mutex);
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
		t_code action)
{
	int	result;

	result = 0;
	if (action == CREATE)
		result = pthread_create(thread, NULL, routine, arg);
	else if (action == JOIN)
		result = pthread_join(*thread, NULL);
	else if (action == DETEACH)
		result = pthread_detach(*thread);
	return (result);
}

/**
 * Function that will check if all the philosophers
 * have eaten all their meals
 */
int	ft_check_all_full(t_data *data)
{
	int i;
	int all_full;
	t_philo *philo;

	if (data->number_of_meals == -1)
		return (0);
	i = 0;
	all_full = 1;
	while (i < data->philo_number)
	{
		philo = &data->philos[i];
		ft_handle_mutexes(data, &philo->data_mtx, LOCK);
		if (!philo->is_full)
			all_full = 0;
		ft_handle_mutexes(data, &philo->data_mtx, UNLOCK);
		if (!all_full)
			return (0);
		i++;
	}
	return (1);
}