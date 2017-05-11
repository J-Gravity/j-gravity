/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   octree.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elee <elee@student.42.us.org>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 19:46:04 by elee              #+#    #+#             */
/*   Updated: 2017/05/10 20:57:11 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker.h"
#include <stdlib.h>

void        createchildren(t_octant *node)
{
    int     i;

    i = 0;
    while (i < 8)
    {
        node->children[i] = (t_octant*)malloc(sizeof(t_octant));
        node->children[i]->parent = node;
        node->children[i]->bodies = node->bodies;
        node->children[i]->children = (t_octant**)malloc(sizeof(t_octant) * 8);
		node->children[i]->children[0] = 0;
        i++;
    }
}

void        set_center_of_mass(t_octant *node)
{
    size_t      i;
    t_vector    com;
	t_vector	com2;
	t_vector	center;

    com.x = 0.0;
    com.y = 0.0;
    com.z = 0.0;
	com2.x = 0.0;
	com2.y = 0.0;
	com2.z = 0.0;
    node->combinedmass = 0.0;
    i = node->start;
    while (i <= node->end)
    {
        com.x += node->bodies[i].position.x * node->bodies[i].mass;
        com.y += node->bodies[i].position.y * node->bodies[i].mass;
        com.z += node->bodies[i].position.z * node->bodies[i].mass;
		com2.x += node->bodies[i].position.x;
		com2.y += node->bodies[i].position.y;
		com2.z += node->bodies[i].position.z;
        node->combinedmass += bodies[i].mass;
        i++;
    }
    node->centerofmass.x = com.x / node->combinedmass;
    node->centerofmass.y = com.y / node->combinedmass;
    node->centerofmass.z = com.z / node->combinedmass;
	node->center.x = com2.x / (node->end - node->start + 1);
	node->center.y = com2.y / (node->end - node->start + 1);
	node->center.z = com2.z / (node->end - node->start + 1);
}

void        find_octant(t_octant *cell)
{
    size_t  i;
    double  x;
    double  y;
    double  z;
    
    i = cell->start;
    while (i <= cell->end)
    {
        x = cell->bodies[i].position.x - cell->center.x;
        y = cell->bodies[i].position.y - cell->center.y;
        z = cell->bodies[i].position.z - cell->center.z;
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

    start = cell->start;
    i = 0;
    while (i < 8)
    {
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
        }
        i++;
    }
}

t_octant    *octree_divide(t_octant *root)
{
    int i;

    if (root->end < root->start)
        return (NULL);
    set_center_of_mass(root);
    createChildren(root);
    find_octant(root);
    qsort(root->bodies + (root->start) * sizeof(t_body), (root->end - root->start + 1), sizeof(t_body), compare_octant);
    set_startend(root);
    i = 0;
    while (i < 8)
    {
        octree_divide(root->children[i]);
        i++;
    }
    return (root);
}
