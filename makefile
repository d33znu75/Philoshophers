# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rhmimchi <rhmimchi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/16 19:47:10 by rhmimchi          #+#    #+#              #
#    Updated: 2024/02/25 18:37:06 by rhmimchi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = ./src/main.c ./src/ft_atoi.c ./src/parser.c ./src/utils.c ./src/routine.c
OBJ = $(SRC:.c=.o)
NAME = philo
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	cc $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.c ./src/philo.h
	cc $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re