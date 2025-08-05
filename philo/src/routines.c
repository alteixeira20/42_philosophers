/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:36:48 by paalexan          #+#    #+#             */
/*   Updated: 2025/08/05 15:13:06 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static void	helper_eat(t_philosopher *philo, t_simulation *sim,
			int left, int right)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = current_timestamp_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_state(philo, "is eating");
	precise_sleep(sim, sim->time_to_eat);
	pthread_mutex_unlock(&sim->forks[left]);
	pthread_mutex_unlock(&sim->forks[right]);
}

static void	eat(t_philosopher *philo)
{
	t_simulation	*sim;
	int				left;
	int				right;

	sim = philo->sim;
	left = philo->id - 1;
	right = philo->id % sim->num_philosophers;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&sim->forks[right]);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&sim->forks[left]);
		print_state(philo, "has taken a fork");
	}
	else
	{
		usleep(500);
		pthread_mutex_lock(&sim->forks[left]);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&sim->forks[right]);
		print_state(philo, "has taken a fork");
	}
	helper_eat(philo, sim, left, right);
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
	while (!is_simulation_finished(sim))
	{
		eat(philo);
		if (is_simulation_finished(sim))
			break ;
		sleep_and_think(philo);
	}
	return (NULL);
}
