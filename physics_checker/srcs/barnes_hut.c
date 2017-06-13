/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   barnes_hut.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/16 23:40:25 by elee              #+#    #+#             */
/*   Updated: 2017/05/31 16:22:22 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "checker.h"
#include <stdio.h>
#include <unistd.h>

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
		i++;
	}
	/*
	**compare against all other nodes
	*/
	while (1)
	{
		node = node->parent;
		if (node->parent == NULL)
			break ;
		i = 0;
		while (i < 8)
		{
			if (node->parent->children[i] != node)
			{
				if (node->parent->children[i]->end < node->parent->children[i]->start)
				{
					adjustvelocity_nodes(&newuniverse, prtc, node->parent->children[i]);
				}
			}
			i++;
		}
	}
}

t_octant	*barnes_hut(t_octant *universe)
{
	size_t		i;

	i = universe->start;
	if (!universe)
		return (0);
	if (!universe->bodies)
		return (0);
	while (i <= universe->end)
	{
		bh(endtree(universe, i), universe, i);
		i++;
	}
	i = universe->start;
	while (i <= universe->end)
	{
		adjustposition(&universe, i);
		i++;
	}
	return (universe);
}

t_octant	*brute_force(t_octant *universe)
{
	size_t		i;
	size_t		j;

	i = universe->start;
	j = i;
	if (!universe)
		return (0);
	if (!universe->bodies)
		return (0);
	while (i <= universe->end)
	{
		j = universe->start;
		while (j <= universe->end)
		{
			if (j != i)
			{
				adjustvelocity(&universe, i, universe->bodies[j]);
			}
			j++;
		}
		i++;
	}
	i = universe->start;
	while (i <= universe->end)
	{
		adjustposition(&universe, i);
		i++;
	}
	return (universe);
}
