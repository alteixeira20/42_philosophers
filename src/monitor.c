/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:45:15 by paalexan          #+#    #+#             */
/*   Updated: 2025/07/24 17:48:13 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static int	check_all_ate(t_simulation *sim)
{
	int	i;
	int	full;

	if (sim->num_must_eat <= 0)
		return (SUCCESS);
	full = 0;
	i = 0;
	while (i < sim->num_philosophers)
	{
		if (sim->philosophers[i].meals_eaten >= sim->num_must_eat)
			full++;
		i++;
	}
	if (full == sim->num_philosophers)
		return (FAILURE);
	return (SUCCESS);
}

static void	kill_philo(t_simulation *sim, int i, long now)
{
	pthread_mutex_unlock(&sim->philosophers[i].meal_mutex);
	pthread_mutex_lock(&sim->print_mutex);
	printf("%ld %d died\n",
		now - sim->start_timestamp,
		sim->philosophers[i].id);
	pthread_mutex_unlock(&sim->print_mutex);
	sim->simulation_finished = 1;
}

static int	check_death(t_simulation *sim)
{
	int		i;
	long	last;
	long	diff;
	long	now;

	i = 0;
	while (i < sim->num_philosophers)
	{
		pthread_mutex_lock(&sim->philosophers[i].meal_mutex);
		last = sim->philosophers[i].last_meal;
		now = current_timestamp_ms();
		diff = now - last;
		if (diff > sim->time_to_die)
		{
			kill_philo(sim, i, now);
			return (FAILURE);
		}
		pthread_mutex_unlock(&sim->philosophers[i].meal_mutex);
		i++;
	}
	return (SUCCESS);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	while (!sim->simulation_finished)
	{
		if (check_death(sim) == FAILURE)
			return (NULL);
		if (check_all_ate(sim) == FAILURE)
		{
			sim->simulation_finished = 1;
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
