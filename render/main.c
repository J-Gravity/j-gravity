/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elee <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 18:12:38 by elee              #+#    #+#             */
/*   Updated: 2017/06/12 18:43:28 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int		main(int argc, char **argv)
{
	int		fd;
	long	cnt;
	long	scale;
	float	tmp;
	int	bytes;

	fd = open(argv[1], O_RDONLY);
	read(fd, &cnt, sizeof(long));
	printf("%ld\n", cnt);
	/*
	read(fd, &scale, sizeof(long));
	printf("%ld\n", scale);
	*/
	while ((bytes = read(fd, &tmp, sizeof(float))) > 0)
	{
		printf("%f\n", tmp); 
	}
	close(fd);
}
