/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 19:44:45 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/14 18:40:37 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static int	ft_is_sign(int c)
{
	return (c == '-' || c == '+');
}

int	ft_atoi(char *str)
{
	int		i;
	int		sign;
	long	res;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	if (str[i] == '-')
		sign = -1;
	if (ft_is_sign(str[i]))
		i++;
	while (ft_isdigit(str[i]))
	{
		res = (res * 10) + (str[i++] - '0');
		if (res > 2147483647 && sign == 1)
			return (-1);
		if (res < 2147483648 && sign == -1)
			return (-1);
	}
	return (res * sign);
}

int	free_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
        pthread_mutex_destroy(data->philos[i].protection);
        pthread_mutex_destroy(data->philos[i].t_last_meal_mutex);
		i++;
	}
	pthread_mutex_destroy(data->print);
    pthread_mutex_destroy(data->running);
	return (EXIT_SUCCESS);
}
