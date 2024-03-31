/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhmimchi <rhmimchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 19:47:29 by rhmimchi          #+#    #+#             */
/*   Updated: 2024/03/31 02:47:01 by rhmimchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_init(t_data *data, t_philo **philo)
{
	int	i;

	i = 0;
	while (i < data->num_of_philo)
	{
		(*philo + i)->id = i;
		(*philo + i)->data = data;
		(*philo + i)->num_of_eat = 0;
		(*philo + i)->time_last_eat = get_time();
		(*philo + i)->full = 0;
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

void	ft_free(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->mutex_eats);
	pthread_mutex_destroy(&data->mutex_time);
	pthread_mutex_destroy(&data->mutex_logs);
	while (i < data->num_of_philo)
	{
		pthread_mutex_destroy(&data->forks_mtx[i]);
		i++;
	}
	free(data->forks_mtx);
	free(philo);
}

int	main(int ac, char **av)
{
	t_data		data;
	t_philo		*philo;
	int			i;
	pthread_t	thread;

	if (init(&data, ac, av) == 1)
		return (1);
	if (allocations(&data, &philo) == 1)
		return (1);
	if (data.num_of_eat == 0)
		return (ft_free(&data, philo), 0);
	i = -1;
	while (++i < data.num_of_philo)
		pthread_create(&philo[i].thread, NULL, routine, &philo[i]);
	pthread_create(&thread, NULL, check_death, philo);
	i = -1;
	while (++i < data.num_of_philo)
		pthread_join((philo + i)->thread, NULL);
	pthread_join(thread, NULL);
	if (data.time_to_eat > data.time_to_sleep)
		ft_usleep(data.time_to_eat * 2);
	else
		ft_usleep(data.time_to_sleep * 2);
	ft_free(&data, philo);
	return (0);
}
