/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elee <elee@student.42.us.org>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 19:46:04 by elee              #+#    #+#             */
/*   Updated: 2017/05/28 17:58:30 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker.h"
#include <stdlib.h>
#include <stdio.h>

void        createchildren(t_octant *node)
{
    int     i;

    i = 0;
    while (i < 8)
    {
        node->children[i] = (t_octant*)malloc(sizeof(t_octant));
		if (node->children[i] == 0)
		{
			dprintf(2, "MALLOC ERROR FOR OCTANT DIVIDE\n");
			exit(0);
		}
        node->children[i]->parent = node;
        node->children[i]->bodies = node->bodies;
        node->children[i]->children = (t_octant**)malloc(sizeof(t_octant) * 8);
		node->children[i]->end = 0;
        i++;
    }
}

t_vector	get_physical_center(t_octant *node)
{
    size_t      i;
	t_vector	center;

	center.x = 0.0;
	center.y = 0.0;
	center.z = 0.0;
    i = node->start;
    while (i <= node->end)
    {
		center.x += node->bodies[i].position.x;
		center.y += node->bodies[i].position.y;
		center.z += node->bodies[i].position.z;
        i++;
    }
	center.x /= (node->end - node->start + 1);
	center.y /= (node->end - node->start + 1);
	center.z /= (node->end - node->start + 1);
	return (center);
}

void        find_octant(t_octant *cell)
{
	t_vector	center;
    size_t  	i;
    double  	x;
    double  	y;
    double  	z;
   
	center = get_physical_center(cell);
    i = cell->start;
    while (i <= cell->end)
    {
        x = cell->bodies[i].position.x - center.x;
        y = cell->bodies[i].position.y - center.y;
        z = cell->bodies[i].position.z - center.z;
        if (x >= 0.0 && y >= 0.0 && z >= 0.0)
            cell->bodies[i].octant = 1;
        else if (x < 0.0 && y >= 0.0 && z >= 0.0)
            cell->bodies[i].octant = 2;
        else if (x < 0.0 && y < 0.0 && z >= 0.0)
            cell->bodies[i].octant = 3;
        else if (x >= 0.0 && y < 0.0 && z >= 0.0)
            cell->bodies[i].octant = 4;
        else if (x >= 0.0 && y >= 0.0 && z < 0.0)
            cell->bodies[i].octant = 5;
        else if (x < 0.0 && y >= 0.0 && z < 0.0)
            cell->bodies[i].octant = 6;
        else if (x < 0.0 && y < 0.0 && z < 0.0)
            cell->bodies[i].octant = 7;
        else if (x >= 0.0 && y < 0.0 && z < 0.0)
            cell->bodies[i].octant = 8;
        i++;
    }
}

int         compare_octant(const void *b1, const void *b2)
{
    const t_body    *a;
    const t_body    *b;

    a = (const t_body *)b1;
    b = (const t_body *)b2;
    if (a->octant < b->octant)
        return (-1);
    if (a->octant == b->octant)
        return (0);
    return (1);
}

size_t      find_start(t_octant *cell, size_t start, int octant)
{
    size_t  i;

    i = start;
    if (cell->bodies[i].octant > octant)
        return (ULONG_MAX);
    while (i <= cell->end)
    {
        if (cell->bodies[i].octant == octant)
            return (i);
        i++;
    }
    return (ULONG_MAX);
}

size_t      find_end(t_octant *cell, size_t start, int octant)
{
    size_t  i;

    i = start;
    while (i <= cell->end)
    {
        if (cell->bodies[i].octant != octant)
            break ;
        i++;
    }
    return (i - 1);
}

void        set_startend(t_octant *cell)
{
    int     i;
    size_t  start;
	size_t	tmp;

    start = cell->start;
    i = 0;
    while (i < 8)
    {
		tmp = start;
        start = find_start(cell, start, i + 1);
        if (start != ULONG_MAX)
        {
            cell->children[i]->start = start;
            cell->children[i]->end = find_end(cell, start, i + 1);
        }
        else
        {
            cell->children[i]->start = 1;
            cell->children[i]->end = 0;
			start = tmp;
        }
        i++;
    }
}

size_t		getdepth(t_octant *root)
{
	size_t	depth;
	
	depth = 0;
	while (root->parent)
	{
		root = root->parent;
		depth++;
	}
	return (depth);
}

void		octree_divide(t_octant *root)
{
	char	i;

    if (root->end < root->start)
		return ;
	if (root->end == root->start)
	{
		createchildren(root);
		return ;
	}
	/*
	if (50 == getdepth(root))
		return ;
		*/
	//printf("nodep %p\n", root);
    createchildren(root);
    find_octant(root);
    qsort(&(root->bodies[root->start]), (root->end - root->start + 1), sizeof(t_body), compare_octant);
    set_startend(root);	
    i = 0;
    while (i < 8)
    {
        octree_divide(root->children[(int)i]);
        i++;
    }
}

void		print_tree(t_octant *root)
{
	int	i;

	if (root->end < root->start)
	{
		printf("address: %p                                    child of %p\n", root, root->parent);
		return ;
	}
	if (root->end == root->start)
	{
	printf("address: %p, boundary: [%zu, %zu], num_bodies: %zu child of %p\n",
			root, root->start, root->end, root->end - root->start + 1, root->parent);
		return ;
	}
	printf("address: %p, boundary: [%zu, %zu], num_bodies: %zu child of %p\n",
			root, root->start, root->end, root->end - root->start + 1, root->parent);
	i = 0;
	while (i < 8)
	{
		print_tree(root->children[i]);
		i++;
	}
}

void	re_tree(t_octant *root)
{
	int		i;

	if (root == 0)
		return ;
	if (root->end < root->start)
		return ;
	i = 0;
	while (i < 0)
	{
		re_tree(root->children[i]);
	}
	if (root->parent != 0)
	{
		free(root->children);
		free(root);
	}
}
