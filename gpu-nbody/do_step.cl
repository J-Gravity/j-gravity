#define TIMESTEP 10000
#define MAX_VEL 300000000
#define G 0.0000000000667408
#define GSTEP 0.0000000667408
__kernel void do_step(
    __global float4* p,
    __global float4* p_out,
    __global float4* v,
    __global float4* v_out,
    __global float4* a,
    const unsigned int count)
{
    int i = get_global_id(0);

    float4 vel = v[i];
    float4 acc = a[i];
    float4 pos = p[i];

    vel.x += acc.x * G *TIMESTEP;
    vel.y += acc.y * G *TIMESTEP;
    vel.z += acc.z * G *TIMESTEP;

    pos.x += vel.x * TIMESTEP;
    pos.y += vel.y * TIMESTEP;
    pos.z += vel.z * TIMESTEP;

    p_out[i] = pos;
    v_out[i] = vel;
}
