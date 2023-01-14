/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 22:24:02 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/15 00:01:11 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	create_forks(t_data *data, int n_philo)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * n_philo);
	if (!data->forks)
		return (EXIT_FAILURE);
	while (i < n_philo)
	{
		pthread_mutex_init(&data->philos[i].t_last_meal_mutex, NULL);
		pthread_mutex_init(&data->philos[i].protection, NULL);
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	return (EXIT_SUCCESS);
}

static int	right_fork(t_data *data, int i)
{
	return ((i + data->n_philo - 1) % data->n_philo);
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * (data->n_philo));
	if (!data->philos)
		return (EXIT_FAILURE);
	if (create_forks(data, data->n_philo) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->run = 1;
	gettimeofday(&data->t_start, NULL);
	while (i < data->n_philo)
	{
		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].eated = 0;
		data->philos[i].fork_l = &data->forks[i];
		data->philos[i].fork_r = &data->forks[right_fork(data, i)];
		gettimeofday(&data->philos[i].t_last_meal, NULL);
		if (pthread_create(&data->philos[i].thread, NULL, &philo_routine,
				(void *)&data->philos[i]))
			return (EXIT_FAILURE);
		i++;
	}
	philo_monitor(data);
	return (EXIT_SUCCESS);
}
