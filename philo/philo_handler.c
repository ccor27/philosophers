#include "philo.h"

/**
 * Function to handle the errors that could occur in mutexes
 * in any case, here we just print the error
 * specifing the case.
 * The function that call this one should  handle 
 * the unlock, free and exit
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
 * Function to handle the errors that could occur in threads
 * in any case, here we just print the error
 * specifing the case.
 * The function that call this one should  handle 
 * the unlock, free and exit
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
 * Function that handle the actions of the mutexes
 * basically if any action fails, we set the end simulation to 1
 * call the ft_handle_mutex_error, the function that call this one
 * should call the free_and_exit
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
 * Function that handle the actions of the threads
 * basically here we just return the value of the action
 * the function that call this one should handle the unlock 
 * and free_and_exit	
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
	return (result);
}
