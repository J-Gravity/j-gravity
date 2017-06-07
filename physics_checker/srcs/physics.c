/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/10 19:21:43 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/26 23:39:44 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <checker.h>
#include <stdlib.h>
#include <math.h>

#define M (*newuniverse)->bodies[prtc].mass
#define PARTICLE (*newuniverse)->bodies[prtc]

//pow won't work on windows for whatever inane reason the _exp10 macro isn't recognised.

double		magnitude(t_vector v)
{
	return (sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z)));
}

t_vector	force3d(t_body mass1, t_body mass2)
{
	t_vector	force;
	double		denominator;

	denominator = pow(vectordist(mass1.position, mass2.position), 3);
	force.x = (G * ABS(mass1.mass * mass2.mass))/denominator;
	force.y = (G * ABS(mass1.mass * mass2.mass))/denominator;
	force.z = (G * ABS(mass1.mass * mass2.mass))/denominator;
	force.x *= (mass2.position.x - mass1.position.x);
	force.y *= (mass2.position.y - mass1.position.y);
	force.z *= (mass2.position.z - mass1.position.z);
	if (force.x != force.x || force.y != force.y || force.z != force.z)
		printf("in force calculation, nan as occured\n");
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
		totalmass += sr_mass_increase(node->bodies[i].mass, magnitude(node->bodies[i].velocity));
		i++;
	}
	return (totalmass);
}

t_vector	findcenterofgravity(const t_octant *node, double combinedmass)
{
	t_vector	center;
	size_t		i;

	center = (t_vector){0.0, 0.0, 0.0};
    i = node->start;
    while (i <= node->end)
    {
        center.x += (node->bodies[i].position.x * node->bodies[i].mass);
        center.y += (node->bodies[i].position.y * node->bodies[i].mass);
        center.z += (node->bodies[i].position.z * node->bodies[i].mass);
	//	printf("cog PARTICLE %ld: position: (%lf, %lf, %lf)\n", i, node->bodies[i].position.x,
//									node->bodies[i].position.y,
//									node->bodies[i].position.z);
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
	t_body		temp;
	
	temp = body;
	temp.mass = sr_mass_increase(temp.mass, magnitude(temp.velocity));
	f = force3d(PARTICLE, temp);
	if (f.x != f.x || f.y != f.y || f.z != f.z || PARTICLE.mass != PARTICLE.mass)
		printf("FORCE: NAN\n");
	if (body.mass < 0 && PARTICLE.mass < 0)
	{
		PARTICLE.velocity.x += ((f.x)/(ABS(PARTICLE.mass))) * TIMESTEP;
		PARTICLE.velocity.y += ((f.y)/(ABS(PARTICLE.mass))) * TIMESTEP;
		PARTICLE.velocity.z += ((f.z)/(ABS(PARTICLE.mass))) * TIMESTEP;
	}
	else
	{
		PARTICLE.velocity.x += ((f.x)/(PARTICLE.mass)) * TIMESTEP;
		PARTICLE.velocity.y += ((f.y)/(PARTICLE.mass)) * TIMESTEP;
		PARTICLE.velocity.z += ((f.z)/(PARTICLE.mass)) * TIMESTEP;
	}
	if (PARTICLE.velocity.x != PARTICLE.velocity.x || 
		PARTICLE.velocity.y != PARTICLE.velocity.y ||
		PARTICLE.velocity.z != PARTICLE.velocity.z)
		printf("adjustvelocity: NAN occurred\n");

	//printf("DEBUG PARTICLE: %ld %.20lf\n", prtc, PARTICLE.velocity.x);
}

void		adjustvelocity_nodes(t_octant **newuniverse, size_t prtc, const t_octant *node)
{
	t_vector	f;
	t_body		body;

	body.mass = findtotalmass(node);
	if (body.mass == 0)
		return ;
	body.position = findcenterofgravity(node, body.mass);
	//printf("position.x = %.40lf, mass = %.40lf\n", body.position.x, body.mass);
	f = force3d(PARTICLE, body);
	if (body.mass < 0 && PARTICLE.mass < 0)
	{
		PARTICLE.velocity.x += ((f.x)/(ABS(PARTICLE.mass))) * TIMESTEP;
		PARTICLE.velocity.y += ((f.y)/(ABS(PARTICLE.mass))) * TIMESTEP;
		PARTICLE.velocity.z += ((f.z)/(ABS(PARTICLE.mass))) * TIMESTEP;
	}
	else
	{
		PARTICLE.velocity.x -= ((f.x)/(PARTICLE.mass)) * TIMESTEP;
		PARTICLE.velocity.y -= ((f.y)/(PARTICLE.mass)) * TIMESTEP;
		PARTICLE.velocity.z -= ((f.z)/(PARTICLE.mass)) * TIMESTEP;
	}
	//printf("DEBUG NODE: % ld %.20lf\n", prtc, PARTICLE.velocity.x);
}

void		adjustposition(t_octant **newuniverse, size_t prtc)
{
	PARTICLE.position.x += (PARTICLE.velocity.x);
	PARTICLE.position.y += (PARTICLE.velocity.y);
	PARTICLE.position.z += (PARTICLE.velocity.z);
	if (PARTICLE.position.x != PARTICLE.position.x || 
		PARTICLE.position.y != PARTICLE.position.y ||
		PARTICLE.position.z != PARTICLE.position.z)
		printf("NAN occurred\n");
}
