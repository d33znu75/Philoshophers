/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhmimchi <rhmimchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:15:04 by rhmimchi          #+#    #+#             */
/*   Updated: 2024/06/12 02:00:57 by rhmimchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_init(t_data *data, t_philo **philo)
{
	int	i;

	i = 0;
	while (i < data->num_of_philo)
	{
		(*philo)[i].id = i;
		(*philo)[i].data = data;
		(*philo)[i].num_of_eat = 0;
		(*philo)[i].time_last_eat = get_time();
		(*philo)[i].full = 0;
		i++;
	}
}

int	forks_init(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_philo)
	{
		if (pthread_mutex_init(&data->forks_mtx[i], NULL))
		{
			printf("Error: pthread_mutex_init failed\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	allocations(t_data *data, t_philo **philo)
{
	*philo = malloc(sizeof(t_philo) * data->num_of_philo);
	if (!*philo)
	{
		printf("Error: malloc failed\n");
		return (1);
	}
	philo_init(data, philo);
	data->forks_mtx = malloc(sizeof(pthread_mutex_t) * data->num_of_philo);
	if (!data->forks_mtx)
	{
		printf("Error: malloc failed\n");
		free(philo);
		return (1);
	}
	if (forks_init(data))
	{
		free(philo);
		free(data->forks_mtx);
		return (1);
	}
	return (0);
}
