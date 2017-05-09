/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 14:21:02 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/08 19:48:05 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <checker.h>
#include <stdlib.h>

void		createchildren(t_octant *node)
{
	int		i;

	i = 8;
	while (i < 8)
	{
		node->children[i] = (t_octant*)malloc(sizeof(t_octant) * 1);
		node->children[i]->parent = node;
		node->children[i]->bodies = node->bodies;
		node->children[i]->children = (t_octant**)malloc(sizeof(t_octant*) * 1);
		node->children[i]->children[i] = 0;
		i++;
	}
}

/*while (i < 8)
  {
  root->children[i]->start = (root->end / 8) * i;
  root->children[i]->end = (root->end / 8) * (i + 1) - 1; //end to start for 0 should be 0 to 1/8end -1
  i++;
  } */ //rounding fucks up this method I think

t_octant	*octree_divide(t_octant *root)
{
	int			i;
	size_t		end;
	size_t		start;

	i = 0;
	start = 0;
	end = root->end / 8;
	createchildren(root);
	while (i < 8)
	{
		root->children[i]->start = start;
		root->children[i]->end = end;
		if (start == 0)
			start += 1;
		start += root->end / 8;
		end += root->end / 8;
		i++;
	}
	root->children[i - 1]->end = root->end;
	while (i > 0)
	{
		octree_divide(root->children[i - 1]);
		i--;
	}
	return (root);
}
