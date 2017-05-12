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
		strbjoin(serial, ftob(work_unit->cell.contained_bodies[i]->mass), sizeof(float));
	}
	strbjoin(serial, ftob(work_unit->cell.cell_as_body.position.x), sizeof(float));
	strbjoin(serial, ftob(work_unit->cell.cell_as_body.position.y), sizeof(float));
	strbjoin(serial, ftob(work_unit->cell.cell_as_body.position.z), sizeof(float));
	strbjoin(serial, ftob(work_unit->cell.cell_as_body.velocity.x), sizeof(float));
	strbjoin(serial, ftob(work_unit->cell.cell_as_body.velocity.y), sizeof(float));
	strbjoin(serial, ftob(work_unit->cell.cell_as_body.velocity.z), sizeof(float));
	strbjoin(serial, ftob(work_unit->cell.cell_as_body.mass), sizeof(float));
	strbjoin(serial, itob(work_unit->adjoining_cells_cnt), sizeof(int));
	for (int i = 0; i < work_unit->adjoining_cells_cnt; i++)
	{
		strbjoin(serial, itob(work_unit->adjoining_cells[i].body_count), sizeof(int));
		for (int x = 0; x < work_unit->adjoining_cells[i].body_count; x++)
		{
			strbjoin(serial, ftob(work_unit->adjoining_cells[i].contained_bodies[x]->position.x), sizeof(float));
			strbjoin(serial, ftob(work_unit->adjoining_cells[i].contained_bodies[x]->position.y), sizeof(float));
			strbjoin(serial, ftob(work_unit->adjoining_cells[i].contained_bodies[x]->position.z), sizeof(float));
			strbjoin(serial, ftob(work_unit->adjoining_cells[i].contained_bodies[x]->velocity.x), sizeof(float));
			strbjoin(serial, ftob(work_unit->adjoining_cells[i].contained_bodies[x]->velocity.y), sizeof(float));
			strbjoin(serial, ftob(work_unit->adjoining_cells[i].contained_bodies[x]->velocity.z), sizeof(float));
			strbjoin(serial, ftob(work_unit->adjoining_cells[i].contained_bodies[x]->mass), sizeof(float));
		}
		strbjoin(serial, ftob(work_unit->adjoining_cells[i].cell_as_body.position.x), sizeof(float));
		strbjoin(serial, ftob(work_unit->adjoining_cells[i].cell_as_body.position.y), sizeof(float));
		strbjoin(serial, ftob(work_unit->adjoining_cells[i].cell_as_body.position.z), sizeof(float));
		strbjoin(serial, ftob(work_unit->adjoining_cells[i].cell_as_body.velocity.x), sizeof(float));
		strbjoin(serial, ftob(work_unit->adjoining_cells[i].cell_as_body.velocity.y), sizeof(float));
		strbjoin(serial, ftob(work_unit->adjoining_cells[i].cell_as_body.velocity.z), sizeof(float));
		strbjoin(serial, ftob(work_unit->adjoining_cells[i].cell_as_body.mass), sizeof(float));
	}
	return (serial->data);
}

t_work_unit	*work_unit_parser(char *str)
{
	int			i;
	t_work_unit	*work_unit;

	i = 0;
	work_unit = malloc(sizeof(t_work_unit));
	work_unit->compute_class = btoi(str += 4);
	work_unit->cell.body_count = btoi(str += 4);
	work_unit->cell.contained_bodies = (t_body **)malloc(sizeof(t_body *) * work_unit->cell.body_count);
	for (int i = 0; i < work_unit->cell.body_count; i++)
	{
		work_unit->cell.contained_bodies[i] = (t_body *)malloc(sizeof(t_body));
		work_unit->cell.contained_bodies[i]->position.x = btof(str += 4);
		work_unit->cell.contained_bodies[i]->position.y = btof(str += 4);
		work_unit->cell.contained_bodies[i]->position.z = btof(str += 4);
		work_unit->cell.contained_bodies[i]->velocity.x = btof(str += 4);
		work_unit->cell.contained_bodies[i]->velocity.y = btof(str += 4);
		work_unit->cell.contained_bodies[i]->velocity.z = btof(str += 4);
		work_unit->cell.contained_bodies[i]->mass = btof(str += 4);
	}
	work_unit->cell.cell_as_body.position.x = btof(str += 4);
	work_unit->cell.cell_as_body.position.y = btof(str += 4);
	work_unit->cell.cell_as_body.position.z = btof(str += 4);
	work_unit->cell.cell_as_body.velocity.x = btof(str += 4);
	work_unit->cell.cell_as_body.velocity.y = btof(str += 4);
	work_unit->cell.cell_as_body.velocity.z = btof(str += 4);
	work_unit->cell.cell_as_body.mass = btof(str += 4);
	work_unit->adjoining_cells_cnt = btoi(str += 4);
	work_unit->adjoining_cells = (t_cell *)malloc(sizeof(t_cell) * work_unit->adjoining_cells_cnt);
	for (int i = 0; i < work_unit->adjoining_cells_cnt; i++)
	{
		work_unit->adjoining_cells[i].body_count = btoi(str += 4);
		work_unit->adjoining_cells[i].contained_bodies =
			(t_body **)malloc(sizeof(t_body *) * work_unit->adjoining_cells[i].body_count);
		for (int x = 0; x < work_unit->adjoining_cells[i].body_count; x++)
		{
			work_unit->adjoining_cells[i].contained_bodies[x] = (t_body *)malloc(sizeof(t_body));
			work_unit->adjoining_cells[i].contained_bodies[x]->position.x = btof(str += 4);
			work_unit->adjoining_cells[i].contained_bodies[x]->position.y = btof(str += 4);
			work_unit->adjoining_cells[i].contained_bodies[x]->position.z = btof(str += 4);
			work_unit->adjoining_cells[i].contained_bodies[x]->velocity.x = btof(str += 4);
			work_unit->adjoining_cells[i].contained_bodies[x]->velocity.y = btof(str += 4);
			work_unit->adjoining_cells[i].contained_bodies[x]->velocity.z = btof(str += 4);
			work_unit->adjoining_cells[i].contained_bodies[x]->mass = btof(str += 4);
		}
		work_unit->adjoining_cells[i].cell_as_body.position.x = btof(str += 4);
		work_unit->adjoining_cells[i].cell_as_body.position.y = btof(str += 4);
		work_unit->adjoining_cells[i].cell_as_body.position.z = btof(str += 4);
		work_unit->adjoining_cells[i].cell_as_body.velocity.x = btof(str += 4);
		work_unit->adjoining_cells[i].cell_as_body.velocity.y = btof(str += 4);
		work_unit->adjoining_cells[i].cell_as_body.velocity.z = btof(str += 4);
		work_unit->adjoining_cells[i].cell_as_body.mass = btof(str += 4);
	}
	return (work_unit);
}

void	serializer_identifier(char *str)
{
	if (btoi(str) == 1)
		work_unit_parser(str);
}

int		main(void)
{
	int			i;
	char		*serial_str;
	t_work_unit	*work_unit;

	i = -1;
	work_unit = malloc(sizeof(t_work_unit));
	work_unit->compute_class = 555;
	//.......cells.........//
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
		work_unit->cell.contained_bodies[i]->mass = 333333333.444;
	}
	work_unit->cell.cell_as_body.position.x = 55;
	work_unit->cell.cell_as_body.position.y = 55;
	work_unit->cell.cell_as_body.position.z = 55;
	work_unit->cell.cell_as_body.velocity.x = 22;
	work_unit->cell.cell_as_body.velocity.y = 22;
	work_unit->cell.cell_as_body.velocity.z = 22;
	work_unit->cell.cell_as_body.mass = 100;
	//.........adjoining cells........//
	work_unit->adjoining_cells = malloc(sizeof(t_cell));
	work_unit->adjoining_cells_cnt = 10;
	work_unit->adjoining_cells = (t_cell *)malloc(sizeof(t_cell) * work_unit->adjoining_cells_cnt);
	for (int i = 0; i < work_unit->adjoining_cells_cnt; i++)
	{
		work_unit->adjoining_cells[i].body_count = 10;
		work_unit->adjoining_cells[i].contained_bodies =
			(t_body **)malloc(sizeof(t_body *) * work_unit->adjoining_cells[i].body_count);
		for (int x = 0; x < work_unit->adjoining_cells[i].body_count; x++)
		{
			work_unit->adjoining_cells[i].contained_bodies[x] = (t_body *)malloc(sizeof(t_body));
			work_unit->adjoining_cells[i].contained_bodies[x]->position.x = i * x;
			work_unit->adjoining_cells[i].contained_bodies[x]->position.y = i * x;
			work_unit->adjoining_cells[i].contained_bodies[x]->position.z = i * x;
			work_unit->adjoining_cells[i].contained_bodies[x]->velocity.x = i * x;
			work_unit->adjoining_cells[i].contained_bodies[x]->velocity.y = i * x;
			work_unit->adjoining_cells[i].contained_bodies[x]->velocity.z = i * x;
			work_unit->adjoining_cells[i].contained_bodies[x]->mass = i * x;
		}
		work_unit->adjoining_cells->cell_as_body.position.x = i;
		work_unit->adjoining_cells->cell_as_body.position.y = i;
		work_unit->adjoining_cells->cell_as_body.position.z = i;
		work_unit->adjoining_cells->cell_as_body.velocity.x = i;
		work_unit->adjoining_cells->cell_as_body.velocity.y = i;
		work_unit->adjoining_cells->cell_as_body.velocity.z = i;
		work_unit->adjoining_cells->cell_as_body.mass = i;
	}
	serial_str = work_unit_serializer(work_unit);
	serializer_identifier(serial_str);
	return (0);
}
