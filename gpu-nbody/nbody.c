/*By pmclaugh for J-Gravity, with code from Hands On OpenCL by Simon McIntosh-Smith & Tom Deakin and GPU Gems 3 by NVIDIA*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#include <unistd.h>
#else
#include <CL/cl.h>
#endif
#include "err_code.h"
#include "minilibx_macos/mlx.h"
#include <string.h>

//pick up device type from compiler command line or from
//the default type
#ifndef DEVICE
#define DEVICE CL_DEVICE_TYPE_DEFAULT
#endif


extern double wtime();       // returns time since some fixed past point (wtime.c)
extern int output_device_info(cl_device_id );


//------------------------------------------------------------------------------

#define STARCOUNT (32768)    // length of vectors a, b, and c
#define SOLAR_MASS 1.989 * __exp10(30)
#define FRAMECOUNT 120
#define WINDOW_FACTOR (2000)
#define WINDOW_DIM 1000 

typedef struct s_env
{
    void    *mlx;
    void    *win;
    cl_float4 **frames;
    int frameind;
}               t_env;

typedef struct s_context
{
    cl_device_id device_id;
    cl_context context;
    cl_command_queue commands;
}               t_context;

char *load_cl_file(char *filename)
{
    char *source;
    int fd;

    source = (char *)calloc(1,2048);
    fd = open(filename, O_RDONLY);
    read(fd, source, 2048);
    return (source);
}

void print_float4(cl_float4 v)
{
    printf("x: %f y: %f z: %f w: %f\n", v.x, v.y, v.z, v.w);
}

cl_float4 rand_star(int xmag, int ymag, int zmag)
{
    return((cl_float4){rand() % 1000 * (pow(-1, rand() % 2)) * __exp10(xmag - 3), \
                        rand() % 1000 * (pow(-1, rand() % 2)) * __exp10(ymag - 3), \
                        rand() % 1000 * (pow(-1, rand() % 2)) * __exp10(zmag - 3), \
                        (pow(1, rand() % 2)) * __exp10(9)});
}

int my_loop_hook(void *param)
{
    t_env *env = (t_env*)param;

    mlx_clear_window(env->mlx, env->win);
    for(int i = 0; i < STARCOUNT; i++)
    {
        if (env->frames[env->frameind][i].w > 0)
            mlx_pixel_put(env->mlx, env->win, env->frames[env->frameind][i].x / WINDOW_FACTOR + 500, env->frames[env->frameind][i].y / WINDOW_FACTOR + 500, 0xFFFFFF);
        else
            mlx_pixel_put(env->mlx, env->win, env->frames[env->frameind][i].x / WINDOW_FACTOR + 500, env->frames[env->frameind][i].y / WINDOW_FACTOR + 500, 0x0000FF);
    }
    env->frameind = (env->frameind + 1) % FRAMECOUNT;
    return (0);
}



t_context *setup_context(void)
{
     cl_uint numPlatforms;
     t_context *c = (t_context *)calloc(1, sizeof(t_context));
     int err;

    // Find number of platforms
    err = clGetPlatformIDs(0, NULL, &numPlatforms);

    // Get all platforms
    cl_platform_id Platform[numPlatforms];
    err = clGetPlatformIDs(numPlatforms, Platform, NULL);
    checkError(err, "Getting platforms");

    // Secure a GPU
    for (int i = 0; i < numPlatforms; i++)
    {
        err = clGetDeviceIDs(Platform[i], DEVICE, 1, &(c->device_id), NULL);
        if (err == CL_SUCCESS)
        {
            break;
        }
    }

    if (c->device_id == NULL)
        checkError(err, "Finding a device");

    // Create a compute context
    c->context = clCreateContext(0, 1, &(c->device_id), NULL, NULL, &err);
    checkError(err, "Creating context");

    // Create a command queue
    c->commands = clCreateCommandQueue(c->context, (c->device_id), 0, &err);
    checkError(err, "Creating command queue");
    return (c);
}

cl_kernel   make_kernel(t_context *c, char *sourcefile, char *name)
{
    cl_kernel k;
    cl_program p;
    int err;
    char *source;

    source = load_cl_file(sourcefile);
    p = clCreateProgramWithSource(c->context, 1, (const char **) & source, NULL, &err);
    checkError(err, "Creating program");

    // Build the program
    err = clBuildProgram(p, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n%s\n", err_code(err));
        clGetProgramBuildInfo(p, c->device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        return NULL;
    }

    // Create the compute kernel from the program
    k = clCreateKernel(p, name, &err);
    checkError(err, "Creating kernel");
    return (k);
}

cl_float4 *make_stars(int n)
{
    cl_float4 *stars = (cl_float4 *) calloc(n, sizeof(cl_float4));

    for (int i = 0; i < n; i++)
        stars[i] = rand_star(5,5,5);
    return (stars);
}

t_env *setup_env(void)
{
    t_env *env = (t_env *)calloc(1, sizeof(t_env));

    env->mlx = mlx_init();
    env->win = mlx_new_window(env->mlx, WINDOW_DIM, WINDOW_DIM, "fuckin lit dude");
    env->frameind = 0;
    return (env);
}

int main (void)
{
    size_t      count;
    t_context   *context;
    cl_kernel   k_pairforce;
    cl_kernel   k_step;

    context = setup_context();
    k_pairforce = make_kernel(context, "pairforce.cl", "pair_force");
    printf("made pairforce ok\n");
    k_step = make_kernel(context, "do_step.cl", "do_step");
    printf("made step ok\n");

    //host-side data
    cl_float4*      h_p = make_stars(STARCOUNT);
    cl_float4*      h_a = (cl_float4 *) calloc(STARCOUNT, sizeof(cl_float4));
    cl_float4*      h_v = (cl_float4 *) calloc(STARCOUNT, sizeof(cl_float4));
    cl_float4**     output_frames = (cl_float4 **) calloc(STARCOUNT, sizeof(cl_float4*));
    for (int i = 0; i < STARCOUNT; i++)
        output_frames[i] = (cl_float4 *) calloc(STARCOUNT, sizeof(cl_float4));

    //device-side data
    cl_mem      d_p_start;
    cl_mem      d_a;
    cl_mem      d_v_start;
    cl_mem      d_v_end;
    cl_mem      d_p_end;
    //inputs
    d_p_start = clCreateBuffer(context->context, CL_MEM_READ_ONLY, sizeof(cl_float4) * STARCOUNT, NULL, NULL);
    d_v_start = clCreateBuffer(context->context, CL_MEM_READ_ONLY, sizeof(cl_float4) * STARCOUNT, NULL, NULL);
    //outputs
    d_a = clCreateBuffer(context->context, CL_MEM_READ_WRITE, sizeof(cl_float4) * STARCOUNT, NULL, NULL);
    d_v_end = clCreateBuffer(context->context, CL_MEM_WRITE_ONLY, sizeof(cl_float4) * STARCOUNT, NULL, NULL);
    d_p_end = clCreateBuffer(context->context, CL_MEM_WRITE_ONLY, sizeof(cl_float4) * STARCOUNT, NULL, NULL);

    //hook memory locations to kernel arguments
    count = STARCOUNT;
    clSetKernelArg(k_pairforce, 0, sizeof(cl_mem), &d_p_start);
    clSetKernelArg(k_pairforce, 1, sizeof(cl_mem), &d_a);
    clSetKernelArg(k_pairforce, 2, sizeof(unsigned int), &count);
    
    clSetKernelArg(k_step, 0, sizeof(cl_mem), &d_p_start);
    clSetKernelArg(k_step, 1, sizeof(cl_mem), &d_p_end);
    clSetKernelArg(k_step, 2, sizeof(cl_mem), &d_v_start);
    clSetKernelArg(k_step, 3, sizeof(cl_mem), &d_v_end);
    clSetKernelArg(k_step, 4, sizeof(cl_mem), &d_a);
    clSetKernelArg(k_step, 5, sizeof(unsigned int), &count);

    //copy over initial data to device locations
    clEnqueueWriteBuffer(context->commands, d_p_start, CL_TRUE, 0, sizeof(cl_float4) * STARCOUNT, h_p, 0, NULL, NULL);
    clEnqueueWriteBuffer(context->commands, d_v_start, CL_TRUE, 0, sizeof(cl_float4) * STARCOUNT, h_v, 0, NULL, NULL);

    //loop
    /*
    call pairforce kernel to put values in d_a
    call step kernel to use d_a to put values in d_p_end and d_v_end
    copy d_p_end to d_p, d_v_end to d_v, loop (this should be done on device without reading back to host)
    loop
    */
    double rtime = wtime();
    size_t global = count;
    printf("beginning calculation...\n");
    for(int i = 0; i < FRAMECOUNT; i++)
    {
        cl_event forces;
        cl_event step;
        clEnqueueNDRangeKernel(context->commands, k_pairforce, 1, NULL, &global, NULL, 0, NULL, &forces);
        clEnqueueNDRangeKernel(context->commands, k_step, 1, NULL, &global, NULL, 1, &forces, &step);
        clEnqueueReadBuffer(context->commands, d_p_end, CL_TRUE, 0, sizeof(cl_float4) * count, output_frames[i], 1, &step, NULL );
        clEnqueueCopyBuffer(context->commands, d_p_end, d_p_start, 0, 0, sizeof(cl_float4) * STARCOUNT, 1, &step, NULL);
        clEnqueueCopyBuffer(context->commands, d_v_end, d_v_start, 0, 0, sizeof(cl_float4) * STARCOUNT, 1, &step, NULL);
        clFinish(context->commands);
        if (i % (FRAMECOUNT / 10) == 0)
            printf("done %d of %d\n", i, FRAMECOUNT);
    }
    rtime = wtime() - rtime;
    printf("hell yeah, it took %lf seconds, which is ~%d ticks/second\n", rtime, (int)(FRAMECOUNT / rtime));
    printf("you'll need to ctrl-C in the shell to close\n");
    t_env *env = setup_env();
    env->frames = output_frames;
    mlx_loop_hook(env->mlx, my_loop_hook, env);
    mlx_loop(env->mlx);
}