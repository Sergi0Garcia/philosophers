/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 19:17:33 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/15 02:02:42 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data		data;
	int			i;

	i = 0;
	data.philos = NULL;
	data.forks = NULL;
	if (valid_argcv(argc, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_args(&data, argc, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_philos(&data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (i < data.n_philo && !pthread_join(data.philos[i].thread, NULL))
		i++;
	free_all_data(&data);
	return (EXIT_SUCCESS);
}
