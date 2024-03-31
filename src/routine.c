/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhmimchi <rhmimchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 18:08:07 by rhmimchi          #+#    #+#             */
/*   Updated: 2024/03/31 02:52:48 by rhmimchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	all_full(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo->data->mutex_eats);
	pthread_mutex_lock(&philo->data->mutex_stop);
	if ((philo + i)->full)
		philo->data->num_philo_full += 1;
	pthread_mutex_unlock(&philo->data->mutex_eats);
	if (philo->data->num_philo_full == philo->data->num_of_philo)
	{
		philo->data->stop = 1;
		pthread_mutex_unlock(&philo->data->mutex_stop);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->mutex_stop);
	return (0);
}

void	philo_dead(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo->data->mutex_logs);
	printf("%ld %d died\n", get_time() - philo->data->start_time,
		(philo + i)->id + 1);
	pthread_mutex_lock(&philo->data->mutex_stop);
	philo->data->stop = 1;
	pthread_mutex_unlock(&philo->data->mutex_stop);
	pthread_mutex_unlock(&philo->data->mutex_logs);
}

void	*check_death(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	sleep_start(philo);
	while (1)
	{
		i = 0;
		while (i < philo->data->num_of_philo)
		{
			pthread_mutex_lock(&philo->data->mutex_time);
			if (get_time() - (philo + i)->time_last_eat > \
				(size_t) philo->data->time_to_die)
			{
				philo_dead(philo, i);
				return (pthread_mutex_unlock(&philo->data->mutex_time), NULL);
			}
			pthread_mutex_unlock(&philo->data->mutex_time);
			if (philo->data->meals_exist == 1 && all_full(philo, i))
				return (NULL);
			i++;
		}
		philo->data->num_philo_full = 0;
	}
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_stop);
	if (philo->data->stop)
	{
		pthread_mutex_unlock(&philo->data->mutex_stop);
		return ;
	}
	pthread_mutex_unlock(&philo->data->mutex_stop);
	printing("is sleeping", philo);
	ft_usleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_stop);
	if (philo->data->stop)
	{
		pthread_mutex_unlock(&philo->data->mutex_stop);
		return ;
	}
	pthread_mutex_unlock(&philo->data->mutex_stop);
	if (philo->data->time_to_think > 0)
	{
		printing("is thinking", philo);
		ft_usleep(philo->data->time_to_think);
	}
	else
		printing("is thinking", philo);
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_time);
	philo->time_last_eat = get_time();
	pthread_mutex_unlock(&philo->data->mutex_time);
	pthread_mutex_lock(&philo->data->forks_mtx[philo->id]);
	printing("has taken a fork", philo);
	pthread_mutex_lock(&philo->data->forks_mtx[(philo->id + 1) % \
		philo->data->num_of_philo]);
	pthread_mutex_lock(&philo->data->mutex_stop);
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
		philo_eat(philo);
		pthread_mutex_lock(&philo->data->mutex_stop);
		if (philo->data->stop)
			return ((void) pthread_mutex_unlock(&philo->data->mutex_stop));
		pthread_mutex_unlock(&philo->data->mutex_stop);
		philo_sleep(philo);
		pthread_mutex_lock(&philo->data->mutex_stop);
		if (philo->data->stop)
			return ((void) pthread_mutex_unlock(&philo->data->mutex_stop));
		pthread_mutex_unlock(&philo->data->mutex_stop);
		philo_think(philo);
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
		printing("is thinking\n", philo);
		ft_usleep(philo->data->time_to_die * 2);
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
