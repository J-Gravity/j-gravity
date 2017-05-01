#define SOFTEN 10000
__kernel void pair_force(
    __global float4* p,
    __global float4* f,
    const unsigned int count)
{
    int i = get_global_id(0);
    float3 r;
    float4 force = {0,0,0,0};
    for(int j = 0; j < count; j++)
    {
        r.x = p[j].x - p[i].x;
        r.y = p[j].y - p[i].y;
        r.z = p[j].z - p[i].z;
        float distSquare = r.x * r.x + r.y *r.y + r.z * r.z + SOFTEN;
        float distSqCube = distSquare * distSquare * distSquare;
        float s = p[j].w / sqrt(distSqCube);
        force.x = force.x + r.x *s;
        force.y = force.y + r.y *s;
        force.z = force.z + r.z *s;
    }
    f[i] = force;
}