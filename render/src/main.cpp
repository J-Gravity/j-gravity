
#include "engine.h"
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>

Render				*g_render;

//hacky solution here, removing artifacts by sorting the output before reading

#define BUFSIZE pow(2, 15)

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
}				particle

int			zcomp(void *a, void *b)
{
	float	az = (float)*a;
	float	bz = (float)*b;

	if (az > bz)
		return (-1);
	if (az == bz)
		return (0);
	if (az < bz)
		return (1);
	if (az != bz)
	{
		printf("NAN FOUND IN FILE\n");
		exit(1);
	}
}

char				*sortfile(char *path)
{
	char			*newpath;
	char			*towrite;
	int				readsize;
	char			*buf;
	particle		*particles;

	newpath = "/tmp/new.jgrav";
	std::ofstream outfile (newpath);
	std::ifstream file (path, ios::in|ios::binary|ios::ate);
	particles = (particle*)malloc(sizeof(particle) * BUFSIZE);
	while (readsize = file.read(particles, BUFSIZE * sizeof(particle)))
	{
		qsort(particles, BUFSIZE, sizeof(particle), zcomp);
		outfile.write(particles, readsize);
	}
	return (newpath);
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
	path = sortfile(path);
	g_render = new Render(1600, 1200, path);
	g_render->set_scale();
	g_render->loop(atoi(argv[2]), atoi(argv[3]));
	g_render->wait_for_death();
	return (0);
}
