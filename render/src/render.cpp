
#include "engine.h"

static void				grav_exit()
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
static inline void		check_error(ttype check, std::string message)
{
	if (check == (ttype)0)
	{
		std::cerr << message;
		grav_exit();
	}
}

Render::Render(int w, int h, std::string path)
{
	this->width = w;
	this->height = h;
	this->path = path;
	check_error(!SDL_Init(SDL_INIT_EVERYTHING), "SDL Init error\n");
	this->win = SDL_CreateWindow("J-Gravity", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	check_error(this->win, "SDL Window creation error\n");
	this->ren = SDL_CreateRenderer(this->win, -1, SDL_RENDERER_ACCELERATED
		| SDL_RENDERER_PRESENTVSYNC);
	check_error(this->ren, "SDL Render create error\n");
	this->cam = new Camera(this->width, this->width,
		new Vector(0, 0, 0.0), new Matrix(ROTATION, 0.0, 0.0, 0.0),
		60, 1000.0, 500000.0);
	this->tick = 0;
	Color::init_color_table();
	Color::ninit_color_table();
	init_threading();
}

static inline long		h_read_long(FILE *f)
{
	long			out;

	fread((void*)(&out), sizeof(out), 1, f);
	return out;
}

static inline double	h_read_double(FILE *f)
{
	float			out;
	size_t			r;

	r = fread((void*)(&out), sizeof(out), 1, f);
	check_error(r == 1, "Error reading from file");
	return (double)out;
}

static int				h_digits(int i)
{
	if (i < 10 && i >= 0)
		return 1;
	else if (i < 100 && i >= 0)
		return 2;
	else if (i < 1000 && i >= 0)
		return 3;
	else if (i < 10000 && i >= 0)
		return 4;
	else if (i < 100000 && i >= 0)
		return 5;
	else if (i < 1000000 && i >= 0)
		return 6;
	else if (i < 10000000 && i >= 0)
		return 7;
	else
	{
		std::cerr << "Ridiculous frame count achieved.\n";
		grav_exit();
		return (0);
	}
}

void					Render::set_scale()
{
	FILE	*f;
	std::string	name;
	long	tmp;

	name = this->path + std::to_string(this->tick) + ".jgrav";
	f = fopen(name.c_str(), "rb");
	tmp = h_read_long(f);
	Render::scale = (double)h_read_long(f) * 1000;
	fclose(f);
}

int						thread_func(void *tmp)
{
	t_thread			*t;
	int					dif;
	Vector				*vec;
	Color				*c;
	double				mass;
	int					x, y, off;
	int					width, height, index;
	double				gcoef, gcons;

	t = (t_thread*)tmp;
	dif = (t->dad->width - t->dad->height) / 2;
	width = t->dad->width;
	height = t->dad->height;
	vec = new Vector();
	SDL_mutexP(t->dad->mutex);
	SDL_CondWait(t->dad->start_cond, t->dad->mutex);
	SDL_mutexV(t->dad->mutex);
	while (true)
	{
		gcoef = (double)width / (t->dad->scale * 3.0);
		gcons = (double)width * (t->dad->scale * 1.5) / (t->dad->scale * 3.0);
		for (long i = 0; i < t->count; i++)
		{
			vec->x = h_read_double(t->f);
			vec->y = h_read_double(t->f);
			vec->z = h_read_double(t->f);
			mass = h_read_double(t->f);
			if (mass > 0.0)
			{
				index = (mass - 1.0) / Color::max_mass * 32;
				if (index > 31)
					index = 31;
				c = Color::table[index];
			}
			else
			{
				index = -(mass + 1.0) / Color::max_mass * 32;
				if (index > 31)
					index = 31;
				c = Color::ntable[index];
			}
			t->dad->cam->watch_vector(vec);
			x = gcoef * vec->x + gcons;
			y = gcoef * vec->y + gcons - dif;
			if (x >= 0 && x < width && y >= 0 && y < height)
			{
				off = (t->dad->width * 4 * y) + x * 4;
				t->dad->pixels[off + 0] = c->b;
				t->dad->pixels[off + 1] = c->g;
				t->dad->pixels[off + 2] = c->r;
				t->dad->pixels[off + 3] = SDL_ALPHA_OPAQUE;
			}
		}
		fclose(t->f);
		SDL_mutexP(t->dad->mutex);
		t->dad->running_threads--;
		if (t->dad->running_threads == 0)
			SDL_CondSignal(t->dad->done_cond);
		SDL_CondWait(t->dad->start_cond, t->dad->mutex);
		SDL_mutexV(t->dad->mutex);
	}
	delete vec;
	return 0;
}

void					Render::init_threading()
{
	std::string			name;

	this->num_threads = 4;
	this->mutex = SDL_CreateMutex();
	this->start_cond = SDL_CreateCond();
	this->done_cond = SDL_CreateCond();
	this->threads = std::vector<t_thread*>(this->num_threads);
	this->tex = SDL_CreateTexture(this->ren,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		this->width, this->height);
	this->pixels = std::vector<unsigned char>(this->width * this->height * 4,
		0);
	for (int i = 0; i < this->num_threads; i++)
	{
		this->threads[i] = new t_thread;
		this->threads[i]->id = i;
		this->threads[i]->dad = this;
		name = "thread_id" + std::to_string(i);
		SDL_CreateThread(thread_func, name.c_str(),
			(void*)this->threads[i]);
	}
}

void					Render::organize_threads(long par)
{
	std::string			name;
	long				cur;
	long				each;
	long				mod;

	cur = 16;
	each = par / this->num_threads;
	mod = par % this->num_threads;
	this->running_threads = this->num_threads;
	std::fill(this->pixels.begin(), this->pixels.end(), 0);
	for (int i = 0; i < this->num_threads; i++)
	{
		name = this->path + std::to_string(this->tick) + ".jgrav";
		this->threads[i]->f = fopen(name.c_str(), "r");
		fseek(this->threads[i]->f, cur, SEEK_SET);
		if (i + 2 == this->num_threads)
			each += mod;
		this->threads[i]->count = each;
		cur += each * 16;
	}
}

void					Render::draw()
{
	FILE				*f;
	std::string			name;
	long				par;
	SDL_Surface			*s;
	std::string			png_name;

	name = this->path + std::to_string(this->tick) + ".jgrav";
	f = fopen(name.c_str(), "rb");
	SDL_SetRenderDrawColor(this->ren, 0, 0, 0, 0);
	SDL_RenderClear(this->ren);
	par = h_read_long(f);
	organize_threads(par);
	fclose(f);
	SDL_mutexP(this->mutex);
	SDL_CondBroadcast(this->start_cond);
	SDL_CondWait(this->done_cond, this->mutex);
	SDL_mutexV(this->mutex);
	SDL_UpdateTexture(this->tex, NULL, &this->pixels[0], this->width * 4);
	SDL_RenderCopy(this->ren, this->tex, NULL, NULL);
	SDL_RenderPresent(this->ren);
	s = SDL_CreateRGBSurface(0, this->width, this->height, 32,
			0, 0, 0, 0xff000000);
	if (s)
	{
		SDL_RenderReadPixels(this->ren, NULL, SDL_GetWindowPixelFormat(this->win),
				s->pixels, s->pitch);
		png_name = this->path + std::to_string(this->tick) + ".bmp";
		SDL_SaveBMP(s, png_name.c_str());
		SDL_FreeSurface(s);
	}
}

static void				h_update_from_input(Render *r, bool *u)
{
	bool				update;

	update = false;
	if (r->keystate[SDL_SCANCODE_W] && (update = true) == true)
		r->cam->mod_angles(M_PI / 32.0, 0.0, 0.0);
	if (r->keystate[SDL_SCANCODE_S] && (update = true) == true)
		r->cam->mod_angles(-M_PI / 32.0, 0.0, 0.0);
	if (r->keystate[SDL_SCANCODE_A] && (update = true) == true)
		r->cam->mod_angles(0.0, -M_PI / 32.0, 0.0);
	if (r->keystate[SDL_SCANCODE_D] && (update = true) == true)
		r->cam->mod_angles(0.0, M_PI / 32.0, 0.0);
	if (r->keystate[SDL_SCANCODE_Q] && (update = true) == true)
		r->cam->mod_angles(0.0, 0.0, -M_PI / 32.0);
	if (r->keystate[SDL_SCANCODE_E] && (update = true) == true)
		r->cam->mod_angles(0.0, 0.0, M_PI / 32.0);
	if (r->keystate[SDL_SCANCODE_UP] && (update = true) == true)
		r->cam->mod_location(0.0, -r->scale / 200.0, 0.0);
	if (r->keystate[SDL_SCANCODE_DOWN] && (update = true) == true)
		r->cam->mod_location(0.0, r->scale / 200.0, 0.0);
	if (r->keystate[SDL_SCANCODE_LEFT] && (update = true) == true)
		r->cam->mod_location(-r->scale / 200.0, 0.0, 0.0);
	if (r->keystate[SDL_SCANCODE_RIGHT] && (update = true) == true)
		r->cam->mod_location(r->scale / 200.0, 0.0, 0.0);
	if (update)
		*u = 0;
}

void					Render::loop(long start, long end)
{
	bool				running;
	bool				paused;
	bool				updated;
	int					dir;
	SDL_Event			event;

	this->tick = start;
	running = 1;
	paused = 1;
	updated = 0;
	dir = 1;
	while (running)
	{
		if (!paused)
		{
			this->tick += dir;
			updated = 0;
		}
		if (this->tick == end || this->tick == 0)
			paused = 1;
		if (this->tick < start)
		{
			updated = 0;
			this->tick = start;
		}
		else if (this->tick > end)
		{
			updated = 0;
			this->tick = end;
		}
		if (!updated)
		{
			this->draw();
			updated = 1;
		}
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_SPACE:
							paused = !paused;
							break;
						case SDLK_b:
							if (this->tick > start)
								this->tick--;
							updated = 0;
							break;
						case SDLK_n:
							if (this->tick < end)
								this->tick++;
							updated = 0;
							break;
						case SDLK_m:
							this->tick = start;
							updated = 0;
							break;
						case SDLK_r:
							dir *= -1;
							break;
						default:
							break;
					}
					break;
				case SDL_MOUSEWHEEL:
					if (event.wheel.y < 0)
						this->scale *= 1.1;
					else if (event.wheel.y > 0)
						this->scale /= 1.1;
					updated = 0;
					break;
			}
		}
		this->keystate = (Uint8*)SDL_GetKeyboardState(NULL);
		h_update_from_input(this, &updated);
	}
}

void					Render::wait_for_death()
{
	SDL_Event			e;

	while (1)
		while (SDL_PollEvent(&e))
			switch (e.type)
			{
				case SDL_KEYDOWN:
				case SDL_QUIT:
					grav_exit();
					break;
			}
}

Render::~Render()
{
}
