# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: segarcia <segarcia@student.42wolfsburg.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/10 19:17:06 by segarcia          #+#    #+#              #
#    Updated: 2023/01/15 01:16:06 by segarcia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	philo

SRC			= 	philo.c 		\
				monitor.c		\
				free.c 			\
				utils.c			\
				time_utils.c	\
				init_args.c 	\
				init_philos.c	\
				controller.c 	\
				validation.c 

OBJS		= 	$(SRC:.c=.o)

CC			= 	gcc
RM			=	rm -f
CFLAGS		= 	-Wall -Werror -Wextra
DEBUGFLAG	= 	-fsanitize=address -g

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(DEBUGFLAG) $(SRC) -o $(NAME)

clean:
	@$(RM) *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re