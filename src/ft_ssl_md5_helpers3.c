/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_md5_helpers3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/26 12:47:40 by jbeall            #+#    #+#             */
/*   Updated: 2018/12/26 12:48:36 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	md5_init_vars(unsigned int *dgst, t_md5_vars *vars)
{
	vars->a = dgst[0];
	vars->b = dgst[1];
	vars->c = dgst[2];
	vars->d = dgst[3];
}

void	md5_add_vars(unsigned int *dgst, t_md5_vars *vars)
{
	dgst[0] += vars->a;
	dgst[1] += vars->b;
	dgst[2] += vars->c;
	dgst[3] += vars->d;
}

void	md5_do_op(int i, t_md5_vars *vars)
{
	if (i < 16)
	{
		vars->f = md5_f(vars->b, vars->c, vars->d);
		vars->g = i;
	}
	else if (i < 32)
	{
		vars->f = md5_g(vars->b, vars->c, vars->d);
		vars->g = ((5 * i) + 1) % 16;
	}
	else if (i < 48)
	{
		vars->f = md5_h(vars->b, vars->c, vars->d);
		vars->g = ((3 * i) + 5) % 16;
	}
	else if (i < 64)
	{
		vars->f = md5_i(vars->b, vars->c, vars->d);
		vars->g = (7 * i) % 16;
	}
}
