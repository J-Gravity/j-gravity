/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssmith <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 16:08:11 by ssmith            #+#    #+#             */
/*   Updated: 2017/05/09 09:45:16 by ssmith           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void	looper(struct sockaddr_in client, int server_fd, int client_fd)
{
	int			err;
	int			read;
	char		buf[BUFFER_SIZE];
	socklen_t	client_len;

	while (1)
	{
		client_len = sizeof(client);
		client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);
		if (client_fd < 0)
			printf("Could not establish new connection\n");
		while (1)
		{
			read = recv(client_fd, buf, BUFFER_SIZE, 0);
			printf("%s", buf);
			if (!read)
				break ;
			if (read < 0)
				printf("Client read failed\n");
			err = send(client_fd, buf, read, 0);
			if (err < 0)
				printf("Client write failed\n");
		}
	}
}

int		main(int argc, char *argv[])
{
	int					server_fd;
	int					client_fd;
	int					opt_val;
	struct sockaddr_in	server;
	struct sockaddr_in	client;

	opt_val = 1;
	if (argc < 2)
	{
		printf("Pass port as argument.\n");
		return (0);
	}
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		printf("Could not create socket\n");
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));
	if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
		printf("Could not bind socket\n");
	if (listen(server_fd, 128) < 0)
		printf("Could not listen on socket\n");
	looper(client, server_fd, client_fd);
	return (0);
}
