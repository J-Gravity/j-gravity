/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect_workers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 21:10:00 by scollet           #+#    #+#             */
/*   Updated: 2017/05/08 21:10:03 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatcher.h"

void  connect_workers(t_dispatcher *dispatcher)
{
  int             worker_fd;
  pid_t           pid;
  t_dispatcher    fd;
  struct addrinfo addr;

  /*
  *   TODO : Connect dispatcher to workers via TCP;
  */

  memset(&addr, 0, sizeof(addr));
  while (some value has not been met)
  {
    if (0 > (worker_fd = accept(dispatcher.server_fd, 0, 0)))
    {
      if (EINTR == errno)
        continue ;
      ft_error();
    }
    if (0 > (pid = fork()))
      ft_error();
    else if (0 == pid)
    {
      close(dispatcher.server_fd);
      while (dispatcher->worker_connections->next)
        dispatcher->worker_connections->next = dispatcher->worker_connections;

    }
    else
      close(worker_fd);
  }
}
