/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/04 13:31:03 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/22 13:12:13 by smifsud          ###   ########.fr       */
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

void			outresults(t_octant *universe, int filen)
{
	char		*filename;
	int			fd;
	int64_t		*buf;
	double		*cords;
	size_t		i;
	size_t		j;

	i = 0;
	j = 0;
	asprintf(&filename, "/output/out%d.jgrav", filen);
	fd = open(filename, O_CREAT, O_WRONLY, O_APPEND);
	buf = malloc(sizeof(int64_t) * 1);
	*buf = universe->end - universe->start + 1;
	write(fd, buf, sizeof(int64_t));
	*buf = (int64_t)5.2;
	cords = (double*)malloc(sizeof(double) * 3 * (universe->end - universe->start + 1));
	if (!cords)
	{
		printf("ALLOC ERROR\n");
		return ;
	}
	while (i <= universe->end)
	{
		cords[j] = universe->bodies[i].position.x;
		j++;
		cords[j] = universe->bodies[i].position.y;
		j++;
		cords[j] = universe->bodies[i].position.z;
		j++;
		i++;
	}
	write(fd, cords, sizeof(double) * 3 * (universe->end - universe->start + 1));
	printf("DEBUG\n");
	if (cords != 0)
	{
		free(cords);
	}
	printf("DEBUG\n");
	free(filename);
	printf("DEBUG\n");
	free(buf);
}

void				simulation(int o, t_octant *universe)
{
	int		i;
	
	i = 0;
	while (i < o)
	{
		universe = barnes_hut(universe);
		outresults(universe, i);
		//printf("%zu, %zu\n", universe->start, universe->end);
		for (size_t j = universe->start; j <= universe->end; j++)
		{
			printf("PARTICLE ID %zu:\nposition = (%lf, %lf, %lf)\nvelocity = (%lf, %lf, %lf)\nmass = (%lf)\n", 
			j,
			universe->bodies[j].position.x,
			universe->bodies[j].position.y,
			universe->bodies[j].position.z,
			universe->bodies[j].velocity.x,
			universe->bodies[j].velocity.y,
			universe->bodies[j].velocity.z,
			universe->bodies[j].mass);
		}
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
	t_body		body;
	double		elevation;
	double		azimuth;
	double		radius;
	t_vector	velocity;

	elevation = asin(rand_double(2) - 1);
	azimuth = 2 * M_PI * rand_double(1);
	radius = cbrt(rand_double(1)) * __exp10(mag);
	body.position = (t_vector){radius * cos(elevation) * cos(azimuth), \
								radius * cos(elevation) * sin(azimuth),
								0.3 * radius * sin(elevation)};
	velocity.x = 1;
	velocity.y = 0;
	velocity.z = rand_double(2);
	body.velocity = velocity;
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
	printf("wokka\n");
	free(buf);
	bodies = create_bodies(atoi(argv[1]), 7);
	root = (t_octant*)malloc(sizeof(t_octant) * 1);
	setnode(root, bodies, atoi(argv[1]));
	octree_divide(root);
	print_tree(root);
	simulation(atoi(argv[2]), root);
	return (0);
}
