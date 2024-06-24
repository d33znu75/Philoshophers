/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhmimchi <rhmimchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:20:36 by rhmimchi          #+#    #+#             */
/*   Updated: 2024/06/12 01:40:47 by rhmimchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_all_full(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo->data->mutex_eats);
	pthread_mutex_lock(&philo->data->mutex_stop);
	if (philo[i].full)
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

void	is_dead(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo->data->mutex_logs);
	pthread_mutex_lock(&philo->data->mutex_stop);
	printf("%ld %d died\n", get_time() - philo->data->start_time,
		philo[i].id + 1);
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
			if (get_time() - philo[i].time_last_eat > \
				(size_t) philo->data->time_to_die)
			{
				is_dead(philo, i);
				return (pthread_mutex_unlock(&philo->data->mutex_time), NULL);
			}
			pthread_mutex_unlock(&philo->data->mutex_time);
			if (philo->data->meals_exist == 1 && is_all_full(philo, i))
				return (NULL);
			i++;
		}
		philo->data->num_philo_full = 0;
	}
}

void	is_slepping(t_philo *philo)
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

void	is_thinking(t_philo *philo)
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
