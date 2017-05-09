/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 13:07:49 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/08 18:59:25 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKER_H
# define CHECKER_H

#include <math.h>

typedef struct	s_vector
{
	double		x;
	double		y;
	double		z;
}				t_vector;

typedef struct	s_body
{
	double		mass;
	t_vector	position;
	t_vector	velocity;
}				t_body;

typedef struct	s_node
{
	t_body		*bodies;
	s_node		**children;
	s_node		*parent;
	int			anglesection;
	t_vector	centerofmass;
	double		combinedmass;
	size_t		start;
	size_t		end;
}				t_octant;

t_octant		*octree_divide(t_octant *root);

size_t			array_len(void *ptr, size_t datasize);
int64_t			parsenbodies(char *buf);

double			finddist(t_vector pos);

int				compare_positions(const void *b1, const void *b2);
t_body			*sortbodies(t_body *bodies);
t_body			*getbodies(int fd, int64_t nbodies);

t_octant		*barnes_hut(t_octant *root);

void			adjustposition(t_octant *newuniverse, size_t index);
void			adjustvelocity(t_octant *newuniverse, size_t index, t_body body);
void			adjustvelocity_nodes(t_octant *newuniverse, size_t index, t_octant *node);

#endif


/*
 *
 * |__________________|
 * |______||__________|
 * |__||__|
 *
 */
