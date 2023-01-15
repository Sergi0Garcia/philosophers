/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 23:55:25 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/15 02:01:14 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_data_mutexes(t_data *data)
{
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->running);
	return (EXIT_FAILURE);
}

int	free_philos(t_philo *philos, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&philos[i].t_last_meal_mutex);
		pthread_mutex_destroy(&philos[i].protection);
		i++;
	}
	free(philos);
	return (EXIT_FAILURE);
}

int	free_forks(pthread_mutex_t *forks, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(forks);
	return (EXIT_FAILURE);
}

int	free_all_data(t_data *data)
{
	free_data_mutexes(data);
	if (data->philos)
		free_philos(data->philos, data->n_philo);
	if (data->forks)
		free_forks(data->forks, data->n_philo);
	return (EXIT_FAILURE);
}
