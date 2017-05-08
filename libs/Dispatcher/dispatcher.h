/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <cyildiri@student.42.us.org>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 19:43:37 by cyildiri          #+#    #+#             */
/*   Updated: 2017/05/05 21:22:28 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPATCHER_H
# define DISPATCHER_H

typedef struct			s_vect3f
{
	float				x;
	float				y;
	float				z;
}						t_vect3f;

typedef	struct			s_socket
{
	int					server_socket;
	struct sockaddr_in	*addr;
	size_t				*addrlen;
}						t_socket;

typedef struct			s_body
{
	t_vect3f			position;
	t_vect3f			velocity;
	float				mass;
}						t_body;

typedef struct			s_cell
{
	t_body				**contained_bodies;
	int					body_count;
	t_body				cell_as_body;
}						t_cell;

typedef struct			s_work_unit
{
	t_cell				cell;
	t_cell				*adjoining_cells;
	int					adjoining_cells_cnt;
	char				*compute_class;
}						t_work_unit;

typedef struct			s_worker
{
	t_cell				*acitve_cells;
	int					cell_cnt;
	char				*compute_class;
	int					socket;
}						t_worker;

typedef struct			s_dispatcher
{
	t_lst				*workers;
	int					worker_cnt;
	t_dataset			*data;
	t_lst				*cells;
	int					cell_cnt;
	int					unassigned_cells;
	int					server_fd;
	t_lst				worker_connections;
}						t_dispatcher;

/*
*	Setup a server socket endpoint
*		@param	port	the port the server will run on
*		@return	returns a struct t_socket that contains neccesary
*				data for establishing TCP connecitons
*/
t_socket	setup_server_socket(int port);

/*
*	Send data to a worker
*		@param woker
*		@param header
*		@param data
*		@incomplete	prototype still needs to be flushed out
*/
void		send_to_worker();

/*
*	Sets up TCP connections with all the workers
*		@param	dispatcher	The dispatcher's main struct
*/
void		connect_workers(t_dispatcher *dispatcher);

/*
*	Get the next dataset from the web server and store it in the
*	dispatcher struct
*		@param	dispatcher	The dispatcher's main struct
*		@return all the n-body starting dataset (pos,vel,mass)
*/
void		request_dataset(t_dispatcher *dispatcher);

/*
*	Divide up the dataset into work_units and assign them a compute_class
*		@param	dispatcher	The dispatcher's main struct
*/
void		divide_dataset(t_dispatcher *dispatcher);
/*
*	Broadcast a super particle to all the cells for their computaiton
*	of the next tick (over the network)
*		@param	dispatcher	The dispatcher's main struct
*		@param	body	The super particle that represents a cell
*/
void		broadcast_super_particle(t_dispatcher *dispatcher, t_body *body);


/*
*	get the cell where the specified particle belongs
*		@param	dispatcher	The dispatcher's main struct
*		@param	body	particle to send
*		@return	cell to send it to
*/
t_cell		*find_appropriate_cell(t_dispatcher *dispatcher, t_body *body);

/*
*	Send body(particle) to a new cell
*		@param	dispatcher	The dispatcher's main struct
*		@param	body	particle to send
*		@param	cell	cell to send it to
*/
void		send_particle(t_dispatcher *dispatcher, t_body *body, t_cell *cell);

/*
*	Requests a worker to dump its cache of cell ticks to central storage
*		@param	dispatcher	The dispatcher's main struct
*		@param	worker	The worker that the request will be sent to
*		@return	0 if the request was fullfilled. 1 otherwise
*/
int 		request_cache_dump(t_dispatcher *dispatcher, t_worker *worker);

/*
*	Send a work unit to a specified worker
*		@param	dispatcher	The dispatcher's main struct
*		@param	worker	worker to recieve the work unit
*		@return	0 if the request was fullfilled. 1 otherwise
*/
int			send_work_unit(t_dispatcher *dispatcher, t_worker *worker);

/*
*	Coalesce all cells for each tick into a full tick
*		@param	dispatcher	The dispatcher's main struct
*		@incomplete	prototype still needs to be flushed out
*/
void		coalesce_into_ticks(t_dispatcher *dispatcher);

/*
*	Save the ticks to the appropriate file format
*		@param	dispatcher	The dispatcher's main struct
*		@incomplete	prototype still needs to be flushed out
*/
void 		save_ouptut(t_dispatcher *dispatcher);

#endif
