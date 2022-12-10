# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: segarcia <segarcia@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/10 19:17:06 by segarcia          #+#    #+#              #
#    Updated: 2022/12/10 19:23:35 by segarcia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= philo

SRC			= philo.c

OBJS		= $(SRC:.c=.o)

CC			= gcc
RM			= rm -f
CFLAGS		= -Wall -Werror -Wextra -g
DEBUGFLAG	= -fsanitize=address

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	@$(RM) *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re