#ifndef PHILO_H
#define PHILO_H

#include <string.h> //memset
#include <stdio.h> //printf
#include <stdlib.h> //malloc, free
#include <unistd.h> //write, usleep
#include <sys/time.h> //gettimeofday
#include <pthread.h> // All functions of pthread_*

//struct to represent a fork
typedef struct s_fork
{
	int fork_id;
	pthread_mutex_t t_mutex;
}	t_fork;

//struct to represent a philo
typedef struct s_philo
{
	int id;
	long meals_counter;
	int is_full;
	long time_last_meal; //to determine if a philo died between meals
	t_fork *left_fork; // left fork that the philo could use
	t_fork *right_fork;// right fork that the philo could use
	pthread_t thread_id;// beacuse the philo is a thread

}	t_philo;

//struct where we'll store the global data
typedef struct s_data
{
	long philo_number;//number of philos
	long time_to_die;
	long time_to_eat;
	long time_to_sleep;
	long number_of_meals; //this is optional, so if this is -1 that meas that there is not limit of meals
	long start_simulation;//to know the time the program starts
	long end_simulation;//to know when a philo dies or all are full
	t_fork *forks; //array of the forks we have
	t_philo *philos; //array of the philos we have
}

//functions
void error_exit(const char *msg);
//parsing.c
void ft_parse_and_store(t_data *data, char **argv);

//parsing_utils.c
int	ft_isspace(char c);
int	ft_isdigit(char c);
const char	*ft_skip_no_digits(char *s);
long	ft_atol(const char *s);



#endif
