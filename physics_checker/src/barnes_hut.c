/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   barnes_hut.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 15:00:12 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/10 19:11:50 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <checker.h>
#include <pthread.h>
#include <stdio.h>

#define UNIVERSE_START(X) universe->children[X]->start
#define UNIVERSE_END(X) universe->children[X]->end

char	threadcount;

t_octant	*endtree(t_octant *universe, size_t prtc)
{
	size_t	i;

	i = 0;
	while (i < 8)
	{
		if (universe->children[0] == 0)
			break ;
		if (prtc <= UNIVERSE_END(i) && prtc >= UNIVERSE_START(i))
		{
			universe = universe->children[i];
			i = 0;
		}
		else
			i++;
	}
	return (universe);
}

#define PARTICLE newuniverse->bodies[prtc]

void		bh(t_octant *node, t_octant *newuniverse, size_t prtc)
{
	size_t	i;


	i = node->parent->start;
	//compare against all particles in current node and neighbor nodes
	while (i <= node->parent->end)
	{
		if (i == prtc)
			continue ;
		adjustvelocity(newuniverse, prtc, node->parent->bodies[i]);
		i++;
	}
	//compare against all other nodes
	while (1)
	{
		node = node->parent;
		i = 0;
		if (node == 0)
			break ;
		while (i < 8)
		{
			if (node->parent->children[i] != node)
				adjustvelocity_nodes(newuniverse, prtc, node->parent->children[i]);
			i++;
		}
	}
	adjustposition(newuniverse, prtc); //adjust position based on velocity
	threadcount--;
	pthread_exit(0);
}

t_octant	*barnes_hut(t_octant *universe)
{
	t_octant	*newuniverse;
	size_t		i;
	pthread_t	threadpool[32];
	t_bharg		args;

	i = 0;
	threadcount = 0;
	newuniverse = (t_octant*)malloc(sizeof(t_octant) * 1);
	if (newuniverse == 0)
	{
		dprintf(2, "ERROR NOT ENOUGH MEMORY TO ALLOCATE NEW UNIVERSE IN BARNES_HUT LINE 86\n");
		exit(0);
	}
	args.universe = newuniverse;
	newuniverse->start = universe->start;
	newuniverse->end = universe->end;
	newuniverse->bodies = (t_body*)malloc(sizeof(t_body) * (universe->end + 1));
	if (newuniverse->bodies == 0)
	{
		dprintf(2, "ERROR NOT ENOUGH MEMORY TO ALLOCATE NEW UNIVERSE IN BARNES_HUT LINE 95\n");
		exit(0);
	}
	//if creating a new universe proves to be too memory intensive we can do adjust_position on all the particles in a big loop at the end and not allocate a new universe
	//multithread this
	while (i <= universe->end)
	{
		//for debug
		dprintf(2, "prtc: %p with mass %lf\n", universe->bodies[i], universe->bodies[i].mass);
		newuniverse->bodies[i] = universe->bodies[i];
		if (threadcount < 32)
		{
			dprintf(2, "Thread spinning off\n");
			args.prtc = i;
			args.node = endtree(universe, i);
			pthread_create(&threadpool[threadcount], 0, (void*)bh, (void*)&args);
			i++;
		}
	}
	free(universe);
	pthread_exit(0);
	return (newuniverse);
}
