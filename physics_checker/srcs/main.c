/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/04 13:31:03 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/16 16:28:43 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <checker.h>
#include <unistd.h>
#include <stdio.h>

void			setnode(t_octant *node, t_body *bodies, int64_t nbodies)
{
	node->parent = 0;
	node->bodies = bodies;
	node->start = 0;
	node->end = (size_t)nbodies - 1;
	node->children = (t_octant**)malloc(sizeof(t_octant*) * 8);
}

void				simulation(int o, t_octant *universe)
{
	int		i;
	
	i = 0;
	while (i < o)
	{
		universe = barnes_hut(universe);
	}
}

t_body			*create_bodies(size_t num_bodies)
{
	t_body	*bodies;

	bodies = (t_body*)malloc(sizeof(t_body) * num_bodies);
	for (size_t i = 0; i < num_bodies; i++)
	{
		bodies[i].position.x = (double)rand() / RAND_MAX * 100;
		bodies[i].position.y = (double)rand() / RAND_MAX * 100;
		bodies[i].position.z = (double)rand() / RAND_MAX * 100;
		bodies[i].velocity.x = 0.0;
		bodies[i].velocity.y = 0.0;
		bodies[i].velocity.z = 0.0;
		bodies[i].mass = 10000000;
	}
	return (bodies);
}

int				main(int argc, char **argv)
{
	//int			fd;
	char		*buf;
	t_body		*bodies;
	t_octant	*root;

	(void)argc;
	buf = malloc(0xFFFFFFFF); //reserve 4 gb of memory
	if (!buf)
	{
		printf("ERROR NOT ENOUGH MEMORY ON SYSTEM\n");
		return (1);
	}
	free(buf);
	//fd = open(argv[1], O_RDONLY);
	bodies = create_bodies(atoi(argv[1]));
	/*
	bodies = (sortbodies(getbodies(fd,
					parsenbodies(buf))));
					*/
	root = (t_octant*)malloc(sizeof(t_octant) * 1);
	setnode(root, bodies, atoi(argv[1]));
	octree_divide(root);
	printf("wtf\n");
	simulation(atoi(argv[2]), root);
	return (0);
}
