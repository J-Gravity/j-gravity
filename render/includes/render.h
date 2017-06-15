
#ifndef RENDER_H
# define RENDER_H

# include "engine.h"

class				Render;

typedef struct				s_thread
{
	int						id;
	FILE					*f; // Already at the correct cursor location
	long					count; // In number of particles
	Render					*dad;
}							t_thread;

class				Render
{
public:
	int							width;
	int							height;
	SDL_Window					*win;
	SDL_Renderer				*ren;
	Camera						*cam;
	long						tick;
	double						scale;
	std::string					path;
	Uint8						*keystate;
	SDL_mutex					*mutex;
	SDL_cond					*start_cond;
	SDL_cond					*done_cond;
	int							num_threads;
	int							running_threads;
	SDL_Texture					*tex;
	std::vector<unsigned char>	pixels;

	Render(int w = 640, int h = 480, std::string path = "data/test-");
	void						set_scale();
	void						draw();
	void						loop(long start, long end);
	void						wait_for_death();
	~Render();

private:
	std::vector<t_thread*>		threads;

	void						init_threading();
	void						organize_threads(long par);
};

#endif
