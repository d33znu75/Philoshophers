/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhmimchi <rhmimchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 18:08:07 by rhmimchi          #+#    #+#             */
/*   Updated: 2024/06/12 06:27:19 by rhmimchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	is_eating1(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_time);
	philo->time_last_eat = get_time();
	pthread_mutex_unlock(&philo->data->mutex_time);
	pthread_mutex_lock(&philo->data->forks_mtx[philo->id]);
	printing("has taken a fork", philo);
	pthread_mutex_lock(&philo->data->forks_mtx[(philo->id + 1) % \
		philo->data->num_of_philo]);
	pthread_mutex_lock(&philo->data->mutex_stop);
}

void	is_eating(t_philo *philo)
{
	is_eating1(philo);
	if (philo->data->stop)
	{
		pthread_mutex_unlock(&philo->data->forks_mtx[philo->id]);
		pthread_mutex_unlock(&philo->data->forks_mtx[(philo->id + 1) % \
			philo->data->num_of_philo]);
		pthread_mutex_unlock(&philo->data->mutex_stop);
		return ;
	}
	pthread_mutex_unlock(&philo->data->mutex_stop);
	printing("has taken a fork", philo);
	printing("is eating", philo);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->data->forks_mtx[philo->id]);
	pthread_mutex_unlock(&philo->data->forks_mtx[(philo->id + 1) % \
		philo->data->num_of_philo]);
	pthread_mutex_lock(&philo->data->mutex_eats);
	philo->num_of_eat++;
	if (philo->num_of_eat == philo->data->num_of_eat)
		philo->full = 1;
	pthread_mutex_unlock(&philo->data->mutex_eats);
}

void	routine2(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->data->mutex_stop);
		if (philo->data->stop)
			return ((void) pthread_mutex_unlock(&philo->data->mutex_stop));
		pthread_mutex_unlock(&philo->data->mutex_stop);
		is_eating(philo);
		pthread_mutex_lock(&philo->data->mutex_stop);
		if (philo->data->stop)
			return ((void) pthread_mutex_unlock(&philo->data->mutex_stop));
		pthread_mutex_unlock(&philo->data->mutex_stop);
		is_slepping(philo);
		pthread_mutex_lock(&philo->data->mutex_stop);
		if (philo->data->stop)
			return ((void) pthread_mutex_unlock(&philo->data->mutex_stop));
		pthread_mutex_unlock(&philo->data->mutex_stop);
		is_thinking(philo);
	}
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->mutex_time);
	philo->time_last_eat = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->mutex_time);
	sleep_start(philo);
	if (philo->data->num_of_philo == 1)
	{
		printing("is thinking", philo);
		ft_usleep(philo->data->time_to_die + 69);
	}
	else if (philo->id % 2 == 1)
	{
		printing("is thinking", philo);
		ft_usleep(philo->data->time_to_eat);
	}
	else if (philo->id == philo->data->num_of_philo - 1)
	{
		printing("is thinking", philo);
		ft_usleep(philo->data->time_to_eat * 2);
	}
	routine2(philo);
	return (NULL);
}
