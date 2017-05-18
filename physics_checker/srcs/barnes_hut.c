/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   barnes_hut.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/16 23:40:25 by elee              #+#    #+#             */
/*   Updated: 2017/05/17 17:16:17 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "checker.h"
#include <stdio.h>

#define UNIVERSE_START(X) universe->children[X]->start
#define UNIVERSE_END(X) universe->children[X]->end

t_octant	*endtree(t_octant *universe, size_t prtc)
{
	size_t	i;

	i = 0;
	while (i < 8)
	{
		if (universe->children[0]->end == 0)
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
	size_t		i;

	printf("beginning: [%zu, %zu]\n", node->start, node->end);
	/*
	**compare against all particles in current node and neighbor nodes
	*/
	i = node->parent->start;
	while (i <= node->parent->end)
	{
		if (i == prtc)
		{
			i++;
			continue ;
		}
		adjustvelocity(&newuniverse, prtc, node->parent->bodies[i]);
		/*
		dprintf(2, "(%lf, %lf, %lf)\n", node->parent->bodies[i].velocity.x,
									node->parent->bodies[i].velocity.y,
									node->parent->bodies[i].velocity.z);
									*/
		i++;
	}
	/*
	**compare against all other nodes
	*/
	while (1)
	{
		node = node->parent;
		if (node == NULL)
			break ;
		i = 0;
		printf("this is the node: %p\n", node);
		while (i < 8)
		{
			printf("boundary: [%zu, %zu]\n", node->start, node->end);
			printf("words %p\n", node->parent->children);
//			printf("DEBUG %ld, %p\n", i, node->parent->children[i]);
			if (node->parent->children[i] != node)
				adjustvelocity_nodes(&newuniverse, prtc, node->parent->children[i]);
			i++;
		}
		printf("abc\n");
	}
	printf("why\n");
	adjustposition(&newuniverse, prtc);
}

void		barnes_hut(t_octant *universe)
{
	size_t		i;

	i = 0;
	if (!universe)
		return ;
	if (!universe->bodies)
		return ;
	while (i <= universe->end)
	{
		bh(endtree(universe, i), universe, i);
		i++;
	}
}
