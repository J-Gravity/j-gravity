/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   barnes_hut.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 15:00:12 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/08 17:34:09 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <checker.h>

#define UNIVERSE_START(X) universe->children[X]->start
#define UNIVERSE_END(X) universe->children[X]->end

t_octant	*endtree(t_octant *universe, size_t prtc)
{
	size_t	i;

	i = 0;
	while (i < 8)
	{
		if (universe->children == 0)
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

void		bh(t_octant *currentuniverse, t_octant *newuniverse)
{
}

t_octant	*barnes_hut(t_octant *universe)
{
	t_octant	*nextframe;
	size_t		i;

	i = 0;
	while (i < universe->end)
	{
		bh(endtree(universe, i), nextframe); 
		i++;
	}
}
