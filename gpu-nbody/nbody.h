#ifndef NBODY_H
# define NBODY_H
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# ifdef __APPLE__
# include <OpenCL/opencl.h>
# include <unistd.h>
# else
# include <CL/cl.h>
# endif
// # include "err_code.h"
# include "minilibx_macos/mlx.h"
# include <string.h>
# include "libft.h"
# include <time.h>

//pick up device type from compiler command line or from
//the default type
# ifndef DEVICE
# define DEVICE CL_DEVICE_TYPE_DEFAULT
# endif


extern double wtime();       // returns time since some fixed past point (wtime.c)
extern int output_device_info(cl_device_id );

#define STARCOUNT pow(2, 15)
#define THREADCOUNT pow(2, 15);
#define GROUPSIZE 256
#define SOLAR_MASS 1.989 * __exp10(30)
#define GENERIC_MASS __exp10(11)
#define FRAMECOUNT 30
#define WINDOW_FACTOR 500
#define WINDOW_DIM 1000 
#define SOFTENING 10000
#define G 0.0000000000667408
#define TIME_STEP 2000

typedef struct s_env
{
    void    *mlx;
    void    *win;
    void    *img;
    cl_float4 **frames;
    int frameind;
    int delay;
    int delay_i;
}               t_env;

typedef struct s_context
{
    cl_device_id device_id;
    cl_context context;
    cl_command_queue commands;
}               t_context;

t_env *setup_env(void);
int my_loop_hook(void *param);


cl_float4 *make_rotational_vels(cl_float4 *positions, unsigned int count);
cl_float4 *make_stars_sphere(int n, int mag);
cl_float4 rand_star(int xmag, int ymag, int zmag);

char *load_cl_file(char *filename);
void    frame_to_file(cl_float4 *frame, unsigned long n, unsigned long framenumber, char *title);
cl_float4 *file_to_frame(char *filename, unsigned long *count);

#endif