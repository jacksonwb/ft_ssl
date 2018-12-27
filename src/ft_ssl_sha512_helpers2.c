/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_sha512_helpers2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/26 13:01:43 by jbeall            #+#    #+#             */
/*   Updated: 2018/12/26 13:13:52 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	sha512_block_init(uint64_t *digest, t_sha512_vars *vars)
{
	vars->a = digest[0];
	vars->b = digest[1];
	vars->c = digest[2];
	vars->d = digest[3];
	vars->e = digest[4];
	vars->f = digest[5];
	vars->g = digest[6];
	vars->h = digest[7];
}

void	sha512_add_digest(uint64_t *digest, t_sha512_vars *vars)
{
	digest[0] += vars->a;
	digest[1] += vars->b;
	digest[2] += vars->c;
	digest[3] += vars->d;
	digest[4] += vars->e;
	digest[5] += vars->f;
	digest[6] += vars->g;
	digest[7] += vars->h;
}

void	sha512_rotate_vars(t_sha512_vars *vars)
{
	vars->h = vars->g;
	vars->g = vars->f;
	vars->f = vars->e;
	vars->e = vars->d + vars->temp1;
	vars->d = vars->c;
	vars->c = vars->b;
	vars->b = vars->a;
	vars->a = vars->temp1 + vars->temp2;
}

void	gnb_sha512_close_block(t_sha512_block *block, int i, int *close_flag)
{
	block->c[i++] = *close_flag ? 0 : 1 << 7;
	while (i < 112)
		block->c[i++] = '\0';
	block->i[14] = 0;
}

void	gnb_sha512_pad_block(t_sha512_block *block, int *close_flag, int i)
{
	block->c[i++] = 1 << 7;
	*close_flag = 1;
	while (i < 128)
		block->c[i++] = '\0';
}
