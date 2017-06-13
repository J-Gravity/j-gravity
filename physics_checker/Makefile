# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elee <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/08/27 21:33:26 by elee              #+#    #+#              #
#    Updated: 2017/06/02 16:44:14 by smifsud          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = physics

CC = gcc

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c octree.c physics.c misc.c barnes_hut.c bodies.c

OBJS = $(SRCS:.c=.o)

INCS = includes/

CHECKER = accuracy_checker

GPUACC = gpu_checker

all: $(NAME) $(CHECKER)

windows:
	gcc srcs/special.c srcs/main.c srcs/physics.c srcs/bodies.c srcs/misc.c srcs/octree.c srcs/barnes_hut.c -m64 -lm -o physics -std=c99 -I includes/ -D_GNU_SOURCE

$(NAME):
	mkdir -p build/
	$(CC) $(CFLAGS) -c $(addprefix srcs/, $(SRCS)) -I$(INCS)
	mv $(OBJS) build/
	gcc $(addprefix build/, $(OBJS)) -o $(NAME) -m64 -lm -std=c99

$(CHECKER):
	gcc srcs/accuracy.c srcs/bodies.c srcs/misc.c libgmp.a -I $(INCS) -m64 -o $@ -lm

$(GPUACC):
	gcc srcs/gpuaccuracy.c srcs/bodies.c srcs/misc.c libgmp.a -I $(INCS) -m64 -o $@ -lm

checkers: $(GPUACC) $(CHECKER)

clean:
	/bin/rm -rf build

fclean: clean
	/bin/rm -f $(NAME)
	rm -f $(CHECKER)
	rm -f $(GPUACC)

re: fclean all

.PHONY: all clean fclean re
