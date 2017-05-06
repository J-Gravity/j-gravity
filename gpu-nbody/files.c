#include "nbody.h"

char *load_cl_file(char *filename)
{
    char *source;
    int fd;

    source = (char *)calloc(1,4096);
    fd = open(filename, O_RDONLY);
    read(fd, source, 4096);
    return (source);
}

void    stars_from_file(cl_float4 **positions, cl_float4 **velocities, int n, char *filename)
{
	//this was for loading from standardized n-body formats
	//not much use now that we've made our own format
    int fd = open(filename, O_RDONLY);
    *positions = (cl_float4 *) calloc(n, sizeof(cl_float4));
    *velocities = (cl_float4 *) calloc(n, sizeof(cl_float4));

    for (int i = 0; i < n; i++)
    {
        char *star;
        get_next_line(fd, &star);
        char **split = ft_strsplit(star, ' ');
        (*positions)[i] = (cl_float4){atof(split[1]), atof(split[2]), atof(split[3]), atof(split[0])};
        (*velocities)[i] = (cl_float4){atof(split[4]), atof(split[5]), atof(split[6]), 0.0};
    }
    close(fd);
}

void    frame_to_file(cl_float4 *frame, unsigned long n, unsigned long framenumber, char *title)
{
    int fd;

    //currently has a bug where files will end up being all 0's after a while.
    //I thought fsync would fix this but it hasn't
    char *filename = ft_strjoin(title, ft_itoa(framenumber));
    filename = ft_strjoin(filename, ft_strdup(".jgrav"));
    unsigned long *l = &n;
    fd = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IROTH);
    write(fd,l,sizeof(unsigned long));
    char *buff = ft_memalloc(4096);
    for (unsigned long i = 0; i < sizeof(cl_float4) * n; i += 4096)
    {
        bzero(buff, 4096);
        memcpy(buff, &(frame[i]), 4096);
        write(fd, buff, 4096);
        fsync(fd);
    }
    close(fd);
}

cl_float4 *file_to_frame(char *filename, unsigned long *count)
{
    int fd = open(filename, O_RDONLY);
    char *buff = ft_memalloc(4096);
    read(fd, buff, sizeof(unsigned long));
    ft_memcpy(count, buff, sizeof(unsigned long));
    cl_float4 *frame = ft_memalloc(*count);
    for (int j = 0; j < *count * sizeof(cl_float4); j += 4096)
    {
        bzero(buff, 4096);
        read(fd, buff, 4096);
        memcpy(frame + j, buff, 4096);
    }
    close(fd);
    return (frame);
}