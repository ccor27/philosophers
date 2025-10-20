#include "philo.h"

/**
 * Function to parse the arguments and store them
 * into data struct
 * ./philo 5 800 200 200 [5]
 */
void ft_parse_and_store(t_data *data, char **argv)
{
    //TODO: parse the numbers to miliseconds 
    data->philo_number = ft_atol(argv[1]);
    data->time_to_die = ft_atol(argv[2]);
    data->time_to_eat = ft_atol(argv[3]);
    data->time_to_sleep = ft_atol(argv[4]);
    if(argv[5])
        data->number_of_meals = ft_atol(argv[5]);
    else
        data->number_of_meals = -1;    
}

/**
 * Function to initialize the structs 
 * forks and philos
 */
void    ft_init_data(t_data *data)
{
    if(!data)
        ft_error_exit("Error: the data struct is null");
    //init fork
    ft_init_forks(data);
    //init philos 
    ft_init_philos(data);
}


/**
 * Function to allocate memory and initialize 
 * the forks
 */
void    ft_init_forks(t_data *data)
{
     int i;

    data->forks = malloc(sizeof(t_fork) * data->philo_number);
    if (!data->forks)
        ft_error_exit("Failed to allocate memory for forks");

    i = 0;
    while (i < data->philo_number)
    {
        data->forks[i].fork_id = (i + 1);
        if (pthread_mutex_init(&data->forks[i].mutex, NULL) != 0)
            ft_free_and_exit("Failed to initialize mutex",data);//TODO: create a function to free memory
        i++;
    }
}

/**
 * Function to initialize the philos
 */
void    ft_init_philos(t_data *data)
{
    int i;

    data->philos = malloc(sizeof(t_philo) * data->philo_number);
    if(!data->philos)
         ft_free_and_exit("Failed to allocate memory for forks",data);
    i = 0;
    while (i < data->philo_number)
    {
        data->philos[i].id = (i + 1);
        data->philos[i].meals_counter = 0;
        data->start_simulation = 0;
        data->end_simulation = 0;
        //TODO: create a function to assing the forks
    }
    
}
