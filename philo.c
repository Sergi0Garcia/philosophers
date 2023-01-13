/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 19:17:33 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/13 12:13:04 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_action(t_data *data, t_philo *philo, int sig)
{
	int	time;

	time = diff_time_x(&data->t_start);
	if (sig == PHILO_FORK)
	{
		pthread_mutex_lock(data->print);
		printf("%i %i has taken a fork\n", time, philo->id);
		pthread_mutex_unlock(data->print);
	}
	else if (sig == PHILO_EATING)
	{
		pthread_mutex_lock(data->print);
		printf("%i %i is eating\n", time, philo->id);
		pthread_mutex_unlock(data->print);
		gettimeofday(&philo->t_last_meal, NULL);
		philo->eated++;
		ft_msleep(philo->data->t_eat);
	}
	else if (sig == PHILO_SLEEPING)
	{
		pthread_mutex_lock(data->print);
		printf("%i %i is sleeping\n", time, philo->id);
		pthread_mutex_unlock(data->print);
		ft_msleep(data->t_sleep);
	}
	else if (sig == PHILO_THINKING)
	{
		pthread_mutex_lock(data->print);
		printf("%d %d is thinking\n", time, philo->id);
		pthread_mutex_unlock(data->print);
	}		
	else if (sig == PHILO_DIED)
	{
		pthread_mutex_lock(data->print);
		printf("%d %d died\n", time, philo->id);
		pthread_mutex_unlock(data->print);
	}
}

static int	init_args(t_data *data, int argc, char **argv)
{
	data->n_philo = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_sleep = ft_atoi(argv[4]);
	data->print = malloc(sizeof(t_mutex) * 1);
	if (!data->print)
		return (EXIT_SUCCESS);
	if (argc == 6)
		data->n_must_eat = ft_atoi(argv[5]);
	else
		data->n_must_eat = -1;
	if (data->n_philo < 1 || data->t_die < 1
		|| data->t_eat < 1 || data->t_sleep < 1)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(data->print, NULL))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	create_forks(t_data *data, int n_philo)
{
	int	i;

	data->forks = malloc(sizeof(t_mutex) * n_philo);
	if (!data->forks)
		return (EXIT_FAILURE);
	i = 0;
	while (i < n_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

void	*philo(void *pt_philo)
{
	t_philo	*philo;

	philo = (t_philo *)pt_philo;
	if (philo->id % 2 == 0)
		ft_msleep(philo->data->t_eat * 0.9);
	while (philo->data->run == 1)
	{
		pthread_mutex_lock(philo->fork_l);
		handle_action(philo->data, philo, PHILO_FORK);
		pthread_mutex_lock(philo->fork_r);
		handle_action(philo->data, philo, PHILO_FORK);
		handle_action(philo->data, philo, PHILO_EATING);
		pthread_mutex_unlock(philo->fork_l);
		pthread_mutex_unlock(philo->fork_r);
		handle_action(philo->data, philo, PHILO_SLEEPING);
		handle_action(philo->data, philo, PHILO_THINKING);
	}
	return ((void *)EXIT_FAILURE);
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->n_philo);
	if (!data->philos)
		return (EXIT_FAILURE);
	if (create_forks(data, data->n_philo) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (pthread_create(&data->th_monitor, NULL, &philo_monitor, data))
		return (EXIT_FAILURE);
	gettimeofday(&data->t_start, NULL);
	data->run = 1;
	while (i < data->n_philo)
	{
		if (pthread_create(&data->philos[i].thread, NULL, &philo,
				&data->philos[i]))
			return (EXIT_FAILURE);
		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].eated = 0;
		data->philos[i].fork_l = &data->forks[i];
		data->philos[i].fork_r = &data->forks[(i + data->n_philo - 1)
			% data->n_philo];
		gettimeofday(&data->philos[i].t_last_meal, NULL);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_data		data;
	int			i;

	i = 0;
	if (valid_argcv(argc, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_args(&data, argc, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_philos(&data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	pthread_join(data.th_monitor, NULL);
	while (i < data.n_philo && data.run == 1)
	{
		pthread_join(data.philos[i].thread, NULL);
		i++;
	}
	free_threads(&data);
	return (EXIT_SUCCESS);
}
