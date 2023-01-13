/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 12:06:00 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/13 12:13:36 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_dead(t_data *data, t_philo *philo)
{
	long	time;

	time = diff_time_x(&philo->t_last_meal);
	if (time >= data->t_die)
	{
		handle_action(data, philo, PHILO_DIED);
		philo->eated = data->n_must_eat + 1;
		return (1);
	}
	return (0);
}

void	*philo_monitor(void *phi_data)
{
	int		i;
	int		eated;
	t_data	*data;

	data = (t_data *)phi_data;
	while (1)
	{
		i = 0;
		eated = 0;
		while (i < data->n_philo)
		{
			if (data->n_must_eat != -1
				&& data->philos[i].eated >= data->n_must_eat)
				eated++;
			if (is_dead(data, &data->philos[i])
				|| eated == data->n_philo)
			{
				data->run = 0;
				free_threads(data);
				return ((void *)0);
			}
			i++;
		}
		usleep(100);
	}
}
