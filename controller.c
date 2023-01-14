/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/14 22:17:03 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/14 23:11:55 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_running(t_data *data)
{
	pthread_mutex_lock(&data->running);
	if (data->run)
	{
		pthread_mutex_unlock(&data->running);
		return (1);
	}
	pthread_mutex_unlock(&data->running);
	return (0);
}

void	print_action(t_data *data, t_philo *philo, int sig)
{
	pthread_mutex_lock(&data->print);
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
	pthread_mutex_unlock(&data->print);
}

static void	handle_philo_eating(t_data *data, t_philo *philo)
{
	print_action(data, philo, PHILO_EATING);
	pthread_mutex_lock(&philo->t_last_meal_mutex);
	gettimeofday(&philo->t_last_meal, NULL);
	pthread_mutex_unlock(&philo->t_last_meal_mutex);
	pthread_mutex_lock(&philo->protection);
	philo->eated++;
	pthread_mutex_unlock(&philo->protection);
	ft_msleep(philo->data->t_eat, data);
}

void	handle_action(t_data *data, t_philo *philo, int sig)
{
	if (!is_running(data))
		return ;
	if (sig == PHILO_FORK && is_running(data))
		print_action(data, philo, PHILO_FORK);
	if (sig == PHILO_EATING && is_running(data))
		handle_philo_eating(data, philo);
	else if (sig == PHILO_SLEEPING && is_running(data))
	{
		print_action(data, philo, PHILO_SLEEPING);
		ft_msleep(data->t_sleep, data);
	}	
	else if (sig == PHILO_THINKING && is_running(data))
	{
		print_action(data, philo, PHILO_THINKING);
		ft_msleep(((data->t_die - time_diff(&philo->t_last_meal) \
			- data->t_eat) * 0.5), data);
	}
	else if (sig == PHILO_DIED && is_running(data))
		print_action(data, philo, PHILO_DIED);
	return ((void) EXIT_SUCCESS);
}

void	*philo_routine(void *pt_philo)
{
	t_philo	*philo;

	philo = pt_philo;
	if (philo->data->n_philo == 1)
	{
		pthread_mutex_lock(philo->fork_l);
		handle_action(philo->data, philo, PHILO_FORK);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		handle_action(philo->data, philo, PHILO_THINKING);
	while (is_running(philo->data))
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
	return ((void *)EXIT_SUCCESS);
}
