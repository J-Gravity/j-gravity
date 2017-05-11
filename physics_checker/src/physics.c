/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/10 19:21:43 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/10 20:20:10 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <checker.h>
#include <stdlib.h>

#define M newuniverse->bodies[prtc].mass
#define PARTICLE newuniverse->bodies[prtc]
#define F_S(S) ((G * body.mass * M) / pow(finddist(body.position) - finddist(PARTICLE.position), 3)) * (body.position.S - PARTICLE.position.S)

double		findtotalmass(const t_octant *node)
{
	double	totalmass;
	size_t	i;

	totalmass = 0;
	i = node->start;
	while (i <= node->end)
	{
		totalmass += node->bodies[i].mass;
		i++;
	}
	return (totalmass);
}

t_vector	findcenterofgravity(const t_octant *node)
{
	t_vector	center;

	return (center);
}

void		adjustvelocity(t_octant *newuniverse, size_t prtc, t_body body)
{
	t_vector	f;

	f.x = F_S(x);
	f.y = F_S(y);
	f.z = F_S(z);
	PARTICLE.velocity.x = (f.x)/(PARTICLE.mass);
	PARTICLE.velocity.y = (f.y)/(PARTICLE.mass);
	PARTICLE.velocity.z = (f.z)/(PARTICLE.mass);
}

void		adjustvelocity_nodes(t_octant *newuniverse, size_t prtc, const t_octant *node)
{
	t_vector	f;
	t_body		body;

	body.mass = findtotalmass(node);
	body.position = findcenterofgravity(node);
	f.x = F_S(x);
	f.y = F_S(y);
	f.z = F_S(z);
	PARTICLE.velocity.x = (f.x)/(PARTICLE.mass);
	PARTICLE.velocity.y = (f.y)/(PARTICLE.mass);
	PARTICLE.velocity.z = (f.z)/(PARTICLE.mass);
}
