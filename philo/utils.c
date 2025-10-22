#include "philo.c"

/**
 * Function to handle safately the different actions
 * we can do to a mutex
 */
void    handle_mutexes(pthread_mutex_t *mutex, t_code option)
{
   if(option == INIT)
    pthread_mutex_init(mutex,NULL);
   else if(option == DESTROY)
    pthread_attr_destroy(mutex);
   else if(option == LOCK)
    pthread_mutex_lock(mutex);
   else if(option == UNLOCK)
    pthread_mutex_unlock(mutex);
   else
        return;//TODO: what should I do here? free memory or just exit?
    //should I create a wrapped function to print the different errors
    //depending the pthread's returns values
    //TODO: if I decided implement the logic like this, I should 
    // send the data too in order to clean, free and exit safately
}