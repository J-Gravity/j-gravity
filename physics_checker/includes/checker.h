/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 13:07:49 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/22 13:01:25 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKER_H
# define CHECKER_H

#include <math.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

# define TIMESTEP 100000000.0
# define G 0.000000000066742

# define ABS(x) (x < 0 ? -x : x)
#define UNITVECTOR(X1, X2) (X2 - X1)/ABS(X2 - X1)
#define FORCE3D(S, C2, C1) ((G * body.mass * M) / (pow(DIST(C1, C2), 3))) * UNITVECTOR(C1.S, C2.S)
//FORCE3D is in the form vector component, body being acted upon, source body

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
	t_vector	force;
	char		octant;
}				t_body;

typedef struct		s_node
{
	t_body			*bodies;
	struct s_node	**children;
	struct s_node	*parent;
	size_t			start;
	size_t			end;
}					t_octant;

typedef struct	s_bharg
{
	size_t		prtc;
	t_octant	*node;
	t_octant	*universe;
}				t_bharg;

void			octree_divide(t_octant *root);
void			print_tree(t_octant *root);

size_t			array_len(void *ptr, size_t datasize);
int64_t			parsenbodies(char *buf);

double			finddist(t_vector pos);
double			vectordist(t_vector p1, t_vector p2);
# define DIST(X, Y) vectordist(X, Y) 

int				compare_positions(const void *b1, const void *b2);
t_body			*sortbodies(t_body *bodies);
t_body			*getbodies(int fd, int64_t nbodies);

t_octant		*barnes_hut(t_octant *root);

void			adjustposition(t_octant **newuniverse, size_t index);
void			adjustvelocity(t_octant **newuniverse, size_t index, t_body body);
void			adjustvelocity_nodes(t_octant **newuniverse, size_t index, const t_octant *node);
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
