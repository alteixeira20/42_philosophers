/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 13:06:16 by paalexan          #+#    #+#             */
/*   Updated: 2025/08/05 15:02:34 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static void	assign_forks(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philosophers)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
		{
			printf("Error: mutex init\n");
			destroy_forks_on_failure(sim->forks, i);
			return ;
		}
		i++;
	}
}

static int	init_forks(t_simulation *sim)
{
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philosophers);
	if (!sim->forks)
	{
		printf("Error: malloc forks\n");
		return (FAILURE);
	}
	assign_forks(sim);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
	{
		printf("Error: mutex init\n");
		destroy_forks_on_failure(sim->forks, sim->num_philosophers);
		return (FAILURE);
	}
	return (SUCCESS);
}

static void	create_philosophers(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_philosophers)
	{
		sim->philosophers[i].id = i + 1;
		sim->philosophers[i].meals_eaten = 0;
		sim->philosophers[i].last_meal = sim->start_timestamp;
		sim->philosophers[i].sim = sim;
		if (pthread_mutex_init(&sim->philosophers[i].meal_mutex, NULL) != 0)
		{
			printf("Error: mutex init\n");
			destroy_philosophers_on_failure(sim->philosophers, i);
			return ;
		}
		i++;
	}
}

static int	init_philosophers(t_simulation *sim)
{
	sim->philosophers = malloc(sizeof(t_philosopher) * sim->num_philosophers);
	if (!sim->philosophers)
	{
		printf("Error: malloc philosophers\n");
		return (FAILURE);
	}
	create_philosophers(sim);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
	{
		printf("Error: mutex init (print_mutex)\n");
		destroy_forks_on_failure(sim->forks, sim->num_philosophers);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	init_simulation(t_simulation *sim)
{
	sim->simulation_finished = 0;
	sim->start_timestamp = current_timestamp_ms();
	if (init_forks(sim) == FAILURE)
		return (FAILURE);
	if (pthread_mutex_init(&sim->eating_lock, NULL) != 0)
	{
		printf("Error: mutex init (eating_lock)\n");
		destroy_forks_on_failure(sim->forks, sim->num_philosophers);
		return (FAILURE);
	}
	if (pthread_mutex_init(&sim->finish_mutex, NULL) != 0)
	{
		printf("Error: mutex init (finish_mutex)\n");
		pthread_mutex_destroy(&sim->eating_lock);
		destroy_forks_on_failure(sim->forks, sim->num_philosophers);
		return (FAILURE);
	}
	if (init_philosophers(sim) == FAILURE)
	{
		pthread_mutex_destroy(&sim->finish_mutex);
		pthread_mutex_destroy(&sim->eating_lock);
		destroy_forks_on_failure(sim->forks, sim->num_philosophers);
		return (FAILURE);
	}
	return (SUCCESS);
}
