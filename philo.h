/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 19:18:12 by segarcia          #+#    #+#             */
/*   Updated: 2023/01/15 01:53:27 by segarcia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <limits.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

# define PHILO_FORK 1
# define PHILO_EATING 2
# define PHILO_SLEEPING 3
# define PHILO_THINKING 4
# define PHILO_DIED 5

typedef struct s_philo
{
	int					id;
	int					eated;
	pthread_t			thread;
	pthread_mutex_t		protection;
	pthread_mutex_t		*fork_r;
	pthread_mutex_t		*fork_l;
	struct s_data		*data;
	struct timeval		t_last_meal;
	pthread_mutex_t		t_last_meal_mutex;
}	t_philo;

typedef struct s_data
{
	t_philo				*philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print;
	pthread_mutex_t		running;
	int					n_philo;
	int					t_die;
	int					t_eat;
	int					t_sleep;
	int					n_must_eat;
	int					run;
	struct timeval		t_start;
}	t_data;

time_t	get_time_ms(void);
int		time_diff(struct timeval *begin);
void	ft_msleep(long ms, t_data *data);
int		ft_atoi(char *str);
int		valid_argcv(int argc, char **argv);
void	philo_monitor(t_data *data);
void	handle_action(t_data *data, t_philo *philo, int sig);
void	print_action(t_data *data, t_philo *philo, int sig);
int		init_args(t_data *data, int argc, char **argv);
int		is_running(t_data *data);
void	print_action(t_data *data, t_philo *philo, int sig);
void	handle_action(t_data *data, t_philo *philo, int sig);
void	*philo_routine(void *pt_philo);
int		init_philos(t_data *data);
int		free_all_data(t_data *data);
int		free_forks(pthread_mutex_t *forks, int n);
int		free_philos(t_philo *philos, int n);
int		free_data_mutexes(t_data *data);

#endif