/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_dataset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scollet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 20:41:19 by scollet           #+#    #+#             */
/*   Updated: 2017/05/16 16:15:00 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dispatcher.h"

void	web_server(void)
{
	t_dataset	*dataset;

	dataset = (t_dataset *)malloc(sizeof(dataset));
	dataset->particle_cnt = 1000000;
	dataset->particles = (t_body *)malloc(sizeof(t_body) * dataset->particle_cnt);
	for (int i = 0; i < dataset->particle_cnt; i++)
	{
		dataset->particles[i].position.x = (float)rand() / RAND_MAX * 100;
		dataset->particles[i].position.y = (float)rand() / RAND_MAX * 100;
		dataset->particles[i].position.z = (float)rand() / RAND_MAX * 100;
		dataset->particles[i].velocity.x = 0;
		dataset->particles[i].velocity.y = 0;
		dataset->particles[i].velocity.z = 0;
		dataset->particles[i].mass = 9999999999999999;
	}
	return (dataset);
}

void	request_dataset(t_dispatcher *dispatcher, t_dataset **init_data)
{

	/*
	 *   TODO : Get the dataset from the web server and store it in the dispatcher struct;
	 */
}

int		main(void)
{
	web_server();
	return (0);
}
