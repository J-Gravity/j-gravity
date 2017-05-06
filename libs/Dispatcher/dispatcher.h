/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <cyildiri@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 19:43:37 by cyildiri          #+#    #+#             */
/*   Updated: 2017/05/05 20:51:42 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPATCHER_H
# define DISPATCHER_H

typedef struct	s_vect3f
{
	float	x;
	float	y;
	float	z;
}				t_vect3f;

typedef struct	s_body
{
	t_vect3f	position;
	t_vect3f	velocity;
	float		mass;
}				t_body;

typedef struct	s_cell
{
	t_body		**contained_bodies;
	int			body_count;
	t_cell		*adjoining_cells;
	int			adjoining_cells_cnt;
	t_body		cell_as_body;
}				t_cell;

typedef struct	s_worker
{
	t_cell		*acitve_cells;
	int			cell_cnt;
	char		*compute_class;
	int			socket;
}				t_worker;

typedef struct	s_dispatcher
{
	t_lst		*workers;
	int			worker_cnt;
	t_dataset	*data;
	t_lst		*cells;
	int			cell_cnt;
	int			unassigned_cells;
	int			server_fd;
	t_lst		worker_connections;
}				t_dispatcher;

/*
*	Sets up TCP connections with workers
*	@param	dispatcher	The dispatcher main struct
*/
void	connect_workers(t_dispatcher *dispatcher);

#endif
