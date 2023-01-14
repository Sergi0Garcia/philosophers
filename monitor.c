/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 12:06:00 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/14 21:42:08 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_dead(t_data *data, t_philo *philo)
{
	long	time;

	pthread_mutex_lock(philo->t_last_meal_mutex);
	time = time_diff(&philo->t_last_meal);
	pthread_mutex_unlock(philo->t_last_meal_mutex);
	if (time >= data->t_die)
	{
		pthread_mutex_lock(data->running);
		data->run = 0;
		pthread_mutex_unlock(data->running);
		print_action(data, philo, PHILO_DIED);
		return (1);
	}
	return (0);
}

static int	is_philo_sated(t_data *data, int i)
{
	if (data->n_must_eat != -1)
		return (0);
	if (data->philos[i].eated >= data->n_must_eat)
		return (1);
	return (0);
}

void	philo_monitor(t_data *data)
{
	int		i;
	int		n_philos_sated;

	while (1)
	{
		i = 0;
		n_philos_sated = 0;
		while (i < data->n_philo)
		{
			pthread_mutex_lock(data->philos[i].protection);
			if (is_philo_sated(data, i))
				n_philos_sated++;
			pthread_mutex_unlock(data->philos[i].protection);
			if (is_dead(data, &data->philos[i])
				|| n_philos_sated == data->n_philo)
			{
				pthread_mutex_lock(data->running);
				data->run = 0;
				pthread_mutex_unlock(data->running);
				return ;
			}
			i++;
		}
		usleep(100);
	}
}
