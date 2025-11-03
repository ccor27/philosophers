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
	printf("Mutex error [%u]: %s", action, reason_str);
}

/**
 * Function to handle the errors of threads (pthreads)
 */
void	ft_handle_thread_error(t_code action, int result_code)
{
	const char *action_str;
	const char *reason_str;

	reason_str = "Unknown error";

	if (action == CREATE)
		action_str = "create";
	else if (action == JOIN)
		action_str = "join";
	else if (action == DETEACH)
		action_str = "detach";
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
