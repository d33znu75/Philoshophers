/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhmimchi <rhmimchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 19:47:29 by rhmimchi          #+#    #+#             */
/*   Updated: 2024/06/12 05:18:51 by rhmimchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_free(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->mutex_eats);
	pthread_mutex_destroy(&data->mutex_time);
	pthread_mutex_destroy(&data->mutex_logs);
	pthread_mutex_destroy(&data->mutex_stop);
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
		pthread_join(philo[i].thread, NULL);
	pthread_join(thread, NULL);
	ft_free(&data, philo);
	return (0);
}
