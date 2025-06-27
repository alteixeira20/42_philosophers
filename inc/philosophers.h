/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:43:14 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/27 13:03:19 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ******************************************************** ****************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

/* ************************************************************************** */
/*                                                                            */
/*                            Structs Declaration                             */
/*                                                                            */
/* ************************************************************************** */

struct s_simulation;

typedef struct s_philosopher
{
	int					id;
	long				last_meal;
	int					meals_eaten;

	pthread_t			thread;
	struct s_simulation *sim;
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
	t_philosopher	*philosophers;
}	t_simulation;

/* ************************************************************************** */
/*                                                                            */
/*                               Initialization                               */
/*                                                                            */
/* ************************************************************************** */

int		init_simulation(t_simulation *sim, int argc, char **argv);
void	destroy_simulation(t_simulation *sim);

/* ************************************************************************** */
/*                                                                            */
/*                             Philosopher Logic                              */
/*                                                                            */
/* ************************************************************************** */

void	*philosopher_routine(void *arg);

/* ************************************************************************** */
/*                                                                            */
/*                                   Utils                                    */
/*                                                                            */
/* ************************************************************************** */

long	current_timestamp_ms(void);
void	precise_sleep(long time_in_ms);
void	print_state(t_philosopher *philo, const char *msg);

#endif
