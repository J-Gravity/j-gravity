/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 18:12:27 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/29 11:07:41 by elee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <checker.h>
#include <gmp.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

t_body			makebody(t_invector inpos, t_invector invel)
{
	t_body		newbody;

	newbody.mass = inpos.mass;
	newbody.position.x = inpos.x;
	newbody.position.y = inpos.y;
	newbody.position.z = inpos.z;
	newbody.velocity.x = invel.x;
	newbody.velocity.x = invel.y;
	newbody.velocity.z = invel.z;
	return (newbody);
}

void			accuracy_check(int cpucomp, int gpucomp)
{
	t_vector	cpu_buf;
	t_fvector	gpu_buf;
	mpf_t		diff_x;	
	mpf_t		diff_y;	
	mpf_t		diff_z;
    mpf_t       diff_all;
	mpf_t		total;
	double		cpu_mass;
	float		gpu_mass;
	int64_t		run_cpu;
    int64_t     run_gpu;

	printf("|--------CALCULATING ACCURACY--------|\n");
	read(cpucomp, &run_cpu, sizeof(int64_t));
    read(gpucomp, &run_gpu, sizeof(int64_t));
    if (run_cpu != run_gpu)
    {
        dprintf(2, "NO OF PARTICLES FOR CPU AND GPU DO NOT MATCH\n");
        exit(0);
    }
	mpf_init2(diff_x, 128);
	mpf_init2(diff_y, 128);
	mpf_init2(diff_z, 128);
	mpf_init2(diff_all, 128);
	mpf_init2(total, 512);
	run = 0;
    while (read(cpucomp, &cpu_buf, sizeof(t_vector)) && read(gpucomp, &gpu_buf, sizeof(t_fvector)))
    {
        read(cpucomp, &cpu_mass, sizeof(double));
        read(gpucomp, &gpu_mass, sizeof(float));
        mpf_set_d(diff_x, cpu_buf.x - gpu_buf.x);
        mpf_set_d(diff_y, cpu_buf.y - gpu_buf.y);
        mpf_set_d(diff_z, cpu_buf.z - gpu_buf.z);
        mpf_set_d(diff_all, 0.0);
        mpf_pow_ui(diff_x, diff_x, 2);
        mpf_pow_ui(diff_y, diff_y, 2);
        mpf_pow_ui(diff_z, diff_z, 2);
        mpf_add(diff_all, diff_all, diff_x);
        mpf_add(diff_all, diff_all, diff_y);
        mpf_add(diff_all, diff_all, diff_z);
        mpf_sqrt(diff_all, diff_all);
        mpf_add(total, diff_all);
    }
    mpf_div(total, total, run_cpu);
    printf("DEVIATION: %lf", mpf_get_d(total));
	mpf_clear(diff_x);
	mpf_clear(diff_y);
	mpf_clear(diff_z);
	mpf_clear(diff_all);
	mpf_clear(total);
}

int			main(int argc, char **argv)
{
	accuracy_check(open(argv[1], O_RDONLY), open(argv[2], O_RDONLY));
	return (0);
}
