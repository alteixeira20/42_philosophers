/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:13:17 by paalexan          #+#    #+#             */
/*   Updated: 2025/07/24 17:42:59 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	destroy_simulation(t_simulation *sim)
{
	int	i;

	if (sim->forks)
	{
		i = 0;
		while (i < sim->num_philosophers)
		{
			pthread_mutex_destroy(&sim->forks[i]);
			i++;
		}
		free(sim->forks);
	}
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->eating_lock);
	if (sim->philosophers)
	{
		i = 0;
		while (i < sim->num_philosophers)
		{
			pthread_mutex_destroy(&sim->philosophers[i].meal_mutex);
			i++;
		}
		free(sim->philosophers);
	}
}

void	destroy_forks_on_failure(pthread_mutex_t *forks, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(forks);
}

void	destroy_philosophers_on_failure(t_philosopher *philosophers, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&philosophers[i].meal_mutex);
		i++;
	}
	free(philosophers);
}
