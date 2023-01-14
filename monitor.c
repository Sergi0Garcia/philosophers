/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 12:06:00 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/14 18:46:08 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_dead(t_data *data, t_philo *philo)
{
	long	time;

    pthread_mutex_lock(philo->t_last_meal_mutex);
    time = diff_time_x(&philo->t_last_meal);
    pthread_mutex_unlock(philo->t_last_meal_mutex);
	if (time >= data->t_die)
	{
        handle_action(data, philo, PHILO_DIED);
		return (1);
	}
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
			if (data->n_must_eat != -1
				&& data->philos[i].eated >= data->n_must_eat)
				    n_philos_sated++;
            pthread_mutex_unlock(data->philos[i].protection);
			if (is_dead(data, &data->philos[i])
				|| n_philos_sated == data->n_philo)
			{
                pthread_mutex_lock(data->running);
				data->run = 0;
                pthread_mutex_unlock(data->running);
				return ((void ) EXIT_SUCCESS);
			}
			i++;
		}
		usleep(100);
	}
}
