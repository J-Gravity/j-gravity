/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/04 13:31:03 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/27 18:25:08 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <checker.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

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
	asprintf(&filename, "out%d.jgrav", filen);
	fd = open(filename, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH | S_IRGRP | S_IWGRP);
	buf = malloc(sizeof(int64_t) * 1);
	*buf = universe->end - universe->start + 1;
	write(fd, buf, sizeof(int64_t));
	*buf = (int64_t)5.2;
	cords = (double*)malloc(sizeof(double) * 3 * (universe->end - universe->start + 1));
	if (!cords)
	{
		printf("ALLOC ERROR\n");
		exit(1);
	}
	while (i <= universe->end)
	{
		cords[j] = universe->bodies[i].position.x;
		printf("CORDS: %lf\n", cords[j]);
		j++;
		cords[j] = universe->bodies[i].position.y;
		j++;
		cords[j] = universe->bodies[i].position.z;
		j++;
		i++;
	}
	printf("DEBUG WOKKA%ld\n", universe->end - universe->start + 1);
	write(fd, cords, sizeof(double) * 3 * (universe->end - universe->start + 1));
	if (cords != 0)
	{
		free(cords);
	}
	free(filename);
	free(buf);
}

void				simulation(t_octant *universe)
{
	int		i;
	
	i = 0;
	while (i < 1)
	{
	/*	for (size_t j = universe->start; j <= universe->end; j++)
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
		} */

		universe = barnes_hut(universe);
		outresults(universe, i);
		//slowoutput(universe, i);
		//printf("%zu, %zu\n", universe->start, universe->end);
/*		for (size_t j = universe->start; j <= universe->end; j++)
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
		} */
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
	body.mass = 10000000000000000.0;
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

t_body			makebody(t_invector inpos, t_invector invel)
{
	t_body		newbody;

	mpf_init2(newbody.position.x, 512);
	mpf_init2(newbody.position.y, 512);
	mpf_init2(newbody.position.z, 512);
	mpf_init2(newbody.velocity.x, 512);
	mpf_init2(newbody.velocity.y, 512);
	mpf_init2(newbody.velocity.z, 512);
	mpf_init2(newbody.mass, 512);
	mp
	newbody.position.x = inpos.x;
	newbody.position.y = inpos.y;
	newbody.position.z = inpos.z;
	newbody.velocity.x = invel.x;
	newbody.velocity.x = invel.y;
	newbody.velocity.z = invel.z;
	return (newbody);
}

t_body			*read_bodies(char *positions, char *velocities, size_t *nbodies)
{
	int			posfd = open(positions, O_RDONLY);
	int			velfd = open(velocities, O_RDONLY);
	t_body		*ret;
	size_t		i;
	t_invector	inpos;
	t_invector	invel;
	
	i = 0;
	read(posfd, nbodies, sizeof(int64_t));
	printf("nbodies %ld\n", *nbodies);
	ret = (t_body*)malloc(sizeof(t_body) * (*nbodies + 1));
	while (i <= *nbodies)
	{
		read(posfd, &inpos, sizeof(t_invector));
		read(velfd, &invel, sizeof(t_invector));
		ret[i] = makebody(inpos, invel);
		i++;
	}
	return (ret);
}

int				main(int argc, char **argv)
{
	char		*buf;
	t_body		*bodies;
	t_octant	*root;
	size_t		nbodies;

	srand(time(NULL));
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
	bodies = read_bodies(argv[1], argv[2], &nbodies);
	root = (t_octant*)malloc(sizeof(t_octant) * 1);
	setnode(root, bodies, nbodies);
	octree_divide(root);
//	print_tree(root);
	simulation(root);
	return (0);
}
