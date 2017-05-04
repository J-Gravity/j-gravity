/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bodies.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 16:48:43 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/03 17:48:25 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <checker.h>

int		compare_positions(const void *a, const void *b)
{
	const t_body	*b1;
	const t_body	*b2;

	b1 = (const t_body *)a;
	b2 = (const t_body *)b;

}

t_body	*sortbodies(t_body *bodies)
{
	qsort(bodies, array_len(bodies, sizeof(t_body)), sizeof(t_body), compare_positions());
	return (bodies);
}

