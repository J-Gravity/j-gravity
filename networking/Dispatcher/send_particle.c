/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_particle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 22:34:22 by scollet           #+#    #+#             */
/*   Updated: 2017/05/09 22:42:39 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatcher.h"

void  send_particle(t_dispatcher *dispatcher, t_body *body, t_cell *cell)
{
  /*
  * TODO : send particle to a new cell;
  */
  write(dispatcher->workers.socket, cell, );
  return ;
}
