/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 19:17:33 by segarcia          #+#    #+#             */
/*   Updated: 2022/12/26 20:18:25 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	diff_time_x(struct timeval *begin)
{
	struct timeval	now;
	int				time;

	gettimeofday(&now, NULL);
	time = (now.tv_sec - begin->tv_sec) * 1000 + \
		(now.tv_usec - begin->tv_usec) / 1000;
	return (time);
}

void	ft_msleep(long ms)
{
	struct timeval	begin;
	long			res;

	gettimeofday(&begin, NULL);
	res = 0;
	while (res < ms)
	{
		usleep(100);
		res = diff_time_x(&begin);
	}
}

void	output(t_data *data, t_philo *philo, int sig)
{
	int	time;

	time = diff_time_x(&data->t_start);
	if (sig == 1)
		printf("%d %d has taken a fork\n", time, philo->id);
	else if (sig == 2)
	{
		printf("%d %d is eating\n", time, philo->id);
		gettimeofday(&philo->t_last_meal, NULL);
		philo->eated++;
		ft_msleep(philo->data->t_eat);
	}
	else if (sig == 3)
	{
		printf("%d %d is sleeping\n", time, philo->id);
		ft_msleep(data->t_sleep);
	}
	else if (sig == 4)
		printf("%d %d is thinking\n", time, philo->id);
	else if (sig == 5)
		printf("%d %d died\n", time, philo->id);
}

int	is_dead(t_data *data, t_philo *philo)
{
	long	time;

	time = diff_time_x(&philo->t_last_meal);
	if (time >= data->t_die)
	{
		output(data, philo, 5);
		philo->eated = data->n_must_eat + 1;
		return (1);
	}
	return (0);
}

int	free_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	return (0);
}

static int	init_args(t_data *data, int argc, char **argv)
{
	data->n_philo = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->n_must_eat = ft_atoi(argv[5]);
	else
		data->n_must_eat = 2147483647;
	if (data->n_philo < 1 || data->t_die < 1
		|| data->t_eat < 1 || data->t_sleep < 1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int create_forks(t_data *data, int n_philo)
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

void	*thread_monitor(void *phi_data)
{
	int		i;
	int		eated;
	t_data	*data;

	data = (t_data *)phi_data;
	ft_msleep(data->t_die * 0.9);
	while (1)
	{
		i = 0;
		eated = 0;
		while (i < data->n_philo)
		{
			if (data->philos[i].eated >= data->n_must_eat)
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
		usleep(200);
	}
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
		output(philo->data, philo, 1);
		pthread_mutex_lock(philo->fork_r);
		output(philo->data, philo, 1);
		output(philo->data, philo, 2);
		pthread_mutex_unlock(philo->fork_l);
		pthread_mutex_unlock(philo->fork_r);
		output(philo->data, philo, 3);
		output(philo->data, philo, 4);
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
	if (pthread_create(&data->th_monitor, NULL, &thread_monitor, data))
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
