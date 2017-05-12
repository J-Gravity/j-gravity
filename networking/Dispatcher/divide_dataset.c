/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   divide_dataset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/09 22:43:16 by scollet           #+#    #+#             */
/*   Updated: 2017/05/09 22:43:17 by scollet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatcher.h"

void  divide_dataset(t_dispatcher *dispatcher)
{
  t_work_unit   work_unit;
  t_dispatcher  hold;
  t_cell        cell;

  hold = dispatcher.unassigned_cells + 1;
  while (--hold)
  {
    /*
    *   TODO : Recursively divide the dataset;
    *   TODO : Store results of Barne's Hut Algorithm in a cell;
    *   TODO : Store the cell inside of work_unit and assign a compute_class;
    */
    ft_memcpy(work_unit->compute_class, class, ft_strlen(class));
  }
  return ;
}
