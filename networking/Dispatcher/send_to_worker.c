/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_to_worker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 22:43:32 by scollet           #+#    #+#             */
/*   Updated: 2017/05/09 22:43:33 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void  send_to_worker(t_dispatcher *dispatcher)
{
  write(dispatcher->worker_connections, dispatcher->data, sizeof(data) * );
  return ;
}
