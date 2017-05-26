/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/10 19:21:43 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/26 15:46:18 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <checker.h>
#include <stdlib.h>

#define M (*newuniverse)->bodies[prtc].mass
#define PARTICLE (*newuniverse)->bodies[prtc]

t_vector	force3d(t_body mass1, t_body mass2)
{
	t_vector	force;
	double		denominator;

	printf("position1: %lf, position2: %lf\n", mass1.position.x, mass2.position.x);
	denominator = pow(vectordist(mass1.position, mass2.position), 3);
	force.x = (G * mass1.mass * mass2.mass)/denominator;
	force.y = (G * mass1.mass * mass2.mass)/denominator;
	force.z = (G * mass1.mass * mass2.mass)/denominator;
	printf("mass2: %.40lf\n", mass2.mass);
	printf("DEBUG %.40lf, %.40lf, %.40lf, %.40lf, %.40lf\n", force.x, denominator, G, mass1.mass, mass2.mass);
	force.x *= (mass2.position.x - mass1.position.x) / ABS(mass2.position.x - mass1.position.x);
	force.y *= (mass2.position.y - mass1.position.y) / ABS(mass2.position.y - mass1.position.y);
	force.z *= (mass2.position.z - mass1.position.z) / ABS(mass2.position.z - mass1.position.z);
	printf("DEBUG %.40lf\n", force.x);
	return (force);
}

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
	size_t		i;

	center = (t_vector){0.0, 0.0, 0.0};
    combinedmass = 0.0;
    i = node->start;
    while (i <= node->end)
    {
        center.x += (node->bodies[i].position.x * node->bodies[i].mass);
        center.y += (node->bodies[i].position.y * node->bodies[i].mass);
        center.z += (node->bodies[i].position.z * node->bodies[i].mass);
	//	printf("cog PARTICLE %ld: position: (%lf, %lf, %lf)\n", i, node->bodies[i].position.x,
//									node->bodies[i].position.y,
//									node->bodies[i].position.z);
        combinedmass += node->bodies[i].mass;
		i++;
    }
	//printf("DEBUG %lf %lf\n", center.x, combinedmass);
    center.x /= combinedmass;
    center.y /= combinedmass;
    center.z /= combinedmass;
	return (center);
}

void		adjustvelocity(t_octant **newuniverse, size_t prtc, t_body body)
{
	t_vector	f;

	f = force3d(PARTICLE, body);
	PARTICLE.velocity.x += ((f.x)/(PARTICLE.mass)) * TIMESTEP;
	PARTICLE.velocity.y += ((f.y)/(PARTICLE.mass)) * TIMESTEP;
	PARTICLE.velocity.z += ((f.z)/(PARTICLE.mass)) * TIMESTEP;
	printf("DEBUG PARTICLE: %ld %.20lf\n", prtc, PARTICLE.velocity.x);
}

void		adjustvelocity_nodes(t_octant **newuniverse, size_t prtc, const t_octant *node)
{
	t_vector	f;
	t_body		body;

	body.mass = findtotalmass(node);
	if (body.mass == 0)
		return ;
	body.position = findcenterofgravity(node);
	printf("position.x = %.40lf, mass = %.40lf\n", body.position.x, body.mass);
	f = force3d(PARTICLE, body);
	PARTICLE.velocity.x += ((f.x)/(PARTICLE.mass)) * TIMESTEP;
	PARTICLE.velocity.y += ((f.y)/(PARTICLE.mass)) * TIMESTEP;
	PARTICLE.velocity.z += ((f.z)/(PARTICLE.mass)) * TIMESTEP;
	printf("DEBUG NODE: % ld %.20lf\n", prtc, PARTICLE.velocity.x);
}

void		adjustposition(t_octant **newuniverse, size_t prtc)
{
	PARTICLE.position.x += (PARTICLE.velocity.x * TIMESTEP);
	PARTICLE.position.y += (PARTICLE.velocity.y * TIMESTEP);
	PARTICLE.position.z += (PARTICLE.velocity.z * TIMESTEP);
}
