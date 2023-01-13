/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 12:03:51 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/13 12:15:50 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

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
