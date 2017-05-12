/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_dataset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 20:41:19 by scollet           #+#    #+#             */
/*   Updated: 2017/05/09 22:43:25 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatcher.h"

void  request_dataset(t_dispatcher *dispatcher, t_dataset **init_data)
{
  /*
  *   TODO : Get the next dataset from the web server and store it in the dispatcher struct;
  */

  connect(dispatcher.server_sock.fd, dispatcher., );
  if (0 > (read(dispatcher.server_sock.fd, dispatcher.dataset, some_length_TBD)))
    ft_error();

}
