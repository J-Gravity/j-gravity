/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_work_unit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 22:42:08 by scollet           #+#    #+#             */
/*   Updated: 2017/05/09 22:42:12 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatcher.h"

int   send_work_unit(t_dispatcher *dispatcher, t_worker *worker)
{
  if ((worker) && (*worker))
  {
    if (0 > (write(worker.socket.fd, dispatcher->work_units,
      dispatcher->work_units.cell.body_count)))
      return (1);
    return (0);
  }
  return (1);
}
