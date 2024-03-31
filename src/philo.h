/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rhmimchi <rhmimchi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 19:48:29 by rhmimchi          #+#    #+#             */
/*   Updated: 2024/03/29 11:04:10 by rhmimchi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_data
{
	int				num_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_think;
	int				num_of_eat;
	int				num_philo_full;
	int				meals_exist;
	int				stop;
	size_t			start_time;
	pthread_mutex_t	mutex_time;
	pthread_mutex_t	mutex_stop;
	pthread_mutex_t	mutex_eats;
	pthread_mutex_t	mutex_logs;
	pthread_mutex_t	*forks_mtx;
}				t_data;

typedef struct s_philo
{
	t_data		*data;
	pthread_t	thread;
	size_t		time_last_eat;
	int			num_of_eat;
	int			full;
	int			id;
}				t_philo;

int		ft_atoi(const char *str);
int		init(t_data *data, int ac, char **av);
size_t	get_time(void);
void	ft_usleep(size_t time);
int		ft_strcmp(const char *s1, const char *s2);
void	sleep_start(t_philo *philo);

// routine functions
void	printing(char *str, t_philo *philo);
void	take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);
void	eat(t_philo *philo);
void	*check_death(void *arg);
void	*routine(void *arg);

#endif