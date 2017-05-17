/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/04 13:31:03 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/17 00:22:24 by elee             ###   ########.fr       */
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
		barnes_hut(universe);
		//printf("%zu, %zu\n", universe->start, universe->end);
		/*
		for (size_t j = universe->start; j <= universe->end; j++)
		{
			printf("PARTICLE ID %zu:\nposition = (%lf, %lf, %lf)\nvelocity = (%lf, %lf, %lf)\n", 
			universe->bodies[i].id,
			universe->bodies[i].position.x,
			universe->bodies[i].position.y,
			universe->bodies[i].position.z,
			universe->bodies[i].velocity.x,
			universe->bodies[i].velocity.y,
			universe->bodies[i].velocity.z);
		}
		*/
		i++;
	}
}

double				rand_double(double max)
{
	double	r;

	r = (double)rand() / (double)(RAND_MAX/max);
	return (r);
}

t_body			rand_body(int mag)
{
	t_body	body;
	double	elevation;
	double	azimuth;
	double	radius;

	elevation = asin(rand_double(2) - 1);
	azimuth = 2 * M_PI * rand_double(1);
	radius = cbrt(rand_double(1)) * __exp10(mag);
	body.position = (t_vector){radius * cos(elevation) * cos(azimuth), \
								radius * cos(elevation) * sin(azimuth),
								0.3 * radius * sin(elevation)};
	body.velocity = (t_vector){0.0, 0.0, 0.0};
	body.mass = rand_double(10000);
	return (body);
}

t_body			*create_bodies(size_t num_bodies, int mag)
{
	t_body	*bodies;

	bodies = (t_body*)malloc(sizeof(t_body) * num_bodies);
	for (size_t i = 0; i < num_bodies; i++)
	{
		bodies[i] = rand_body(mag);
		bodies[i].id = i;
	}
	return (bodies);
}

int				main(int argc, char **argv)
{
	char		*buf;
	t_body		*bodies;
	t_octant	*root;

	if (argc != 3)
		exit(1);
	buf = malloc(0xFFFFFFFF); //reserve 4 gb of memory
	if (!buf)
	{
		printf("ERROR NOT ENOUGH MEMORY ON SYSTEM\n");
		return (1);
	}
	free(buf);
	bodies = create_bodies(atoi(argv[1]), 7);
	root = (t_octant*)malloc(sizeof(t_octant) * 1);
	setnode(root, bodies, atoi(argv[1]));
	octree_divide(root);
	simulation(atoi(argv[2]), root);
	return (0);
}
