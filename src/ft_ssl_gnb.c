/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_gnb.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/26 12:41:16 by jbeall            #+#    #+#             */
/*   Updated: 2018/12/26 13:39:46 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int				get_next_block_str(t_md5_in in, t_md5_block *block,
	uint64_t *len, int *close_flag)
{
	int i;
	int count;

	i = -1;
	while (in.str[++i] && i < 64)
		block->c[i] = in.str[i];
	*len += i;
	count = i;
	if (i < 56)
	{
		block->c[i++] = *close_flag ? 0 : 1 << 7;
		while (i < 56)
			block->c[i++] = '\0';
		block->u64[7] = in.ssl_len_endian(len);
		return (0);
	}
	else if (i < 64)
	{
		block->c[i++] = 1 << 7;
		*close_flag = 1;
		while (i < 64)
			block->c[i++] = '\0';
	}
	return (count);
}

void			gnb_helper1(t_md5_in *in, t_md5_block *block,
	int *count, int *i)
{
	while (*count < 64 && (*i = read(in->fd, &(block->c[0]) +
		*count, 64 - *count)))
		*count += *i;
	in->flags == MD5_P_FLAG ? ft_printf("%.*s", *count, block) : 0;
}

int				get_next_block_fd(t_md5_in in, t_md5_block *block,
	uint64_t *len, int *close_flag)
{
	int i;
	int count;

	count = 0;
	if (in.fd == 0)
		gnb_helper1(&in, block, &count, &i);
	else
		count = read(in.fd, block, 64);
	i = count;
	*len += count;
	if (i < 56)
	{
		block->c[i++] = *close_flag ? 0 : 1 << 7;
		while (i < 56)
			block->c[i++] = '\0';
		block->u64[7] = in.ssl_len_endian(len);
		return (0);
	}
	else if (i < 64 && (*close_flag = 1))
	{
		block->c[i++] = 1 << 7;
		while (i < 64)
			block->c[i++] = '\0';
	}
	return (count);
}

unsigned int	get_md5_s(int i)
{
	static unsigned int md5_s[64] = {
		7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
		5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
		4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16,
		23, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

	return (md5_s[i]);
}

unsigned int	get_md5_k(int i)
{
	static unsigned int md5_k[64] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

	return (md5_k[i]);
}
