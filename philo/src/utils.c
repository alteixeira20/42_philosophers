/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:40:09 by paalexan          #+#    #+#             */
/*   Updated: 2025/08/05 15:06:47 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	is_positive_integer(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (FAILURE);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

const char	*get_arg_description(int index)
{
	if (index == 1)
		return (ARG_NUM_PHILOSOPHERS);
	if (index == 2)
		return (ARG_TIME_TO_DIE);
	if (index == 3)
		return (ARG_TIME_TO_EAT);
	if (index == 4)
		return (ARG_TIME_TO_SLEEP);
	if (index == 5)
		return (ARG_NUM_MUST_EAT);
	return ("unknown");
}

long	current_timestamp_ms(void)
{
	struct timeval	tv;
	long			ms;

	gettimeofday(&tv, NULL);
	ms = (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
	return (ms);
}

void	precise_sleep(t_simulation *sim, int duration_ms)
{
	long	start;
	long	now;

	start = current_timestamp_ms();
	while (1)
	{
		now = current_timestamp_ms();
		if ((now - start) >= duration_ms)
			break ;
		if (is_simulation_finished(sim))
			break ;
		usleep(500);
	}
}

void	print_state(t_philosopher *philo, const char *msg)
{
	t_simulation	*sim;
	long			timestamp;

	sim = philo->sim;
	if (is_simulation_finished(sim))
		return ;
	pthread_mutex_lock(&sim->print_mutex);
	timestamp = current_timestamp_ms() - sim->start_timestamp;
	printf("%ld %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&sim->print_mutex);
}
