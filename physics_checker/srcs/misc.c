/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 17:23:29 by smifsud           #+#    #+#             */
/*   Updated: 2017/06/02 13:28:10 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <checker.h>
#include <stdlib.h>
#include <math.h>

double		pow10(double x)
{
	double	i;
	double	num;
	
	i = 0;
	num = 1;
	while (i < x)
	{
		num *= 10;
		i++;
	}
	return (num);
}

size_t		array_len(void *ptr, size_t datasize)
{
	size_t	ret;
	char	*arr;

	ret = 0;
	arr = (char*)ptr;
	while (*arr)
	{
		arr += datasize;
		ret++;
	}
	return (ret);
}

double		vectordist(t_vector p1, t_vector p2)
{
	double	x = pow(p2.x - p1.x, 2);
	double	y = pow(p2.y - p1.y, 2);
	double	z = pow(p2.z - p1.z, 2);
	return (sqrt(x + y + z));
}
