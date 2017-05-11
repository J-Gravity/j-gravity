/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/10 19:21:43 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/10 21:00:46 by elee             ###   ########.fr       */
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
    double      combinedmass;

    center.x = 0.0;
    center.y = 0.0;
    center.z = 0.0;
    combinedmass = 0.0;
    i = node->start;
    while (i <= node->end)
    {
        center.x += node->bodies[i].position.x * node->bodies[i].mass;
        center.y += node->bodies[i].position.y * node->bodies[i].mass;
        center.z += node->bodies[i].position.z * node->bodies[i].mass;
        combinedmass += bodies[i].mass;
    }
    center.x /= combinedmass;
    center.y /= combinedmass;
    center.z /= combinedmass;
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
