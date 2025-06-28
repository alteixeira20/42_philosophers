#include "../inc/philosophers.h"

// Checks if a string represents a positive integer
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

// Gets argument name automatically for easier debugging
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
	long	ms;

	gettimeofday(&tv, NULL);
	ms = (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
	return (ms);
}

void	precise_sleep(t_simulation *sim, int duration_ms)
{
	long	start;
	long	now;

	start = current_timestamp_ms();
	while (!sim->simulation_finished)
	{
		now = current_timestamp_ms();
		if ((now - start) >= duration_ms)
			break ;
		usleep(500);
	}
}
