#include "dispatcher.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

char	*itob(int value)
{
	unsigned int	i;
	char			*string;

	i = 0;
	string = calloc(1, sizeof(int));
	while (i < sizeof(int))
	{
		string[i] = ((char *)(&value))[i];
		i++;
	}
	return string;	
}

char	*ftob(float value)
{
	char *string;

	string = calloc(1, sizeof(float));
	for (unsigned int i = 0; i < sizeof(float); i++)
		string[i] = ((char *)(&value))[i];
	return string;	
}

int		btoi(char *str)
{
	int				ret;

	for (unsigned int i = 0; i < sizeof(int); i++)
		((char *)(&ret))[i] = str[i];
	return (ret);
}

float	btof(char *str)
{
	float	ret;

	for (unsigned int i = 0; i < sizeof(float); i++)
		((char *)(&ret))[i] = str[i];
	return (ret);
}

void	strbjoin(t_serial *serial, char const *s2, size_t size)
{
	char	*copy;
	int		i;

	i = 0;
	copy = (char *)malloc(serial->len);
	for (int x = 0; x < serial->len; x++)
		copy[x] = serial->data[x];
	serial->data = (char *)calloc(1, serial->len + size);
	while (i < serial->len)
	{
		serial->data[i] = copy[i];
		i++;
	}
	for (unsigned int x = 0; x < size; x++)
	{
		serial->data[i] = s2[x];
		i++;
	}
	serial->len += size;
}

char	*work_unit_serializer(t_work_unit *work_unit)
{
	t_serial		*serial;

	serial = malloc(sizeof(serial));
	serial->data = itob(1);
	serial->len = 4;
	strbjoin(serial, itob(work_unit->compute_class), sizeof(int));
	strbjoin(serial, itob(work_unit->cell.body_count), sizeof(int));
	for (int i = 0; i< work_unit->cell.body_count; i++)
	{
		strbjoin(serial, ftob(work_unit->cell.contained_bodies[i]->position.x), sizeof(float));
		strbjoin(serial, ftob(work_unit->cell.contained_bodies[i]->position.y), sizeof(float));
		strbjoin(serial, ftob(work_unit->cell.contained_bodies[i]->position.z), sizeof(float));
		strbjoin(serial, ftob(work_unit->cell.contained_bodies[i]->velocity.x), sizeof(float));
		strbjoin(serial, ftob(work_unit->cell.contained_bodies[i]->velocity.y), sizeof(float));
		strbjoin(serial, ftob(work_unit->cell.contained_bodies[i]->velocity.z), sizeof(float));
	}
	printf("serial->len %d\n", serial->len);
	return (serial->data);
}

t_work_unit	*work_unit_parser(char *str)
{
	int			i;
	t_work_unit	*work_unit;

	i = 0;
	work_unit = malloc(sizeof(t_work_unit));
	work_unit->compute_class = btoi(str += 4);
	printf("wu->compute_class = %d\n", work_unit->compute_class);
	work_unit->cell.body_count = btoi(str += 4);
	printf("wu->body_count = %d\n", work_unit->cell.body_count);
	work_unit->cell.contained_bodies = (t_body **)malloc(sizeof(t_body *) * work_unit->cell.body_count);
	for (int i = 0; i < work_unit->cell.body_count; i++)
	{
		work_unit->cell.contained_bodies[i] = (t_body *)malloc(sizeof(t_body));
		work_unit->cell.contained_bodies[i]->position.x = btoi(str += 4);
		work_unit->cell.contained_bodies[i]->position.y = btoi(str += 4);
		work_unit->cell.contained_bodies[i]->position.z = btoi(str += 4);
		work_unit->cell.contained_bodies[i]->velocity.x = btoi(str += 4);
		work_unit->cell.contained_bodies[i]->velocity.y = btoi(str += 4);
		work_unit->cell.contained_bodies[i]->velocity.z = btoi(str += 4);
	}
	return (work_unit);
}

void	serializer_identifier(char *str)
{
	printf("\nserializer id: %d\n", btoi(str));
	if (btoi(str) == 1)
	{
		printf("yes\n");
		work_unit_parser(str);
	}
}

int		main(void)
{
	int			i;
	char		*serial_str;
	t_work_unit	*work_unit;

	i = -1;
	work_unit = malloc(sizeof(t_work_unit));
	work_unit->cell.body_count = 10;
	work_unit->cell.contained_bodies = (t_body **)malloc(sizeof(t_body *) * work_unit->cell.body_count);
	for (int i = 0; i < work_unit->cell.body_count; i++)
	{
		work_unit->cell.contained_bodies[i] = (t_body *)malloc(sizeof(t_body));
		work_unit->cell.contained_bodies[i]->position.x = 123456789.123;
		work_unit->cell.contained_bodies[i]->position.y = 987654321.321;
		work_unit->cell.contained_bodies[i]->position.z = 123456789.123;
		work_unit->cell.contained_bodies[i]->velocity.x = 111111111.222;
		work_unit->cell.contained_bodies[i]->velocity.y = 222222222.333;
		work_unit->cell.contained_bodies[i]->velocity.z = 333333333.444;
	}
	work_unit->adjoining_cells = malloc(sizeof(t_cell));
	work_unit->adjoining_cells->contained_bodies = (t_body **)malloc(sizeof(t_body*));
	work_unit->adjoining_cells->contained_bodies[0] = (t_body *)malloc(sizeof(t_body) *
			work_unit->cell.body_count);
	while (++i < work_unit->cell.body_count)
	{
		work_unit->adjoining_cells->contained_bodies[0][i].position.x = i;
		work_unit->adjoining_cells->contained_bodies[0][i].position.y = i;
		work_unit->adjoining_cells->contained_bodies[0][i].position.z = i;
		work_unit->adjoining_cells->contained_bodies[0][i].velocity.x = i;
		work_unit->adjoining_cells->contained_bodies[0][i].velocity.y = i;
		work_unit->adjoining_cells->contained_bodies[0][i].velocity.z = i;
	}
	work_unit->adjoining_cells->cell_as_body.velocity.x = i;
	work_unit->adjoining_cells->cell_as_body.velocity.y = i;
	work_unit->adjoining_cells->cell_as_body.velocity.z = i;
	work_unit->adjoining_cells_cnt = 10;
	work_unit->compute_class = 555;
	serial_str = work_unit_serializer(work_unit);
	serializer_identifier(serial_str);
	return (0);
}




















































/*
int		main(void)
{
	int			i;
	int			body_ct;
	t_worker	*worker;

	i = -1;
	body_ct = 10;
	worker = malloc(sizeof(t_worker));
	worker->active_cells = malloc(sizeof(t_cell));
	worker->active_cells->contained_bodies = (t_body **)malloc(sizeof(t_body*));
	worker->active_cells->contained_bodies[0] = (t_body *)malloc(sizeof(t_body) * body_ct);
	while (++i < body_ct)
	{
		worker->active_cells->contained_bodies[0][i].position.x = i;
		worker->active_cells->contained_bodies[0][i].position.y = i;
		worker->active_cells->contained_bodies[0][i].position.z = i;
		worker->active_cells->contained_bodies[0][i].velocity.x = i;
		worker->active_cells->contained_bodies[0][i].velocity.y = i;
		worker->active_cells->contained_bodies[0][i].velocity.z = i;
	}
		worker->active_cells->cell_as_body.velocity.x = i;
		worker->active_cells->cell_as_body.velocity.y = i;
		worker->active_cells->cell_as_body.velocity.z = i;
}
*/

/*
int		main(void)
{
	t_dispatcher	*dispatcher;

	dispatcher = malloc(sizeof(t_dispatcher));
	dispatcher->workers = malloc(sizeof(t_lst));
	dispatcher->data = malloc(sizeof(t_dataset));
	dispatcher->cells = malloc(sizeof(t_lst));
	return (0);
}
*/
