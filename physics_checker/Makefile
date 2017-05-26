# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elee <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/08/27 21:33:26 by elee              #+#    #+#              #
#    Updated: 2017/05/26 15:55:22 by smifsud          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = physics

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c octree.c physics.c misc.c barnes_hut.c bodies.c

OBJS = $(SRCS:.c=.o)

INCS = includes/

CHECKER = accuracy_checker

all: $(NAME)

$(NAME): $(CHECKER)
	mkdir -p build/
	$(CC) $(CFLAGS) -c $(addprefix srcs/, $(SRCS)) -I$(INCS)
	mv $(OBJS) build/
	gcc $(addprefix build/, $(OBJS)) -o $(NAME) -m64

$(CHECKER):
	gcc srcs/accuracy.c srcs/bodies.c srcs/misc.c -I $(INCS) -m64 -o $(CHECKER) -lgmp

clean:
	/bin/rm -rf build

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
