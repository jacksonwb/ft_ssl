/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_sha256_helpers2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jackson <jbeall@student.42.us.org>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/26 12:49:45 by jbeall            #+#    #+#             */
/*   Updated: 2018/12/27 10:29:13 by jackson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void		sha256_rotate_vars(t_sha256_vars *vars)
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

void		sha256_no_str_err(void)
{
	ft_printf("sha256 option requires an argument -- s\n");
	ft_printf("usage: sha256 [-pqr] [-s string] [files...]\n");
	exit(0);
}

uint64_t	sha256_len_endian(uint64_t *len)
{
	int			j;
	uint64_t	temp;
	uint64_t	new_len;

	j = 0;
	temp = 0;
	new_len = *len * 8;
	while (j < 8)
	{
		temp <<= 8;
		temp += new_len & 255;
		new_len >>= 8;
		++j;
	}
	return (temp);
}

void		sha256_init_digest(unsigned int *digest)
{
	digest[0] = 0x6a09e667;
	digest[1] = 0xbb67ae85;
	digest[2] = 0x3c6ef372;
	digest[3] = 0xa54ff53a;
	digest[4] = 0x510e527f;
	digest[5] = 0x9b05688c;
	digest[6] = 0x1f83d9ab;
	digest[7] = 0x5be0cd19;
}

void		sha256_print(char *str, unsigned int *digest, int flags, int mode)
{
	char	*c;
	int		i;

	i = 0;
	c = (mode == MD5_MODE_STR) ? "\"" : "";
	if (str && !(flags & MD5_Q_FLAG) && !(flags & MD5_R_FLAG))
		ft_printf("SHA256 (%s%s%s) = ", c, str, c);
	while (i < 8)
		ft_printf("%08x", digest[i++]);
	if (str && !(flags & MD5_Q_FLAG) && (flags & MD5_R_FLAG))
		ft_printf(" %s%s%s", c, str, c);
	ft_printf("\n");
}
