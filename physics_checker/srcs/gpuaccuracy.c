/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpuaccuracy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 18:12:27 by smifsud           #+#    #+#             */
/*   Updated: 2017/06/02 16:41:29 by smifsud          ###   ########.fr       */
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

void			accuracy_check_gpu(int cpucomp, int gpucomp)
{
	t_fvector	cpu_buf;
	t_fvector	gpu_buf;
	mpf_t		cpu_x;	
	mpf_t		cpu_y;	
	mpf_t		cpu_z;
	mpf_t		cpu_m;
	mpf_t		gpu_x;	
	mpf_t		gpu_y;	
	mpf_t		gpu_z;
	mpf_t		gpu_m;
	mpf_t		temp;
	float		cpu_mass;
	float		gpu_mass;
	int64_t		run;

	printf("|--------CALCULATING ACCURACY--------|\n");
	printf("|-----------------GPU1----------------|\n");
	read(cpucomp, &run, sizeof(int64_t));
	mpf_init2(cpu_x, 512);
	mpf_init2(cpu_y, 512);
	mpf_init2(cpu_z, 512);
	mpf_init2(cpu_m, 512);
	mpf_init2(gpu_x, 512);
	mpf_init2(gpu_y, 512);
	mpf_init2(gpu_z, 512);
	mpf_init2(gpu_m, 512);
	mpf_init2(temp, 64);
	printf("%lld\n", run);
	run = 0;
	while (read(cpucomp, &cpu_buf, sizeof(t_fvector)))
	{
		printf("%lld\n", run);
		read(cpucomp, &cpu_mass, sizeof(float));
		mpf_set_d(temp, cpu_mass);
		mpf_add(cpu_m, cpu_m, temp);
		mpf_set_d(temp, cpu_buf.x * cpu_mass);
		mpf_add(cpu_x, cpu_x, temp);
		mpf_set_d(temp, cpu_buf.y * cpu_mass);
		mpf_add(cpu_y, cpu_y, temp);
		mpf_set_d(temp, cpu_buf.z * cpu_mass);
		mpf_add(cpu_z, cpu_z, temp);
		run++;
	}
	mpf_div(cpu_x, cpu_x, cpu_m);
	mpf_div(cpu_y, cpu_y, cpu_m);
	mpf_div(cpu_z, cpu_z, cpu_m);
	
	printf("|-----------------GPU2----------------|\n");
	mpf_init2(gpu_x, 512);
	mpf_init2(gpu_y, 512);
	mpf_init2(gpu_z, 512);
	mpf_init2(gpu_m, 512);
	mpf_set_d(gpu_x, 0);
	mpf_set_d(gpu_y, 0);
	mpf_set_d(gpu_z, 0);
	mpf_set_d(gpu_m, 0);
	read(gpucomp, &run, sizeof(int64_t));
	while (read(gpucomp, &gpu_buf, sizeof(t_fvector)))
	{
		printf("debug\n");
		read(gpucomp, &gpu_mass, sizeof(float));
		mpf_set_d(temp, gpu_mass);
		mpf_add(gpu_m, gpu_m, temp);
		mpf_set_d(temp, gpu_buf.x * gpu_mass);
		mpf_add(gpu_x, gpu_x, temp);
		mpf_set_d(temp, gpu_buf.y * gpu_mass);
		mpf_add(gpu_x, gpu_x, temp);
		mpf_set_d(temp, gpu_buf.z * gpu_mass);
		mpf_add(gpu_x, gpu_x, temp);
	}
	mpf_div(gpu_x, gpu_x, gpu_m);
	mpf_div(gpu_y, gpu_y, gpu_m);
	mpf_div(gpu_z, gpu_z, gpu_m);

	printf("|-----------------DIFF---------------|\n");
	mpf_sub(cpu_x, cpu_x, gpu_x); 
	mpf_sub(cpu_y, cpu_y, gpu_y); 
	mpf_sub(cpu_z, cpu_z, gpu_z); 
	mpf_pow_ui(cpu_x, cpu_x, 2);
	mpf_pow_ui(cpu_y, cpu_y, 2);
	mpf_pow_ui(cpu_z, cpu_z, 2);
	mpf_add(cpu_x, cpu_x, cpu_y);
	mpf_add(cpu_x, cpu_x, cpu_z);
	mpf_sqrt(cpu_x, cpu_x);
	printf("DEVIATION: %lf", mpf_get_d(cpu_x));
	mpf_clear(cpu_x);
	mpf_clear(cpu_y);
	mpf_clear(cpu_z);
	mpf_clear(cpu_m);
	mpf_clear(gpu_x);
	mpf_clear(gpu_y);
	mpf_clear(gpu_z);
	mpf_clear(gpu_m);
	mpf_clear(temp);
}

/*
 * void			accuracy_check(int cpucomp, int gpucomp)
 * {
 * 	t_vector	buf;
 * 		t_fvector	cmpbuf;
 * 			mpf_t		bignum;
 * 				mpf_t		compare;
 * 					mpf_t		reg_x;
 * 						mpf_t		reg_y;
 * 							mpf_t		reg_z;
 * 								mpf_t		math;
 * 									int64_t		run;
 *
 * 										printf("|--------CALCULATING ACCURACY--------|\n");
 * 											read(cpucomp, &run, sizeof(int64_t));
 * 												run = 0;
 * 													mpf_init2(bignum, 512);
 * 														mpf_init2(compare, 512);
 * 															mpf_init2(math, 512);
 * 																mpf_init2(reg, 64);
 * 																	mpf_set_d(bignum, 0);
 * 																		mpf_set_d(compare, 0);
 * 																			printf("CONFIRM\n");
 * 																				while (read(cpucomp, &(buf), sizeof(t_vector)))
 * 																					{
 * 																						//	printf("RUN: %lld\n", run);
 * 																							//	printf("%.40lf, %.40lf, %lf, %lf, %lf\n", mpf_get_d(compare), finddist(buf), buf.x, buf.y, buf.z);
 * 																									mpf_set_d(reg, finddist(buf));
 * 																											printf("paus\n");
 * 																													mpf_add(compare, compare, reg);
 * 																															run++;
 * 																																}
 * 																																	printf("CALCULATED CPU SAMPLESET\n\n\n");
 * 																																		mpf_div_ui(compare, compare, run);
 * 																																			read(gpucomp, &run, sizeof(int64_t));
 * 																																				run = 0;
 * 																																					printf("DEBUGGY\n");
 * 																																						while (read(gpucomp, &cmpbuf, sizeof(t_fvector)))
 * 																																							{
 * 																																									printf("RUN: %lld\n", run);
 * 																																											buf.x = cmpbuf.x;
 * 																																													buf.y = cmpbuf.y;
 * 																																															buf.z = cmpbuf.z;
 * 																																																	mpf_set_d(reg, finddist(cmpbuf));
 * 																																																			mpf_add(bignum, bignum, reg);
 * 																																																					run++;
 * 																																																							if (1 > read(gpucomp, &cmpbuf, sizeof(float)))
 * 																																																										break ;
 * 																																																											}
 * 																																																												printf("CALCULATED GPU SAMPLESET\n\n\n");
 * 																																																													mpf_div_ui(bignum, bignum, run);
 * 																																																														mpf_add(math, bignum, compare);
 * 																																																															mpf_div_ui(math, math, 2);
 * 																																																																mpf_sub(bignum, bignum, compare);
 * 																																																																	mpf_abs(bignum, bignum);
 * 																																																																		mpf_div(bignum, bignum, math);
 * 																																																																			mpf_mul_ui(bignum, bignum, 100);
 * 																																																																				printf("DEVIATION : %lf%%\n", mpf_get_d(bignum));
 * 																																																																					mpf_clear(bignum);
 * 																																																																						mpf_clear(reg);
 * 																																																																						}
 * 																																																																						*/
int			main(int argc, char **argv)
{
	accuracy_check_gpu(open(argv[1], O_RDONLY), open(argv[2], O_RDONLY));
	return (0);
}
