#include "philo.h"

// function hadle the cycle of each philo
void	ft_philo_cycle(t_philo *philo, t_data *data)
{
	long	current;

	current = ft_get_time_in_ms();
	long elapsed = current - data->start_simulation;
		// to have the time when a philo do the action
	// validate if any philo has died, if it so exit
	// 1)start thinking
	// validate if any philo has died, if it so exit
	// 2)pickup the forks if they are avaiable
	// validate if any philo has died, if it so exit
	// 3)start eating
	// 4)releasae forks and start to sleep
	// 5)repeate all the process
}


void    ft_monitor_process(t_data *data)
{
    int i;
    long    current_time;

    while(1)
    {
        i = 0;
        if(data->end_simulation)
            break;
        while(i < data->philo_number)
        {
            current_time = ft_get_time_in_ms();
            if((current_time - data->philos[i].time_last_meal)>data->time_to_die)
            {
                data->end_simulation = 1;
                break;
            }
            i++;
        }
    }
}