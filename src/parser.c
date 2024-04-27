/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhmimchi <rhmimchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 20:02:12 by rhmimchi          #+#    #+#             */
/*   Updated: 2024/04/15 20:24:14 by rhmimchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static	int	is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

static	int	init_data(t_data *data, int ac, char **av)
{
	data->num_of_philo = ft_atoi(av[1]);
	data->time_to_die = (size_t) ft_atoi(av[2]);
	data->time_to_eat = (size_t) ft_atoi(av[3]);
	data->time_to_sleep = (size_t) ft_atoi(av[4]);
	if (ac == 6)
	{
		data->num_of_eat = ft_atoi(av[5]);
		if (data->num_of_eat < 0)
			return (1);
		data->meals_exist = 1;
	}
	else
		data->num_of_eat = -1;
	if (data->num_of_philo < 0 || data->time_to_die < 0 || \
		data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (1);
	return (0);
}

int	init_2(t_data *data)
{
	if (pthread_mutex_init(&data->mutex_logs, NULL)
		|| pthread_mutex_init(&data->mutex_eats, NULL)
		|| pthread_mutex_init(&data->mutex_stop, NULL)
		|| pthread_mutex_init(&data->mutex_time, NULL))
		return (1);
	data->num_philo_full = 0;
	data->stop = 0;
	data->start_time = get_time() + (10 * data->num_of_philo);
	data->time_to_think = data->time_to_eat - data->time_to_sleep;	// !!
	if (data->num_of_philo % 2 != 0) 								// !!
		data->time_to_think += data->time_to_eat;					// !!
	return (0);
}

int	init(t_data *data, int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}
	if (is_digit(av[1]) == 1 || is_digit(av[2]) == 1 || \
		is_digit(av[3]) == 1 || is_digit(av[4]) == 1)
	{
		printf("Error: arguments must be numbers\n");
		return (1);
	}
	if (ac == 6 && is_digit(av[5]) == 1)
	{
		printf("Error: arguments must be numbers\n");
		return (1);
	}
	if (init_data(data, ac, av) == 1)
	{
		printf("Error: wrong arguments\n");
		return (1);
	}
	if (init_2(data) == 1)
		return (printf("Error!\n"), 1);
	return (0);
}
