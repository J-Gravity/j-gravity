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

void  connect_workers(t_dispatcher *dispatcher, t_lst **workers)
{
  int             worker_fd;
  pthread_t       *work_thread;
  /*
  *   TODO : Connect dispatcher to workers via TCP;
  *   : add worker to linked list;
  *   : update worker socket;
  *   : repeat for next worker;
  */

  while (workers->next)
  {
    if (0 > (*workers.socket.fd = accept(dispatcher.server_fd, 0, 0)))
    {
      if (EINTR == errno)
        continue ;
      break ;
    }
    //add worker to linked list
    dispatcher->workers = *workers;
    dispatcher->workers->next = workers->next;
    *workers = workers->next;
  }
  return ;
}
