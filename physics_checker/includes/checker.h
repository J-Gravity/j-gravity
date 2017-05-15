/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 13:07:49 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/10 20:20:21 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKER_H
# define CHECKER_H

#include <math.h>

# define TIMESTEP 1;
# define G 0.000000000066742

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
	char		octant;
}				t_body;

typedef struct	s_node
{
	t_body		*bodies;
	s_node		**children;
	s_node		*parent;
	size_t		start;
	size_t		end;
	char		corner;
	t_vector	potential;
}				t_octant;

typedef struct	s_bharg
{
	size_t		prtc;
	t_octant	*node;
	t_octant	*universe;
}				t_bharg;

t_octant		*octree_divide(t_octant *root);

size_t			array_len(void *ptr, size_t datasize);
int64_t			parsenbodies(char *buf);

double			finddist(t_vector pos);
# define DIST(X, Y) finddist(x) - finddist(y)

int				compare_positions(const void *b1, const void *b2);
t_body			*sortbodies(t_body *bodies);
t_body			*getbodies(int fd, int64_t nbodies);

t_octant		*barnes_hut(t_octant *root);

void			adjustposition(t_octant *newuniverse, size_t index);
void			adjustvelocity(t_octant *newuniverse, size_t index, t_body body);
void			adjustvelocity_nodes(t_octant *newuniverse, size_t index, const t_octant *node);
double			findtotalmass(const t_octant *node);
t_vector		findcenterofgravity(const t_octant *node);

#endif


/*
 *
 * |__________________|
 * |______||__________|
 * |__||__|
 *
 */
