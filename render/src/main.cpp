
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
}				particle;


bool		zcomp(particle a, particle b)
{
	return (a.z < b.z);
}

void				sortfile(std::string path, int i)
{
	std::string		newpath;
	char			*towrite;
	int				readsize;
	char			buf[8];
	particle		*particles;

	newpath = "/tmp/new.jgrav";
	std::ofstream outfile (newpath);
	std::ifstream file (path, std::ios::in|std::ios::binary|std::ios::ate);
	particles = (particle*)malloc(sizeof(particle) * BUFSIZE);
	file.read(buf, 8);
	outfile.write(buf, 8);
	while ((readsize = file.read((char*)particles, BUFSIZE * sizeof(particle))))
	{
		std::sort(particles, &(particles[BUFSIZE - 1]), zcomp);
		outfile.write((const char*)particles, readsize);
	}
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
