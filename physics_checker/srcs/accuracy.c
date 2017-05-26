/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accuracy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 18:12:27 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/26 16:46:41 by smifsud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <checker.h>
#include <gmp.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void			accuracy_check(int cpucomp, int gpucomp)
{
	t_vector	buf;
	t_fvector	cmpbuf;
	mpf_t		bignum;
	mpf_t		compare;
	mpf_t		reg;
	mpf_t		math;
	int64_t		run;

	printf("|--------CALCULATING ACCURACY--------|\n");
	read(cpucomp, &run, sizeof(int64_t));
	run = 0;
	mpf_init2(bignum, 512);
	mpf_init2(compare, 512);
	mpf_init2(math, 512);
	mpf_init2(reg, 64);
	mpf_set_d(bignum, 0);
	mpf_set_d(compare, 0);
	printf("CONFIRM\n");
	while (read(cpucomp, &(buf), sizeof(t_vector)))
	{
		printf("RUN: %lld\n", run);
		printf("%.40lf, %.40lf, %lf, %lf, %lf\n", mpf_get_d(compare), finddist(buf), buf.x, buf.y, buf.z);
		mpf_set_d(reg, finddist(buf));
		printf("paus\n");
		mpf_add(compare, compare, reg);
		run++;
		if (1 > read(cpucomp, &buf, sizeof(double)))
			break ;
	}
	printf("CALCULATED CPU SAMPLESET\n\n\n");
	mpf_div_ui(compare, compare, run);
	read(gpucomp, &run, sizeof(int64_t));
	run = 0;
	printf("DEBUGGY\n");
	while (read(gpucomp, &cmpbuf, sizeof(t_fvector)))
	{
		printf("RUN: %lld\n", run);
		buf.x = cmpbuf.x;
		buf.y = cmpbuf.y;
		buf.z = cmpbuf.z;
		mpf_set_d(reg, finddist(buf));
		mpf_add(bignum, bignum, reg);
		run++;
		if (1 > read(gpucomp, &cmpbuf, sizeof(float)))
			break ;
	}
	printf("CALCULATED GPU SAMPLESET\n\n\n");
	mpf_div_ui(bignum, bignum, run);
	mpf_add(math, bignum, compare);
	mpf_div_ui(math, math, 2);
	mpf_sub(bignum, bignum, compare);
	mpf_abs(bignum, bignum);
	mpf_div(bignum, bignum, math);
	mpf_mul_ui(bignum, bignum, 100);
	printf("DEVIATION : %lf%%\n", mpf_get_d(bignum));
	mpf_clear(bignum);
	mpf_clear(reg);
}

int			main(int argc, char **argv)
{
	accuracy_check(open(argv[1], O_RDONLY), open(argv[2], O_RDONLY));
	return (0);
}
