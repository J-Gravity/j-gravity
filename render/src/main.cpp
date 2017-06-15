
#include "engine.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

Render				*g_render;

//hacky solution here, removing artifacts by sorting the output before reading

#define BUFSIZE 65536

typedef struct	particle_s
{
	float		x;
	float		y;
	float		z;
	float		m;
	float		xv;
	float		yv;
	float		zv;
	float		e;
}				particle;

bool		zcomp(particle a, particle b)
{
	return (a.z < b.z);
}

static inline long      h_read_long(FILE *f)
{
	long            out;

	fread((void*)(&out), sizeof(out), 1, f);
	return out;
}

static inline float		h_read_float(FILE *f)
{
	float           out;

	fread((void*)(&out), sizeof(out), 1, f);
	return (double)out;
}

static inline particle	h_read_particle(FILE *f)
{
	particle par;

	par.x = h_read_float(f);
	par.y = h_read_float(f);
	par.z = h_read_float(f);
	par.m = h_read_float(f);
	return (par);
}

static inline void		h_write_particle(FILE *f, particle p)
{
	fwrite(&p.x, sizeof(float), 1, f);
	fwrite(&p.y, sizeof(float), 1, f);
	fwrite(&p.z, sizeof(float), 1, f);
	fwrite(&p.m, sizeof(float), 1, f);
}

void		sortfile(std::string path, int index)
{
	std::string		newpath;
	FILE			*input;
	FILE			*output;
	long			cnt;
	long			scale;
	long			i;
	long			j;
	long			k;
	std::vector<particle> vp;
	particle		p;

	newpath = path + "new-" + std::to_string(index) + ".jgrav";
	path = path + std::to_string(index) + ".jgrav";
	input = fopen(path.c_str(), "rb");
	output = fopen(newpath.c_str(), "wb");
	cnt = h_read_long(input);
	fwrite(&cnt, sizeof(long), 1, output);
	scale = h_read_long(input);
	fwrite(&scale, sizeof(long), 1, output);
	i = 0;
	while (i < cnt)
	{
		j = 0;
		while (j < BUFSIZE && i + j < cnt)
		{
			p = h_read_particle(input);
			vp.push_back(p);
			++j;
		}
		std::sort(vp.begin(), vp.end(), zcomp);
		k = 0;
		while (k < j)
		{
			h_write_particle(output, vp[k]);
			++k;
		}
		i += j; 
		vp.clear();
	}
	fclose(input);
	fclose(output);
}

int					main(int argc, char *argv[])
{
	std::string		path;
	if (argc < 3)
	{
		std::cerr << "Usage: ./grav [prefix] [start frame] [end frame]\n";
		std::cerr << "Example: ./grav big_data/multimass- 0 1199\n";
		return 0;
	}
	path = argv[1];
	for (int i = atoi(argv[2]); i <= atoi(argv[3]); i++)
		sortfile(path, i);
	path = path + "new-";
	g_render = new Render(1600, 1200, path);
	g_render->set_scale();
	g_render->loop(atoi(argv[2]), atoi(argv[3]));
	g_render->wait_for_death();
	return (0);
}
