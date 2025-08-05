/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:58:27 by paalexan          #+#    #+#             */
/*   Updated: 2025/08/05 15:02:10 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>

/* ************************************************************************** */
/*                                                                            */
/*                                  Settings                                  */
/*                                                                            */
/* ************************************************************************** */

# define DEBUG_MODE 0

/* ************************************************************************** */
/*                                                                            */
/*                           Argument Descriptions                            */
/*                                                                            */
/* ************************************************************************** */

# define ARG_NUM_PHILOSOPHERS "num_philosophers"
# define ARG_TIME_TO_DIE      "time_to_die"
# define ARG_TIME_TO_EAT      "time_to_eat"
# define ARG_TIME_TO_SLEEP    "time_to_sleep"
# define ARG_NUM_MUST_EAT     "num_must_eat"

/* ************************************************************************** */
/*                                                                            */
/*                            Structs Declaration                             */
/*                                                                            */
/* ************************************************************************** */

struct	s_simulation;

typedef struct s_philosopher
{
	int					id;
	int					meals_eaten;

	long				last_meal;

	pthread_t			thread;
	pthread_mutex_t		meal_mutex;

	struct s_simulation	*sim;
}	t_philosopher;

typedef struct s_simulation
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_must_eat;
	int				simulation_finished;

	long			start_timestamp;

	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	eating_lock;
	pthread_mutex_t	finish_mutex;

	t_philosopher	*philosophers;
}	t_simulation;

typedef enum e_status
{
	ERROR	= -1,
	SUCCESS = 0,
	FAILURE = 1
}	t_status;

/* ************************************************************************** */
/*                                                                            */
/*                               Initialization                               */
/*                                                                            */
/* ************************************************************************** */

int			init_simulation(t_simulation *sim);
int			is_simulation_finished(t_simulation *sim);
void		set_simulation_finished(t_simulation *sim, int status);

/* ************************************************************************** */
/*                                                                            */
/*                                  Parsing                                   */
/*                                                                            */
/* ************************************************************************** */

int			parse_args(t_simulation *sim, int argc, char **argv);

/* ************************************************************************** */
/*                                                                            */
/*                             Thread Management                              */
/*                                                                            */
/* ************************************************************************** */

int			create_philosopher_threads(t_simulation *sim);
void		join_philosopher_threads(t_simulation *sim);

/* ************************************************************************** */
/*                                                                            */
/*                             Philosopher Logic                              */
/*                                                                            */
/* ************************************************************************** */

void		*philosopher_routine(void *arg);
void		*monitor_routine(void *arg);

/* ************************************************************************** */
/*                                                                            */
/*                                   Utils                                    */
/*                                                                            */
/* ************************************************************************** */

int			is_positive_integer(const char *str);
const char	*get_arg_description(int index);
long		current_timestamp_ms(void);
void		precise_sleep(t_simulation *sim, int duration_ms);

/* ************************************************************************** */
/*                                                                            */
/*                              Debug & Messages                              */
/*                                                                            */
/* ************************************************************************** */

void		print_state(t_philosopher *philo, const char *msg);
void		debug_print(const char *msg);
void		debug_simulation(t_simulation *sim);

/* ************************************************************************** */
/*                                                                            */
/*                                  Cleanup                                   */
/*                                                                            */
/* ************************************************************************** */

void		destroy_simulation(t_simulation *sim);
void		destroy_forks_on_failure(pthread_mutex_t *forks, int count);
void		destroy_philosophers_on_failure(t_philosopher *philosophers,
				int count);

#endif
