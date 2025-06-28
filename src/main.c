/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:38:26 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/27 21:12:53 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static int	setup_simulation(t_simulation *sim, int argc, char **argv)
{
	debug_print("Parsing Arguments ...\n");
	if (parse_args(sim, argc, argv) == FAILURE)
		return (FAILURE);
	debug_print("Initializing Simulation ...\n");
	if (init_simulation(sim) == FAILURE)
		return (FAILURE);
	debug_simulation(sim);	
	return (SUCCESS);
}

static int	run_simulation(t_simulation *sim)
{
	pthread_t	monitor_thread;

	debug_print("Creating philosopher threads ...");
	if (create_philosopher_threads(sim) == FAILURE)
	{
		destroy_simulation(sim);
		return (FAILURE);
	}
	debug_print("Creating monitor thread ...");
	if (pthread_create(&monitor_thread, NULL, monitor_routine, sim) != 0)
	{
		printf("Error: failed to create monitor thread\n");
		return (FAILURE);
	}
	debug_print("Joining monitor thread ...");
	pthread_join(monitor_thread, NULL);
	debug_print("Joining philosopher threads ...");
	join_philosopher_threads(sim);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;

	debug_print("=== STARTING PROGRAM ===");
	if (setup_simulation(&sim, argc, argv) == FAILURE)
		return (FAILURE);
	if (run_simulation(&sim) == FAILURE)
	{
		debug_print("Destroying Simulation  ...\n");
		destroy_simulation(&sim);
		return (FAILURE);
	}
	debug_print("Destroying Simulation  ...\n");
	destroy_simulation(&sim);
	debug_print("=== ENDING PROGRAM ===\n");
	return (SUCCESS);
}
