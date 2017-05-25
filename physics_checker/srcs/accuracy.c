/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accuracy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smifsud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 18:12:27 by smifsud           #+#    #+#             */
/*   Updated: 2017/05/24 19:39:06 by smifsud          ###   ########.fr       */
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
	mpf_t		bignum;
	mpf_t		compare;
	mpf_t		reg;
	mpf_t		math;
	size_t		run;

	printf("|--------CALCULATING ACCURACY--------|");
	run = 0;
	mpf_init2(bignum, 256);
	mpf_init2(compare, 256);
	mpf_init2(math, 256);
	mpf_init2(reg, 64);
	mpf_set_d(bignum, 0);
	mpf_set_d(compare, 0);
	while (read(cpucomp, &buf, sizeof(t_vector)))
	{
		mpf_set_d(reg, finddist(buf));
		mpf_add(compare, compare, reg);
		run++;
	}
	printf("CALCULATED CPU SAMPLESET");
	mpf_div_ui(compare, compare, run);
	run = 0;
	while (read(gpucomp, &buf, sizeof(t_vector)))
	{
		mpf_set_d(reg, finddist(buf));
		mpf_add(bignum, bignum, reg);
		run++;
	}
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
