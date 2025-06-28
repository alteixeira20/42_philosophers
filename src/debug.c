/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:36:31 by paalexan          #+#    #+#             */
/*   Updated: 2025/06/27 22:35:35 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	debug_print(const char *msg)
{
#if DEBUG_MODE
	printf("%s\n", msg);
#else
	(void)msg;
#endif
}

void	debug_simulation(t_simulation *sim)
{
#if DEBUG_MODE
	printf("Simulation initialized successfully:\n");
	printf("  Philosophers: %d\n", sim->num_philosophers);
	printf("  Time to die: %d ms\n", sim->time_to_die);
	printf("  Time to eat: %d ms\n", sim->time_to_eat);
	printf("  Time to sleep: %d ms\n", sim->time_to_sleep);
	if (sim->num_must_eat != -1)
		printf("  Must eat: %d times\n", sim->num_must_eat);
	else
		printf("  Must eat: unlimited\n");
#else
	(void)sim;
#endif
}

void	print_state(t_philosopher *philo, const char *msg)
{
	t_simulation	*sim;
	long			timestamp;

	sim = philo->sim;
	pthread_mutex_lock(&sim->print_mutex);
	if (!sim->simulation_finished)
	{
		timestamp = current_timestamp_ms() - sim->start_timestamp;
		printf("%ld %d %s\n", timestamp, philo->id, msg);
	}
	pthread_mutex_unlock(&sim->print_mutex);
}
