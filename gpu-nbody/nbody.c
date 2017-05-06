/*By pmclaugh for J-Gravity, with code from Hands On OpenCL by Simon McIntosh-Smith & Tom Deakin and GPU Gems 3 by NVIDIA*/
#include "nbody.h"
#include "err_code.h"

void print_float4(cl_float4 v)
{
    printf("x: %f y: %f z: %f w: %f\n", v.x, v.y, v.z, v.w);
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
        exit(0);
    }

    // Create the compute kernel from the program
    k = clCreateKernel(p, name, &err);
    checkError(err, "Creating kernel");
    return (k);
}

int main (void)
{
    srand ( time(NULL) ); //before we do anything, seed rand() with the current time
    t_context   *context;
    cl_kernel   k_nbody;
    int err;

    context = setup_context();
    k_nbody = make_kernel(context, "pair3.cl", "nbody");
    printf("made kernel ok\n");

    //host-side data
    cl_float4*      h_p = make_stars_sphere(STARCOUNT, 5);
    cl_float4*      h_v = make_rotational_vels(h_p, STARCOUNT);
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


    size_t global = THREADCOUNT;
    size_t local = GROUPSIZE;
    //hook memory locations to kernel arguments
   
    size_t soften = SOFTENING;
    size_t timestep = TIME_STEP;
    float grav = G;
    size_t count = STARCOUNT;
    clSetKernelArg(k_nbody, 0, sizeof(cl_mem), &d_p_start);
    clSetKernelArg(k_nbody, 1, sizeof(cl_mem), &d_p_end);
    clSetKernelArg(k_nbody, 2, sizeof(cl_mem), &d_v_start);
    clSetKernelArg(k_nbody, 3, sizeof(cl_mem), &d_v_end);
    clSetKernelArg(k_nbody, 4, sizeof(cl_mem), &d_a);
    clSetKernelArg(k_nbody, 5, sizeof(cl_float4) * GROUPSIZE, NULL);
    clSetKernelArg(k_nbody, 6, sizeof(unsigned int), &soften);
    clSetKernelArg(k_nbody, 7, sizeof(unsigned int), &timestep);
    clSetKernelArg(k_nbody, 8, sizeof(float), &grav);
    clSetKernelArg(k_nbody, 9, sizeof(unsigned int), &count);

    //copy over initial data to device locations
    clEnqueueWriteBuffer(context->commands, d_p_start, CL_TRUE, 0, sizeof(cl_float4) * STARCOUNT, h_p, 0, NULL, NULL);
    clEnqueueWriteBuffer(context->commands, d_v_start, CL_TRUE, 0, sizeof(cl_float4) * STARCOUNT, h_v, 0, NULL, NULL);

    double rtime = wtime();
    printf("beginning calculation...\n");
    for(int i = 0; i < FRAMECOUNT; i++)
    {
        cl_event tick;
        err = clEnqueueNDRangeKernel(context->commands, k_nbody, 1, NULL, &global, &local, 0, NULL, &tick);
        checkError(err, "Enqueueing kernel");
        clEnqueueReadBuffer(context->commands, d_p_end, CL_TRUE, 0, sizeof(cl_float4) * count, output_frames[i], 1, &tick, NULL );
        clEnqueueCopyBuffer(context->commands, d_p_end, d_p_start, 0, 0, sizeof(cl_float4) * STARCOUNT, 1, &tick, NULL);
        clEnqueueCopyBuffer(context->commands, d_v_end, d_v_start, 0, 0, sizeof(cl_float4) * STARCOUNT, 1, &tick, NULL);
        clFinish(context->commands);
        if (i % (FRAMECOUNT / 10) == 0)
            printf("done %d of %d\n", i, FRAMECOUNT);
    }
    rtime = wtime() - rtime;
    printf("hell yeah, it took %lf seconds, which is ~%3f ticks/second\n", rtime, FRAMECOUNT / rtime);
    printf("you'll need to ctrl-C in the shell to close\n");
    rtime = wtime();
    for (int i = 0; i < FRAMECOUNT; i++)
    {
        frame_to_file(output_frames[i], STARCOUNT, i, "good-");
    }
    rtime = wtime() - rtime;
    printf("flushing to disk took %lf seconds\n", rtime);

    unsigned long test = 0;
    cl_float4 *readback = file_to_frame("good-0.jgrav", &test);

    print_float4(readback[1]);
    print_float4(output_frames[0][1]);
    // t_env *env = setup_env();
    // env->frames = output_frames;
    // mlx_loop_hook(env->mlx, my_loop_hook, env);
    // mlx_loop(env->mlx);
}