# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elee <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/08/27 21:33:26 by elee              #+#    #+#              #
#    Updated: 2017/05/16 16:29:08 by elee             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = physics

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c octree.c physics.c misc.c barnes_hut.c bodies.c

OBJS = $(SRCS:.c=.o)

INCS = includes/

all: $(NAME)

$(NAME):
	mkdir -p build/
	$(CC) $(CFLAGS) -c $(addprefix srcs/, $(SRCS)) -I$(INCS)
	mv $(OBJS) build/
	gcc $(addprefix build/, $(OBJS)) -o $(NAME)

clean:
	/bin/rm -rf build

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
