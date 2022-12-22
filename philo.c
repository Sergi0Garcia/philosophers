/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 19:17:33 by segarcia          #+#    #+#             */
/*   Updated: 2022/12/21 19:15:05 by segarcia         ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	if (valid_argc(argc) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	printf("%i\n", ft_atoi(argv[1]));
	printf("Hello World");
	return (1);
}
