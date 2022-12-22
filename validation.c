/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 19:51:17 by segarcia          #+#    #+#             */
/*   Updated: 2022/12/21 20:47:05 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	valid_argc(int argc)
{
	if (argc != 5 && argc != 6)
	{
		write(1, "Error usage: ", 13);
		write(1, "[number_of_philosophers] ", 25);
		write(1, "[time_to_die] ", 14);
		write(1, "[time_to_eat] ", 14);
		write(1, "[time_to_sleep] ", 16);
		write(1, "[number_of_times_each_philosopher_must_eat]\n", 44);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	valid_argv(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{	
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] >= '0' && argv[i][j] <= '9')
				j++;
			else
			{
				write(1, "Enter valid number \n", 20);
				return (EXIT_FAILURE);
			}
		}
		if (ft_atoi(argv[i]) == -1)
		{
			write(1, "Enter number between MIN_INT - MAX_INT\n", 39);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	valid_argcv(int argc, char **argv)
{
	if (valid_argc(argc) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (valid_argv(argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
