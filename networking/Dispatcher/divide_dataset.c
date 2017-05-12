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

void  divide_dataset(t_dispatcher *dispatcher, t_dataset *init_data, t_lst **work_units)
{
  t_dispatcher  hold;
  t_work_unit   single_unit;

  hold = dispatcher.unassigned_cells + 1;
  while (--hold)
  {

    /*
    *   TODO : Recursively divide the dataset;
    *   TODO : Store results of Barne's Hut Algorithm in a cell;
    *   @Optimization TODO : Store the cell inside of work_unit and assign a compute_class;
    */

    single_unit = barnes_hut(init_data);
  }
  return ;
}
