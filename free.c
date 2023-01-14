/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 23:55:25 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/14 23:59:24 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	free_data_mutexes(t_data *data)
{
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->running);
	return (EXIT_FAILURE);
}

static void	free_philos(t_philo *philos, int n)
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
}

static void	free_forks(pthread_mutex_t *forks, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	free(forks);
}

void	free_all_data(t_data *data)
{
	free_data_mutexes(data);
	if (data->philos)
		free_philos(data->philos, data->n_philo);
	if (data->forks)
		free_forks(data->forks, data->n_philo);
}
