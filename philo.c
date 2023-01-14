/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 19:17:33 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/14 21:42:55 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_running(t_data *data)
{
	pthread_mutex_lock(data->running);
	if (data->run)
	{
		pthread_mutex_unlock(data->running);
		return (1);
	}
	pthread_mutex_unlock(data->running);
	return (0);
}

void	print_action(t_data *data, t_philo *philo, int sig)
{
	pthread_mutex_lock(data->print);
	if (sig == PHILO_FORK)
		printf("%i %i has taken a fork\n", time_diff(&data->t_start), philo->id);
	else if (sig == PHILO_EATING)
		printf("%i %i is eating\n", time_diff(&data->t_start), philo->id);
	else if (sig == PHILO_SLEEPING)
		printf("%i %i is sleeping\n", time_diff(&data->t_start), philo->id);
	else if (sig == PHILO_THINKING)
		printf("%d %d is thinking\n", time_diff(&data->t_start), philo->id);
	else if (sig == PHILO_DIED)
		printf("%d %d died\n", time_diff(&data->t_start), philo->id);
	pthread_mutex_unlock(data->print);
}

void	handle_action(t_data *data, t_philo *philo, int sig)
{
	if (!is_running(data))
		return ;
	if (sig == PHILO_FORK && is_running(data))
	   print_action(data, philo, PHILO_FORK);
	if (sig == PHILO_EATING && is_running(data))
	{
		print_action(data, philo, PHILO_EATING);
		pthread_mutex_lock(philo->t_last_meal_mutex);
		gettimeofday(&philo->t_last_meal, NULL);
		pthread_mutex_unlock(philo->t_last_meal_mutex);
		pthread_mutex_lock(philo->protection);
		philo->eated++;
		pthread_mutex_unlock(philo->protection);
		ft_msleep(philo->data->t_eat, data);
	}
	else if (sig == PHILO_SLEEPING && is_running(data))
	{
		print_action(data, philo, PHILO_SLEEPING);
		ft_msleep(data->t_sleep, data);
	}	
	else if (sig == PHILO_THINKING && is_running(data))
	{
		print_action(data, philo, PHILO_THINKING);
		ft_msleep(((data->t_die - time_diff(&philo->t_last_meal) - data->t_eat) / 2), data);
	}
	else if (sig == PHILO_DIED && is_running(data))
		print_action(data, philo, PHILO_DIED);
	return ((void) EXIT_SUCCESS);
}

static int	init_args(t_data *data, int argc, char **argv)
{
	data->n_philo = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_sleep = ft_atoi(argv[4]);
	data->print = malloc(sizeof(pthread_mutex_t));
	if (!data->print)
		return (EXIT_SUCCESS);
	data->running = malloc(sizeof(pthread_mutex_t));
	if (!data->running)
		return (EXIT_SUCCESS);
	if (argc == 6)
		data->n_must_eat = ft_atoi(argv[5]);
	else
		data->n_must_eat = -1;
	if (data->n_philo < 1 || data->t_die < 1
		|| data->t_eat < 1 || data->t_sleep < 1)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(data->print, NULL) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(data->running, NULL) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	create_forks(t_data *data, int n_philo)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * n_philo);
	if (!data->forks)
		return (EXIT_FAILURE);
	while (i < n_philo)
	{
		data->philos[i].t_last_meal_mutex = malloc(sizeof(pthread_mutex_t));
		data->philos[i].protection = malloc(sizeof(pthread_mutex_t));
		if (!data->philos[i].t_last_meal_mutex || !data->philos[i].protection)
			return (EXIT_SUCCESS);
		if (pthread_mutex_init(data->philos[i].t_last_meal_mutex, NULL) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		if (pthread_mutex_init(data->philos[i].protection, NULL) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		if (pthread_mutex_init(&data->forks[i], NULL) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

void	*philo(void *pt_philo)
{
	t_philo	*philo;
	t_data	*data;

	philo = pt_philo;
	data = philo->data;
	if (data->n_philo == 1)
	{
		pthread_mutex_lock(philo->fork_l);
		print_action(philo->data, philo, PHILO_FORK);
		handle_action(philo->data, philo, PHILO_FORK);
		return (NULL);
	}
	if (philo->id % 2 == 0)
	{
		handle_action(philo->data, philo, PHILO_THINKING);
	}
	while (is_running(data))
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
		data->philos[i].fork_r = &data->forks[(i + data->n_philo - 1)
			% data->n_philo];
		gettimeofday(&data->philos[i].t_last_meal, NULL);
		if (pthread_create(&data->philos[i].thread, NULL, &philo,
				(void *)&data->philos[i]))
			return (EXIT_FAILURE);
		i++;
	}
	philo_monitor(data);
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
	while (i < data.n_philo && !pthread_join(data.philos[i].thread, NULL))
		i++;
	free_threads(&data);
	return (EXIT_SUCCESS);
}
