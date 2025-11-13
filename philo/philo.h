#ifndef PHILO_H
#define PHILO_H

#include <limits.h>
#include <pthread.h>  // All functions of pthread_*
#include <stdio.h>	  //printf
#include <stdlib.h>	  //malloc, free
#include <string.h>	  //memset
#include <sys/time.h> //gettimeofday
#include <unistd.h>	  //write, usleep

//================== enum and structs =====================
typedef struct s_philo t_philo;
// enum  to handle the operations of a pthread
typedef enum e_code
{
	// pthread options
	CREATE,
	JOIN,
	DETEACH,
	INIT,
	// mutex options
	DESTROY,
	LOCK,
	UNLOCK,
	//actions
	EATING,
	THINKING,
	TAKING,
	SLEEPING,
	//to know which fork take
	RIGHT,
	LEFT

} t_code;

// struct to represent a fork
typedef struct s_fork
{
	int fork_id;
	pthread_mutex_t mutex;
} t_fork;

// struct where we'll store the global data
typedef struct s_data
{
	long philo_number; // number of philos
	long time_to_die;
	long time_to_eat;
	long time_to_sleep;
	long number_of_meals;	   // this is optional, so if this is
							   //-1 that meas that there is not limit of meals
	long start_simulation;	   // to know the time the program starts
	long end_simulation;	   // to know when a philo dies or all are full, initially -1
	pthread_mutex_t data_mtx; //mutex to avoid race condition in data
	pthread_mutex_t print_mtx; //mutex to manage the actions prints
	pthread_t monitor;
	// TODO: create a variable to the monitor, it should be a pthread
	t_fork *forks;	 // array of the forks we have
	t_philo *philos; // array of the philos we have
} t_data;

// struct to represent a philo
struct s_philo
{
	int id;
	long meals_counter;
	int is_full;
	long time_last_meal;	  // to determine if a philo died between meals
	pthread_mutex_t data_mtx; // mutex to control race condition
	t_fork *left_fork;		  // left fork that the philo could use
	t_fork *right_fork;		  // right fork that the philo could use
	pthread_t thread_id;	  // beacuse the philo is a thread
	t_data *data;			  // Pointer back to the global data struct

};

//================== functions prototipes =====================
// parsing.c
void ft_parse_and_store(int argc, t_data *data, char **argv);
void ft_init_data(t_data *data);
void ft_init_philos(t_data *data);
void	*ft_philo_starter(void *arg);
void	ft_join_threads(t_data *data);
void	*ft_monitor_starter(void *arg);
// parsing_utils.c
int ft_isspace(char c);
int ft_isdigit(char c);
const char *ft_skip_no_digits(const char *s);
long ft_atol(const char *s);
// clean_up
void ft_error_exit(const char *msg);
void ft_free_and_exit(const char *msg, t_data *data);
void ft_free_forks(t_data *data);
void ft_free_philos(t_data *data);
// utils.c
int ft_handle_mutexes(t_data *data, pthread_mutex_t *mutex,
					  t_code action);
int	ft_handle_thread(pthread_t *thread, void *(*routine)(void *), void *arg,
		t_code action, t_data *data);
long ft_get_time(void);
void    ft_print_action(t_data *data,t_philo *philo, t_code code);
int		ft_should_stop(t_data *data, t_philo *philo);
// error_handler
void ft_handle_mutex_error(t_code action, int result_code);
void ft_handle_thread_error(t_code action, int result_code);
//philo_actions.c
void	ft_eat(t_data *data, t_philo *philo);
//void	ft_lock_unlock_forks(t_data *data, t_philo *philo, t_code code);
int	ft_fork_action(t_data *data, t_philo * philo, t_code code);
void    ft_think(t_data *data, t_philo *philo);
void    ft_sleep(t_data *data, t_philo *philo);
//philo_process
void	ft_philo_cycle(t_philo *philo, t_data *data);
void	ft_monitor_process(t_data *data);
int	ft_check_philo_death(t_data *data, long time_to_die);
#endif
