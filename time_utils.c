/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 12:03:51 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/14 23:03:51 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	time_diff(struct timeval *begin)
{
	struct timeval	now;
	int				time;

	gettimeofday(&now, NULL);
	time = (now.tv_sec - begin->tv_sec) * 1000 + \
		(now.tv_usec - begin->tv_usec) / 1000;
	return (time);
}

void	ft_msleep(long ms, t_data *data)
{
	struct timeval	begin;
	long			res;

	gettimeofday(&begin, NULL);
	res = 0;
	while (res < ms)
	{
		pthread_mutex_lock(&data->running);
		if (!data->run)
		{
			pthread_mutex_unlock(&data->running);
			return ;
		}
		pthread_mutex_unlock(&data->running);
		usleep(100);
		res = time_diff(&begin);
	}
}
