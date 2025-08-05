/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 20:47:44 by paalexan          #+#    #+#             */
/*   Updated: 2025/08/05 13:37:58 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	create_philosopher_threads(t_simulation *sim)
{
	int	i;
	int	j;

	i = 0;
	while (i < sim->num_philosophers)
	{
		if (pthread_create(&sim->philosophers[i].thread, NULL,
				philosopher_routine, &sim->philosophers[i]) != 0)
		{
			printf("Error: failed to create thread for philosopher %i\n",
				i + 1);
			pthread_mutex_lock(&sim->finish_mutex);
			sim->simulation_finished = 1;
			pthread_mutex_unlock(&sim->finish_mutex);
			j = 0;
			while (j < i)
			{
				pthread_join(sim->philosophers[j].thread, NULL);
				j++;
			}
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

// Joins all philosopher threads
void	join_philosopher_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philosophers)
	{
		pthread_join(sim->philosophers[i].thread, NULL);
		i++;
	}
}
