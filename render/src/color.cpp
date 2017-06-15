
#include "engine.h"

std::vector<Color*>	Color::table = (std::vector<Color*>)0;
std::vector<Color*>	Color::ntable = (std::vector<Color*>)0;
double	Color::max_mass = 5.0;
double	Color::min_mass = 1.0;

static void			grav_exit()
{
	if (g_render->ren)
		SDL_DestroyRenderer(g_render->ren);
	if (g_render->win)
		SDL_DestroyWindow(g_render->win);
	delete	g_render;
	SDL_Quit();
	exit(0);
}

template <class ttype>
static inline void	check_error(ttype check, std::string message)
{
	if (check == (ttype)0)
	{
		std::cerr << message;
		grav_exit();
	}
}

Color::Color()
{
}

Color::Color(unsigned char nr, unsigned char ng, unsigned char nb)
{
	this->r = nr;
	this->g = ng;
	this->b = nb;
}

void				Color::init_color_table()
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	Color::table = std::vector<Color*>(32);
	for (int i = 0; i < 32; ++i)
	{
		r = 255.0;
		g = 255 - (float)i * 8.0; 
		b = 0.0;
		Color::table[i] = new Color(r, g, b);
	}
}

void				Color::ninit_color_table()
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	Color::ntable = std::vector<Color*>(32);
	for (int i = 0; i < 32; ++i)
	{
		r = 0.0;
		g = 255 - (float)i * 8.0; 
		b = 255.0;
		Color::ntable[i] = new Color(r, g, b);
	}
}

Color::~Color()
{
}
