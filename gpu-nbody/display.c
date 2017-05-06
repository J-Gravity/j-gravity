#include "nbody.h"

t_env *setup_env(void)
{
    t_env *env = (t_env *)calloc(1, sizeof(t_env));

    env->mlx = mlx_init();
    env->win = mlx_new_window(env->mlx, WINDOW_DIM, WINDOW_DIM, "each one is a setting sun");
    env->img = mlx_new_image(env->mlx, WINDOW_DIM, WINDOW_DIM);
    env->frameind = 0;
    env->delay_i = 0;
    env->delay = 4;
    return (env);
}

unsigned int tint(float z, unsigned int color)
{
    z /= WINDOW_FACTOR; // z is now -1000...1000
    z += 1000; // z is  now 0...2000
    unsigned int c = (unsigned int)((2000 - z) * 256);
    c = color + c * 256 * 256 * 256;
    return c;
}

void    draw_img_point(void *img, int x, int y, int c)
{
    int     bpp;
    int     size_line;
    int     endian;
    char    *imgbuff;
    int     i;

    imgbuff = mlx_get_data_addr(img, &bpp, &size_line, &endian);
    bpp /= 8;
    i = -1;
    if (0 <= x && x <= WINDOW_DIM && 0<= y && y <= WINDOW_DIM)
        while (++i < 4)
            imgbuff[y * size_line + (x * bpp + i)] = ((unsigned char *)&c)[i];
}

void    wipe_image(void *img)
{
    for (int i = 0; i < WINDOW_DIM; i++)
        for (int k = 0; k < WINDOW_DIM; k++)
            draw_img_point(img, i, k, 0x000000);
}

int my_loop_hook(void *param)
{
    t_env *env = (t_env*)param;

    if (env->delay_i == 0)
    {
        mlx_destroy_image(env->mlx, env->img);
        env->img = mlx_new_image(env->mlx, 1000, 1000);
        for(int i = 0; i < STARCOUNT; i++)
        {
            draw_img_point(env->img, env->frames[env->frameind][i].x / WINDOW_FACTOR + 500, env->frames[env->frameind][i].y / WINDOW_FACTOR + 500, 0xFFFFFF);
        }
        mlx_put_image_to_window(env->mlx, env->win, env->img, 0, 0);
        env->frameind = (env->frameind + 1) % FRAMECOUNT;
    }
    else
        env->delay_i = (env->delay_i + 1) % env->delay; 
    return (0);
}