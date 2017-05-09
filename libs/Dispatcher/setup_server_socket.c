/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_server_socket.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyildiri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 19:52:54 by cyildiri          #+#    #+#             */
/*   Updated: 2017/05/08 19:52:55 by cyildiri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatcher.h"

static	int		set_socket_options(int server_sock)
{
	int	opt;
	int	level;
	int	option_name;

	opt = 0;
	level = SOL_SOCKET;
	option_name = SO_REUSEADDR | SO_REUSEPORT;
	if (setsockopt(server_sock, level, option_name, &opt, sizeof(opt)) == -1)
		return (-1);
	return (0);
}

static void		set_addr_info(t_socket *sock, short family, in_addr_t addr,
				int port)
{
	sock->addr->sin_family = family;
	sock->addr->sin_addr.s_addr = addr;
	sock->addr->sin_port = htons(port);
}

t_socket		setup_server_socket(int port)
{
	t_socket	server_sock;

	if ((server_sock.fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		printf("Creating the server socket failed with code: %d\n", errno);
		exit(1);
	}
	if (set_socket_options(server_sock.fd) == -1)
	{
		printf("Set socket options failed with code: %d\n", errno);
		exit(1);
	}
	set_addr_info(&server_sock, AF_INET, INADDR_ANY, port);
	if (bind(server_sock.fd, (struct sockaddr *)server_sock.addr,
		*server_sock.addrlen) == -1)
	{
		printf("Binding the server socket failed with code: %d\n", errno);
		exit(1);
	}
	if (listen(server_sock.fd, 3) == -1)
	{
		printf("Listening on server socket failed with code: %d\n", errno);
		exit(1);
	}
	return (server_sock);
}
