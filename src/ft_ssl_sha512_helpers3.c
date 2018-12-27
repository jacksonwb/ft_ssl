/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_sha512_helpers3.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/26 13:03:16 by jbeall            #+#    #+#             */
/*   Updated: 2018/12/26 15:51:16 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int		sha512_set_flags_dispatch(int i, char **argv, int flags, char *str)
{
	if (argv[i][0] == '-' && !(flags & MD5_REAL_ARGS_FLAG))
	{
		flags |= md_arg_parse(argv[i], &str, "sha512", &sha512_dispatch);
		if (flags & MD5_S_FLAG && str)
		{
			sha512_dispatch(str, MD5_MODE_STR, flags);
			flags ^= MD5_S_FLAG;
			flags |= MD5_CMD_FLAG;
		}
	}
	else if (flags & MD5_S_FLAG && !str)
	{
		str = argv[i];
		sha512_dispatch(str, MD5_MODE_STR, flags);
		flags ^= MD5_S_FLAG;
		flags |= MD5_CMD_FLAG;
	}
	else
	{
		sha512_dispatch(argv[i], MD5_MODE_FILE, flags);
		flags |= MD5_CMD_FLAG + MD5_REAL_ARGS_FLAG;
	}
	return (flags);
}

void	sha512_no_str_err(void)
{
	ft_printf("sha512 option requires an argument -- s\n");
	ft_printf("usage: sha512 [-pqr] [-s string] [files...]\n");
	exit(0);
}

void	sha512_init_digest(uint64_t *digest)
{
	digest[0] = 0x6a09e667f3bcc908;
	digest[1] = 0xbb67ae8584caa73b;
	digest[2] = 0x3c6ef372fe94f82b;
	digest[3] = 0xa54ff53a5f1d36f1;
	digest[4] = 0x510e527fade682d1;
	digest[5] = 0x9b05688c2b3e6c1f;
	digest[6] = 0x1f83d9abfb41bd6b;
	digest[7] = 0x5be0cd19137e2179;
}

int		get_next_sha512block_fd(t_md5_in in, t_sha512_block *block,
	uint64_t *len, int *close_flag)
{
	int i;
	int count;

	count = 0;
	if (in.fd == 0)
	{
		while (count < 128 && (i = read(in.fd, &(block->c[0]) +
			count, 128 - count)))
			count += i;
		in.flags == MD5_P_FLAG ? ft_printf("%.*s", count, block) : 0;
	}
	else
		count = read(in.fd, block, 128);
	i = count;
	*len += count;
	if (i < 112)
	{
		gnb_sha512_close_block(block, i, close_flag);
		block->i[15] = in.ssl_len_endian(len);
		return (0);
	}
	else if (i < 128)
		gnb_sha512_pad_block(block, close_flag, i);
	return (count);
}

int		get_next_sha512block_str(t_md5_in in, t_sha512_block *block,
	uint64_t *len, int *close_flag)
{
	int i;
	int count;

	i = 0;
	while (in.str[i] && i < 128)
	{
		block->c[i] = in.str[i];
		i++;
	}
	*len += i;
	count = i;
	if (i < 112)
	{
		block->c[i++] = *close_flag ? 0 : 1 << 7;
		while (i < 112)
			block->c[i++] = '\0';
		block->i[14] = 0;
		block->i[15] = in.ssl_len_endian(len);
		return (0);
	}
	else if (i < 128)
		gnb_sha512_pad_block(block, close_flag, i);
	return (count);
}
