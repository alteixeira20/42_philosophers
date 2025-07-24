/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:38:26 by paalexan          #+#    #+#             */
/*   Updated: 2025/07/24 17:33:44 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static int	setup_simulation(t_simulation *sim, int argc, char **argv)
{
	if (parse_args(sim, argc, argv) == FAILURE)
		return (FAILURE);
	if (init_simulation(sim) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

static int	run_simulation(t_simulation *sim)
{
	pthread_t	monitor_thread;

	if (create_philosopher_threads(sim) == FAILURE)
	{
		destroy_simulation(sim);
		return (FAILURE);
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, sim) != 0)
	{
		printf("Error: failed to create monitor thread\n");
		return (FAILURE);
	}
	pthread_join(monitor_thread, NULL);
	join_philosopher_threads(sim);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;

	if (setup_simulation(&sim, argc, argv) == FAILURE)
		return (FAILURE);
	if (run_simulation(&sim) == FAILURE)
	{
		destroy_simulation(&sim);
		return (FAILURE);
	}
	destroy_simulation(&sim);
	return (SUCCESS);
}
