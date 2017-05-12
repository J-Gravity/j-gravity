/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 20:48:50 by cyildiri          #+#    #+#             */
/*   Updated: 2017/05/08 20:48:51 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatcher.h"

int	main(void)
{
	t_dispatcher	*dispatcher;

	dispatcher = (t_dispatcher	*)calloc(1, sizeof(t_dispatcher));
	dispatcher->server_sock = setup_server_socket(PORT);
	connect_workers(dispatcher, &dispatcher->workers);
	request_dataset(dispatcher, &dispatcher->dataset);
	divide_dataset(dispatcher, dispatcher->dataset, &dispatcher->work_units);
	launch_simulation(dispatcher); // blocks thread until all workers are done.
	dump_all_workers_cache(dispatcher);
	coalesce_into_ticks(dispatcher);
	save_output(dispatcher);
	return (0);
}
