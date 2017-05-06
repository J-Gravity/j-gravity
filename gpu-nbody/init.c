#include "nbody.h"

cl_float4 rand_star(int xmag, int ymag, int zmag)
{
    return((cl_float4){rand() % 1000 * (pow(-1, rand() % 2)) * __exp10(xmag - 3), \
                        rand() % 1000 * (pow(-1, rand() % 2)) * __exp10(ymag - 3), \
                        rand() % 1000 * (pow(-1, rand() % 2)) * __exp10(zmag - 3), \
                        __exp10(rand() % 2) * GENERIC_MASS});
}

cl_float4 rand_star_sphere(int mag)
{
    //mag means the sphere is 10^mag radius
    //basically multiply random unit vector by random percentage of 10^mag

    cl_float4 unit = (cl_float4){(float)(rand() % 1000), (float)(rand() % 1000), (float)(rand() % 1000), 0};
    float umag = sqrt(unit.x * unit.x + unit.y * unit.y + unit.z * unit.z);
    unit.x = unit.x / umag;
    unit.y = unit.y / umag;
    unit.z = unit.z / umag;

    float factor = ((rand() % 80) + 20) / 100.0;
    factor *= __exp10(mag);
    unit.x = unit.x * (pow(-1, rand() % 2)) * factor;
    unit.y = unit.y * (pow(-1, rand() % 2)) * factor;
    unit.z = unit.z * (pow(-1, rand() % 2)) * factor;
    unit.w = GENERIC_MASS;

    return (unit);
}

cl_float4 rand_vel(int xmag, int ymag, int zmag)
{
    cl_float4 vel;
    vel = rand_star(xmag, ymag, zmag);
    vel.w = 0;
    return vel;
}

cl_float4 *make_rotational_vels(cl_float4 *positions, unsigned int count)
{
    cl_float4 *vels = (cl_float4 *) calloc(count, sizeof(cl_float4));

    for (int i = 0; i < count; i++)
    {
        cl_float4 pos = positions[i];
        vels[i] = (cl_float4){-1 * pos.y / 80000, pos.x / 80000, 0, 0};
    }
    return (vels);
}

cl_float4 *make_stars(int n)
{
    cl_float4 *stars = (cl_float4 *) calloc(n, sizeof(cl_float4));

    for (int i = 0; i < n; i++)
        stars[i] = rand_star(5,5,5);
    return (stars);
}

cl_float4 *make_stars_sphere(int n, int mag)
{
    cl_float4 *stars = (cl_float4 *) calloc(n, sizeof(cl_float4));

    for (int i = 0; i < n; i++)
    {
        stars[i] = rand_star_sphere(mag);
    }
    return (stars);
}

cl_float4 *make_vels(int n)
{
    cl_float4 *vels = (cl_float4 *) calloc(n, sizeof(cl_float4));

    for (int i = 0; i < n; i++)
        vels[i] = rand_vel(-1,-1,-1);
    return (vels);
}