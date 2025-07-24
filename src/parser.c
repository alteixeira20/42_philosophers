/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paalexan <paalexan@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:14:00 by paalexan          #+#    #+#             */
/*   Updated: 2025/07/24 17:45:06 by paalexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static int	parse_int(const char *str)
{
	long	num;
	int		i;

	num = 0;
	i = 0;
	while (str[i])
	{
		num = num * 10 + (str[i] - '0');
		if (num > INT_MAX)
			return (ERROR);
		i++;
	}
	return ((int)num);
}

static int	validate_arguments(int argc, char **argv)
{
	int	i;
	int	value;

	i = 1;
	while (i < argc)
	{
		if (is_positive_integer(argv[i]) == FAILURE)
		{
			printf("Error: argument '%s' ('%s') is not a positive integer\n",
				get_arg_description(i), argv[i]);
			return (FAILURE);
		}
		value = parse_int(argv[i]);
		if (value <= 0)
		{
			printf("Error: argument '%s' ('%s') ",
				get_arg_description(i), argv[i]);
			printf("is out of bounds (>0 and <= INT_MAX)\n");
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

static int	convert_args(t_simulation *sim, int argc, char **argv)
{
	if (validate_arguments(argc, argv) == FAILURE)
		return (FAILURE);
	sim->num_philosophers = parse_int(argv[1]);
	sim->time_to_die = parse_int(argv[2]);
	sim->time_to_eat = parse_int(argv[3]);
	sim->time_to_sleep = parse_int(argv[4]);
	if (argc == 6)
		sim->num_must_eat = parse_int(argv[5]);
	else
		sim->num_must_eat = -1;
	return (SUCCESS);
}

int	parse_args(t_simulation *sim, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s num_philosophers ", argv[0]);
		printf("time_to_die time_to_eat time_to_sleep [num_must_eat]\n");
		return (FAILURE);
	}
	else
		return (convert_args(sim, argc, argv));
}
