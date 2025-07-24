/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:36:48 by paalexan          #+#    #+#             */
/*   Updated: 2025/07/24 17:42:47 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static void	get_fork_indices(t_philosopher *philo, int *first, int *second)
{
	t_simulation	*sim;
	int				left;
	int				right;

	sim = philo->sim;
	left = philo->id - 1;
	right = philo->id % sim->num_philosophers;
	if (philo->id % 2 == 0)
	{
		*first = right;
		*second = left;
	}
	else
	{
		*first = left;
		*second = right;
	}
}

static void	eat(t_philosopher *philo)
{
	t_simulation	*sim;

	sim = philo->sim;
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = current_timestamp_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	print_state(philo, "is eating");
	philo->meals_eaten++;
	precise_sleep(sim, sim->time_to_eat);
}

static void	take_forks_and_eat(t_philosopher *philo)
{
	t_simulation	*sim;
	int				first;
	int				second;

	sim = philo->sim;
	get_fork_indices(philo, &first, &second);
	pthread_mutex_lock(&sim->forks[first]);
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(&sim->forks[second]);
	print_state(philo, "has taken a fork");
	eat(philo);
	pthread_mutex_unlock(&sim->forks[second]);
	pthread_mutex_unlock(&sim->forks[first]);
}

static void	sleep_and_think(t_philosopher *philo)
{
	t_simulation	*sim;

	sim = philo->sim;
	print_state(philo, "is sleeping");
	precise_sleep(sim, sim->time_to_sleep);
	print_state(philo, "is thinking");
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;
	t_simulation	*sim;

	philo = (t_philosopher *)arg;
	sim = philo->sim;
	if (sim->num_philosophers == 1)
	{
		pthread_mutex_lock(&sim->forks[0]);
		print_state(philo, "has taken a fork");
		precise_sleep(sim, sim->time_to_die);
		pthread_mutex_unlock(&sim->forks[0]);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!sim->simulation_finished)
	{
		take_forks_and_eat(philo);
		if (sim->simulation_finished)
			break ;
		sleep_and_think(philo);
	}
	return (NULL);
}
