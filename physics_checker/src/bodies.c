/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bodies.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 16:48:43 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/04 13:30:52 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <checker.h>

double		finddist(t_vector pos)
{
	double	ret;

	ret = sqrt((pos.x * pos.x) + (pos.y + pos.y) + (pos.z * pos.z));
	return (ret);
}

int			compare_positions(const void *b1, const void *b2)
{
	const t_body	*a;
	const t_body	*b;
	double			adist;
	double			bdist;

	a = (const t_body *)b1;
	b = (const t_body *)b2;
	adist = finddist(a->position);
	bdist = finddist(b->position);
	if (adist < bdist)
		return (-1);
	if (adist == bdist)
		return (0);
	return (1);
}

t_body		*sortbodies(t_body *bodies)
{
	qsort(bodies, array_len(bodies, sizeof(t_body)),
			sizeof(t_body), compare_positions);
	return (bodies);
}
