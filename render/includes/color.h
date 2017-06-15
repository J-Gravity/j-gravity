
#ifndef COLOR_H
# define COLOR_H

# include "engine.h"

class							Color
{
public:
	unsigned char				r;
	unsigned char				g;
	unsigned char				b;
	static std::vector<Color*>	table;
	static std::vector<Color*>	ntable;
	static double				max_mass;
	static double				min_mass;

	Color();
	Color(int index);
	Color(double mass);
	Color(unsigned char nr, unsigned char ng, unsigned char nb);
	inline int					get_int();
	static void					init_color_table();
	static void					ninit_color_table();
	static void					set_range(double min, double max);
	static void					nset_range(double min, double max);
	~Color();
};

#endif
