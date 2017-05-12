/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_server_socket.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 22:43:44 by scollet           #+#    #+#             */
/*   Updated: 2017/05/09 22:43:45 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatcher.h"

t_socket  setup_server_socket(int port)
{
  t_socket  server_sock;
  int       opt;

  server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (0 > setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT), &opt, sizeof(opt))
    perror();
  addr->sin_family = AF_INET;
  addr->sin_addr.s_addr = INADDR_ANY;
  addr->sin_port = htons(port);
  if (0 > bind(server_sock, (struct sockaddr *)addr, *addr_len))
    perror();
  if (0 > (listen(server_sock, 5)))
    perror();
  return (server_sock);
}
