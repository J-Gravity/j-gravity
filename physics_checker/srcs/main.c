/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/04 13:31:03 by smifsud           #+#    #+#             */
/*   Updated: 2017/06/12 18:38:09 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <stdlib.h>
#include <fcntl.h>
#include <checker.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
void			setnode(t_octant *node, t_body *bodies, int64_t nbodies)
{
    node->parent = 0;
    node->bodies = bodies;
    node->start = 0;
    node->end = (size_t)nbodies - 1;
    node->children = (t_octant**)malloc(sizeof(t_octant*) * 8);
}

t_body          *copybodies(t_body *bodies, size_t len)
{
    t_body  *new;
	size_t i;

	i = 0;

    new = (t_body*)malloc(sizeof(t_body) * len);
    while (i < len)
    {
        new[i].position = bodies[i].position;
        new[i].velocity = bodies[i].velocity;
        new[i].id = bodies[i].id;
        new[i].mass = bodies[i].mass;
        new[i].octant = bodies[i].octant;
        i++;
    }
    return (new);
}

int         compare_id(const void *b1, const void *b2)
{
    const t_body    *a; 
    const t_body    *b; 

    a = (const t_body *)b1;
    b = (const t_body *)b2;
    if (a->id < b->id)
        return (-1);
    if (a->id == b->id)
        return (0);
    return (1);
}

void			outresults(t_octant *universe, int filen)
{
    char		*filename;
    int			fd;
    int64_t		*buf;
    double		*cords;
    size_t		i;
    size_t		j;
    t_body      *new;

    i = 0;
    j = 0;
    printf("THIS IS A DEBUG MESSAGE\n");
    asprintf(&filename, "out%d.jgrav", filen);
    fd = open(filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH | S_IRGRP | S_IWGRP);
    buf = malloc(sizeof(int64_t) * 1);
    *buf = universe->end - universe->start + 1;
    write(fd, buf, sizeof(int64_t));
    cords = (double*)malloc(sizeof(double) * 4 * (universe->end - universe->start + 1));
    if (!cords)
    {
        printf("ALLOC ERROR\n");
        exit(1);
    }
    new = copybodies(universe->bodies, (universe->end - universe->start + 1));
    qsort(&new[0], (universe->end - universe->start + 1), sizeof(t_body), compare_id);
    while (i <= universe->end)
    {
        cords[j] = new[i].position.x;
        j++;
        cords[j] = new[i].position.y;
        j++;
        cords[j] = new[i].position.z;
        j++;
        cords[j] = new[i].mass;
        j++;
        //	if (i == 71523 || i == 71522 || i == 71524)
        //			printf("CORDS: x: %lf, y: %lf, z: %lf, m: %lf\n", cords[j - 4], cords[j - 3], cords[j - 2], cords[j - 1]);
        i++;
    }
    printf("DEBUG SIZE%ld\n", universe->end - universe->start + 1);
    write(fd, cords, sizeof(double) * 4 * (universe->end - universe->start + 1));
    free(cords);
    free(filename);
    free(buf);
    free(new);
}

void				simulation(t_octant *universe)
{
    int		i;

    i = 0;
    while (i < 50000)
    {
        printf("OK %d\n", i);
    	universe = barnes_hut(universe);
		//universe = brute_force(universe);
        if (i % 10000 == 0)
	{
		outresults(universe, i);
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

t_body			makebody(t_invector inpos)
{
    t_body		newbody;

    newbody.mass = inpos.mass;
    newbody.position.x = inpos.x;
    newbody.position.y = inpos.y;
    newbody.position.z = inpos.z;
    newbody.velocity.x = inpos.xvel;
    newbody.velocity.x = inpos.yvel;
    newbody.velocity.z = inpos.zvel;
    return (newbody);
}

t_body			*read_bodies(char *positions, size_t *nbodies)
{
    int			posfd = open(positions, O_RDONLY);
    t_body		*ret;
    size_t		i;
    t_invector	inpos;

    read(posfd, nbodies, sizeof(int64_t));
    printf("nbodies %ld\n", *nbodies);
    ret = (t_body*)malloc(sizeof(t_body) * (*nbodies + 1));
    read(posfd, &i, sizeof(int64_t));
    i = 0;
    while (i <= *nbodies)
    {
        read(posfd, &inpos, sizeof(t_invector));
        ret[i] = makebody(inpos);
        ret[i].id = i;
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

    (void)argc;
    srand(time(NULL));
    buf = malloc(0xFFFFFFFF); //reserve 4 gb of memory
    if (!buf)
    {
        printf("ERROR NOT ENOUGH MEMORY ON SYSTEM\n");
        return (1);
    }
    printf("wokka\n");
    free(buf);
    bodies = read_bodies(argv[1], &nbodies);
    root = (t_octant*)malloc(sizeof(t_octant) * 1);
    setnode(root, bodies, nbodies);
    octree_divide(root);
    //print_tree(root);
    simulation(root);
    return (0);
}
