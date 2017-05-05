/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/04 13:31:03 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/04 14:33:43 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include <checker.h>
#include <unistd.h>
#include <stdio.h>

void			setnode(t_octant *node, t_body *bodies, int64_t nbodies)
{
	node->parent = 0;
	node->bodies = bodies;
	node->start = 0;
	node->end = (size_t)nbodies;
	node->children = (t_octant**)malloc(sizeof(t_octant*) * 1);
}

void				simulation(int o, t_octant *universe)
{
	int		i;
	char	*buf;
	
	i = 0;
	while (i < o)
	{
		buf = barnes_hut(universe);
		write_out(buf); // not happy with this, uses too much memory for our output. Perhaps this needs to be done in smaller steps inside barnes-hut
		free(buf);
	}
}

int				main(int argc, char **argv)
{
	int			fd;
	char		*buf;
	t_body		*bodies;
	t_octant	*root;

	buf = malloc(0xFFFFFFFF); //reserve 4 gb of memory
	if (!buf)
	{
		printf("ERROR NOT ENOUGH MEMORY ON SYSTEM\n");
		return (1);
	}
	free(buf);
	fd = open(argv[1], O_RDONLY);
	bodies = (sortbodies(getbodies(fd,
					parsenbodies(buf))));
	root = (t_octant*)malloc(sizeof(t_octant) * 1);
	setnode(root, bodies, parsenbodies(buf));
	free(buf);
	root = octree_divide(root);
	simulation(atoi(argv[2]), root);
	/*buf = barnes_hut(root);
	write_out(buf);*/
	return (0);
}
