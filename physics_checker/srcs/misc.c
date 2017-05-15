/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 17:23:29 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/03 17:25:57 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <checker.h>
#include <stdlib.h>

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
